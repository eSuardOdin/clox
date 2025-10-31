#ifndef VALUE_H
#define VALUE_H

#include "common.h"

typedef double Value;

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;


void initValueArray(ValueArray *va);
void writeValueArray(ValueArray *va, Value v);
void freeValueArray(ValueArray *va);
void printValue(Value value);

#endif