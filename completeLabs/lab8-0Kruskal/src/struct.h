#ifndef STRUCT
#define STRUCT

#include "head.h"

typedef struct Edge {
    int start;
    int end;
} Edge;

typedef struct Graph {
    Edge** edge;
    int *weightList;
    int quantNodes;
    int quantEdges;
} Graph;

#endif
