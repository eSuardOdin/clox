#include "chunk.h"
#include "vm.h"
#include "common.h"
#include "debug.h"

int main(int argc, char **argv)
{
    initVM();

    Chunk chunk;
    initChunk(&chunk);
    
    // // --- 1 * 2 + 3 ---
    // // 1 * 2 
    // writeConstant(&chunk, 1.0, 1);
    // writeConstant(&chunk, 2.0, 1);
    // writeChunk(&chunk, OP_MULTIPLY, 1);
    // // + 3
    // writeConstant(&chunk, 3.0, 1);
    // writeChunk(&chunk, OP_ADD, 1);
    // writeChunk(&chunk, OP_RETURN, 1);



    // // --- 1 + 2 * 3 ---
    // // 2 * 3 
    // writeConstant(&chunk, 2.0, 2);
    // writeConstant(&chunk, 3.0, 2);
    // writeChunk(&chunk, OP_MULTIPLY, 2);
    // // + 1
    // writeConstant(&chunk, 1.0, 2);
    // writeChunk(&chunk, OP_ADD,2);
    // writeChunk(&chunk, OP_RETURN, 2);

    // --- 1 + 2 * 3 - 4 / -5 ---
    writeConstant(&chunk, 2.0, 1);
    //[2]
    writeConstant(&chunk, 3.0, 1);
    // [2][3]
    writeChunk(&chunk, OP_MULTIPLY, 1);
    // [6]

    writeConstant(&chunk, 4.0, 1);
    // [6][4]
    writeConstant(&chunk, 5.0, 1);
    // [6][4][5]
    writeChunk(&chunk, OP_NEGATE, 1);
    // [6][4][-5]
    writeChunk(&chunk, OP_DIVIDE, 1);
    // [6][-0.8]
    writeChunk(&chunk, OP_SUBSTRACT, 1);
    // [6.8]
    writeConstant(&chunk, 1.0, 1);
    // [6.8][1]
    writeChunk(&chunk, OP_ADD, 1);
    // [7.8]
    writeChunk(&chunk, OP_RETURN, 1);
    
    disassembleChunk(&chunk, "main");
    
    interpret(&chunk);
    freeChunk(&chunk);

    freeVM();    
    return 0;
}