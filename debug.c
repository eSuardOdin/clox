#include "debug.h"
#include "chunk.h"
#include "value.h"
#include <stdio.h>

/**
 * @brief Disassemble a chunk to show its instructions
 * 
 * @param chunk 
 * @param name 
 */
void disassembleChunk(Chunk *chunk, const char *name) {
    printf("====== %s ======\n", name);
    for(int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}


int disassembleInstruction(Chunk *chunk, int offset) {
    printf("%04d ", offset);    // Print instruction line (offset)
    if(offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1])
    {
        // If same line, add a column
        printf("   | ");
    } else {
        // Else, print the line number
        printf("%4d ", chunk->lines[offset]);
    }
    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }

}


static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}


static int constantInstruction(const char *name, Chunk *chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];         // Get the data after opcode (pointer to data value)
    printf("%-16s %04d '", name, constant);
    printValue(chunk->constants.values[constant]); // Print the value
    printf("'\n");
    return offset + 2;
}