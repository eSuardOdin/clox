#ifndef VM_H
#define VM_H

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    // Chunk the VM is running
    Chunk* chunk;
    // Instruction Pointer (Program Counter)
    uint8_t* ip;
    // Stack to put operators/operandes in
    Value stack[STACK_MAX];
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
void freeVM();
InterpretResult interpret(Chunk* chunk);
static InterpretResult run();


// -- Stack functions --

static void resetStack();
Value pop();
void push(Value value);

#endif