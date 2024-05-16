#ifndef HUFF_TREE
#define HUFF_TREE

#include "struct.h"
HuffNode *createEncodeHuffTree(HuffNode *huffNodeList, int qs, int *ret);
void freeEncodeTree(HuffNode *huffTree);

HuffNode *createDecodeHuffTree(FILE *fileIn, unsigned char *byteCode, unsigned char *readBitCount, int *ret);
char byteReadingByBit(FILE *fileIn, unsigned char *byteCode, unsigned char *readBitCount);

void freeTree(HuffNode *huffTree);

#endif
