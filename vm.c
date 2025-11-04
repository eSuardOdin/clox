#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"
#include <stdio.h>

Vm vm;  // Static variable

static void resetStack() {
    vm.stackTop = vm.stack;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

void initVM() {
    resetStack();
}

void freeVM() {

}

InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
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
    #define BINARY_OP(op) \
    do { \
        double b = pop();\
        double a = pop();\
        push(a op b);\
    } while(false)\

    // Main loop
    int i = 0;
    while(1) {
        #ifdef DEBUG_TRACE_EXECUTION
            // Stack visibility
            for(Value* val = vm.stack; val < vm.stackTop; val++) {
                printf("[ %g ]", *val);
            }
            printf("\n");
            disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
        #endif
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
                push(-pop());
                break;
            case OP_ADD:            BINARY_OP(+); break;
            case OP_SUBSTRACT:      BINARY_OP(-); break;
            case OP_MULTIPLY:       BINARY_OP(*); break;
            case OP_DIVIDE:         BINARY_OP(/); break;
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_CONSTANT_LONG
#undef BINARY_OP
}