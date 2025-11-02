#include "chunk.h"
#include "common.h"
#include "debug.h"

int main(int argc, char **argv)
{
    Chunk chunk;
    initChunk(&chunk);

    // Manually write the two bytes for a constant instruction
    int constantA = addConstant(&chunk, 1.2);
    int constantB = addConstant(&chunk, 5.5);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constantA, 123);

    writeChunk(&chunk, OP_CONSTANT, 124);
    writeChunk(&chunk, constantB, 124);

    // Write the byte for a return instruction
    writeChunk(&chunk, OP_RETURN, 124);


    disassembleChunk(&chunk, "main");
    printLineInfos(&chunk);
    return 0;
}