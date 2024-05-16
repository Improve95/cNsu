#ifndef FUNC
#define FUNC

#include "struct.h"
HuffNode *createHuffNodeList(FILE *fileIn, int *quantSymbols, int *ret);
int compare(const HuffNode *a, const HuffNode *b);
int createCodeTable(Code *tableCodes, HuffNode *huffTree);
void writeInFile(FILE *fileIn, FILE *fileOut, Code *tableCodes, HuffNode *huffTree, int quantSymbols, int from);

#endif
