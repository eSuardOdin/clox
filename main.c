#include "chunk.h"
#include "common.h"
#include "debug.h"

int main(int argc, char **argv)
{
    Chunk chunk;
    initChunk(&chunk);

    // Manually write the two bytes for a constant instruction

    int constant_a_index = addConstant(&chunk, 1.2);
    int constant_b_index = addConstant(&chunk, 5.5);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant_a_index, 123);

    writeChunk(&chunk, OP_CONSTANT, 124);
    writeChunk(&chunk, constant_b_index, 124);

    // Write the byte for a return instruction
    writeChunk(&chunk, OP_RETURN, 124);


    disassembleChunk(&chunk, "main");
    printLineInfos(&chunk);


    // Chunk to debug constants
    Chunk dbg;
    initChunk(&dbg);
    int line = 0;
    for(int i = 0; i < (0xFF + 3); i++) {
        if(!(i % 5)) {
            line++;
        }
        writeConstant(&dbg, (Value)i, line);
    }
    disassembleChunk(&dbg, "debug");


    // for(int i = 0; i < dbg.constants.count; i++) {
    //     printf("Const: %g | Index: %d\n", dbg.constants.values[i], i);
    // }

    return 0;
}