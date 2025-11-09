#include "chunk.h"
#include "common.h"
#include "value.h"
#include "memory.h"

void initValueArray(ValueArray *va) {
    va->capacity = 0;
    va->count = 0;
    va->values = NULL;
}

/**
 * @brief Write a new value and expend array if
 * necessary
 * 
 * @param va 
 * @param v 
 */
void writeValueArray(ValueArray* va, Value v) {
    if(va->capacity < va->count + 1) {
        int oldCapacity = va->capacity;
        va->capacity = GROW_CAPACITY(oldCapacity);
        va->values = GROW_ARRAY(Value, va->values, oldCapacity, va->capacity);
    }

    va->values[va->count] = v;
    va->count++;
}

/**
 * @brief Free array and reinit it
 * 
 * @param va 
 */
void freeValueArray(ValueArray *va) {
    FREE_ARRAY(Value, va->values, va->capacity);
    initValueArray(va);
}


void printValue(Value value) {
    switch (value.type) {
        case VAL_NUM:
            printf("%g", AS_NUMBER(value));    // Double shortest value
            break;
        case VAL_BOOL:
            printf("%s", value.as.boolean ? "true" : "false");
            break;
        case VAL_NIL:
            printf("NIL");
            break;
    }
}

bool valuesEqual(Value a, Value b) {
    if (a.type != b.type) return false;
    switch (a.type) {
        case VAL_BOOL:  return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NUM:   return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_NIL:   return true;
        default:        return false;
    }
}


