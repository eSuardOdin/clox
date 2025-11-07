#include "vm.h"
#include "compiler.h"
#include "chunk.h"
#include "debug.h"
#include "memory.h"
#include "value.h"
#include <stdlib.h>
#include <stdio.h>

Vm vm;  // Static variable

static void resetStack() {
    printf("[vm.c][resetStack()] => Entering resetStack()");
    if(vm.stack == NULL) {
        vm.stack = (Value*)malloc(STACK_INIT);
    }
    vm.stackCount = 0;
    vm.stackCapacity = 0;
    vm.stackTop = vm.stack; // ?
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
    printf("[PUSH] Putting: %g at stack[%d]\n", value, (int)(vm.stackTop - vm.stack));
    *vm.stackTop = value;
    printf("[PUSH] Well ?\n");

    vm.stackTop++;
    vm.stackCount++;
}

void initVM() {
    printf("[vm.c][initVM()] => Entering initVM()");
    resetStack();
}

void freeVM() {

}

InterpretResult interpret(const char* source) {
    // Debug
    printf("[vm.c][interpret()] => Entering interpret()");
    if(!compile(source, vm.chunk)) {
        freeChunk(vm.chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    

    return INTERPRET_OK;
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
                // push(-pop());
                *(vm.stackTop -1) = -(*(vm.stackTop - 1));
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
