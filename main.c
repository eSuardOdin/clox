#include "chunk.h"
#include "vm.h"
#include "common.h"
#include "debug.h"

int main(int argc, char **argv)
{
    initVM();

    Chunk chunk;
    initChunk(&chunk);
    writeConstant(&chunk, 5.5, 123);
    writeConstant(&chunk, 1.23, 124);
    // Write the byte for a return instruction
    writeChunk(&chunk, OP_RETURN, 124);


    disassembleChunk(&chunk, "main");
    freeVM();    
    freeChunk(&chunk);

    // // Chunk to debug constants
    // Chunk dbg;
    // initChunk(&dbg);
    // int line = 0;
    // for(int i = 0; i < (0xFF + 3); i++) {
    //     if(!(i % 5)) {
    //         line++;
    //     }
    //     writeConstant(&dbg, (Value)i, line);
    // }
    // disassembleChunk(&dbg, "debug");
    

    return 0;
}