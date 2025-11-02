#ifndef VM_H
#define VM_H

#include "chunk.h"

typedef struct {
    // Chunk the VM is running
    Chunk* chunk;
    // Instruction Pointer (Program Counter)
    uint8_t* ip;
} Vm;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);
static InterpretResult run();



#endif