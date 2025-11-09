#include "vm.h"
#include "compiler.h"
#include "chunk.h"
#include "debug.h"
#include "memory.h"
#include "value.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

Vm vm;  // Static variable

static void resetStack() {
    vm.stackCount = 0;
    vm.stackCapacity = 0;
    vm.stackTop = vm.stack;
}


static void runtimeError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.chunk->code - 1;
    int line = getLine(vm.chunk, instruction);
    fprintf(stderr, "[line %d] in script\n", line);
    resetStack();
}


Value pop() {
    vm.stackTop--;
    vm.stackCount--;
    return *vm.stackTop;
}

void push(Value value) {
    if(vm.stackCapacity < vm.stackCount + 1) {
        printf("[GROWING STACK] Old capacity: %d\n", vm.stackCapacity);
        int oldCapacity = vm.stackCapacity;
        vm.stackCapacity = GROW_CAPACITY(oldCapacity);
        vm.stack = GROW_ARRAY(Value, vm.stack, oldCapacity, vm.stackCapacity);
        // New address for the top stack pointer
        vm.stackTop = vm.stack + vm.stackCount;
        printf("[GROWING STACK] New capacity: %d\n", vm.stackCapacity);
    }
    *vm.stackTop = value;

    vm.stackTop++;
    vm.stackCount++;
}

void initVM() {
    resetStack();
}

void freeVM() {

}

InterpretResult interpret(const char* source) {
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    freeChunk(&chunk);
    return result;
}

static Value peek(int distance) {
    return vm.stackTop[-1 - distance];
}

static InterpretResult run() {

    // Macros def
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
    #define READ_CONSTANT_LONG() (vm.chunk->constants.values[READ_BYTE() \
        | (READ_BYTE() << 8)\
        | (READ_BYTE() << 16)])
    
    // External do-while permit to append the semicolon at end of the block
    // on macro exec
    #define BINARY_OP(valueType, op) \
    do { \
      if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
        runtimeError("Operands must be numbers."); \
        return INTERPRET_RUNTIME_ERROR; \
      } \
      double b = AS_NUMBER(pop()); \
      double a = AS_NUMBER(pop()); \
      push(valueType(a op b)); \
    } while (false)

    // Main loop
    int i = 0;
    while(1) {
        // #ifdef DEBUG_TRACE_EXECUTION
        //     // Stack visibility
        //     for(Value* val = vm.stack; val < vm.stackTop; val++) {
        //         printf("[ %g ]", *val);
        //     }
        //     printf("\n");
        //     disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
        // #endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_RETURN:
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            case OP_CONSTANT:
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            case OP_CONSTANT_LONG:
                Value constant_long = READ_CONSTANT_LONG();
                push(constant_long);
                break;
            case OP_NEGATE:
                if(!IS_NUMBER(peek(0))) {
                    runtimeError("Operand msut be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            case OP_ADD:            BINARY_OP(NUMBER_VAL, +); break;
            case OP_SUBSTRACT:      BINARY_OP(NUMBER_VAL, -); break;
            case OP_MULTIPLY:       BINARY_OP(NUMBER_VAL, *); break;
            case OP_DIVIDE:         BINARY_OP(NUMBER_VAL, /); break;
            case OP_NIL:            push(NIL_VAL); break;
            case OP_FALSE:          push(BOOL_VAL(false)); break;
            case OP_TRUE:           push(BOOL_VAL(true)); break;
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_CONSTANT_LONG
#undef BINARY_OP
}
