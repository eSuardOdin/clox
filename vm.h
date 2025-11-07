#ifndef VM_H
#define VM_H

#include "chunk.h"
#include "value.h"

#define STACK_INIT 256

typedef struct {
    int stackCount;
    int stackCapacity;
    // Chunk the VM is running
    Chunk* chunk;
    // Instruction Pointer (Program Counter)
    uint8_t* ip;
    // Stack to put operators/operandes in
    Value* stack;
    // Top a the Value stack (+1)
    Value* stackTop;
} Vm;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

// -- VM functions --

void initVM();
InterpretResult interpret(const char* source);
void freeVM();
static InterpretResult run();


// -- Stack functions --

static void resetStack();
Value pop();
void push(Value value);

#endif