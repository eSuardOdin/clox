#ifndef DEBUG_H
#define DEBUG_H

#include "chunk.h"


void printLineInfos(Chunk* chunk);
void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int offset);
static int simpleInstruction(const char* name, int offset);
static int constantInstruction(const char* name, Chunk* chunk, int offset);
static int constantLongInstruction(const char *name, Chunk *chunk, int offset);
#endif