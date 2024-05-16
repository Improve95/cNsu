#ifndef STRUCTS
#define STRUCTS

#include "head.h"
typedef struct huffNode {
    struct huffNode *left, *right;
    size_t frequency;
    unsigned char symbol;
} HuffNode;

typedef struct queue {
    struct queue *next;
    HuffNode *node;
} Queue;

typedef struct code {
    char huffCode[256];
    unsigned char symbol;
} Code;

#endif
