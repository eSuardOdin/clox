#ifndef VALUE_H
#define VALUE_H

#include "common.h"

typedef enum {
    VAL_NIL,
    VAL_BOOL,
    VAL_NUM,
} ValueType;


typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} Value;


#define IS_BOOL(value)          ((value).type == VAL_BOOL)
#define IS_NIL(value)           ((value).type == VAL_NIL)
#define IS_NUMBER(value)        ((value).type == VAL_NUM)

#define AS_BOOL(value)          ((value).as.boolean)
#define AS_NUMBER(value)        ((value).as.number)

#define BOOL_VAL(value)         ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL                 ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value)       ((Value){VAL_NUM, {.number = value}})


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