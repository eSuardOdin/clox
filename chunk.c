#include <stdio.h>
#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "common.h"
#include "value.h"

void initChunk(Chunk* chunk) {
    chunk->byteCount = 0;
    chunk->byteCapacity = 0;
    chunk->lineCount = 0;
    chunk->lineCapacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    freeValueArray(&chunk->constants);
    FREE_ARRAY(uint8_t, chunk->code, chunk->byteCapacity);
    FREE_ARRAY(lineInfo, chunk->lines, chunk->byteCapacity);
    initChunk(chunk);
}

/**
 * @brief Adds a byte whit it's line information
 * in the Chunk
 * 
 * @param chunk The chunk to write in 
 * @param byte The byte to add
 * @param line Number of the line the byte is in
 */
void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    // --- Line processing ---
    // If line already in use, add lenght
    if( chunk->lineCount > 0 && line == (chunk->lines[chunk->lineCount - 1]).line) {
        chunk->lines[chunk->lineCount - 1].lenght++;
    } 
    // Else add a line
    else {
        // Expend array if necessary
        if(chunk->lineCapacity < chunk->lineCount + 1) {
            int oldCapacity = chunk->lineCapacity; 
            chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
            chunk->lines = GROW_ARRAY(lineInfo, chunk->lines, oldCapacity, chunk->lineCapacity);
        }
        lineInfo li;
        li.line = line;
        li.lenght = 1;
        chunk->lines[chunk->lineCount] = li;
        chunk->lineCount++;
    }
    // --- byte processing ---
    if(chunk->byteCapacity < chunk->byteCount +1) {
        int oldCapacity = chunk->byteCapacity;
        chunk->byteCapacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->byteCapacity);
    }

    chunk->code[chunk->byteCount] = byte;
    chunk->byteCount++;
}


/**
 * @brief Get line associated with the byte offset
 * 
 * @param chunk 
 * @param offset 
 * @return int 
 */
int getLine(Chunk *chunk, int offset) {
    int bytesCount = 0;
    
    for(int i = 0; i < chunk->lineCount; i++) {
        
        bytesCount += chunk->lines[i].lenght;
        if(bytesCount > offset) {
            return chunk->lines[i].line;
        }
    }
    return -1;
}


/**
 * @brief Writes a new constant in chunk
 * chooses opcode between OP_CONSTANT and
 * OP_CONSTANT_LONG depending on number
 * of constants already in pool
 * 
 * @param chunk The chunk to append constant to
 * @param v The constant to add
 * @param line Line of the constant
 */
void writeConstant(Chunk* chunk, Value v, int line) {
    int index = addConstant(chunk, v);
    // If can be encoded into 1 byte
    if(index < 0xFF) {
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, index, line);
    } 
    // If can be encoded into 3 bytes
    else if(index < 0xFFFFFF) {
        writeChunk(chunk, OP_CONSTANT_LONG, line);
        writeChunk(chunk, (uint8_t)(index & 0xFF), line);
        writeChunk(chunk, (uint8_t)((index >> 8) & 0xFF), line);
        writeChunk(chunk, (uint8_t)((index >> 16) & 0xFF), line);
    }
    // Else, handle it later ?
    else {
        fprintf(stderr, "Too many constants, cannot encode it.\n");
        exit(EXIT_FAILURE);
    }
}


/**
 * @brief Add a constant to chunk pool
 * 
 * @param chunk 
 * @param v 
 * @return int 
 */
int addConstant(Chunk* chunk, Value v) {
    writeValueArray(&chunk->constants, v);
    return chunk->constants.count - 1;
}