#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,        // 2 bytes : 1 byte opcode & 1 byte Value  
    OP_CONSTANT_LONG,   // 4 bytes : 1 byte opcode & 3 byte Value
    OP_NEGATE,
    OP_ADD,
    OP_SUBSTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_RETURN,          // 1 byte  : 1 byte opcode
} OpCode;

typedef struct {
    int line;
    int lenght;
}lineInfo;

typedef struct {
    int byteCount;
    int byteCapacity;
    int lineCount;
    int lineCapacity;
    uint8_t* code;
    lineInfo* lines;
    ValueArray constants;
} Chunk;


void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void writeConstant(Chunk* chunk, Value v, int line);
int addConstant(Chunk* chunk, Value v);
int getLine(Chunk* chunk, int offset);

#endif