#ifndef DECODE_FUNC
#define DECODE_FUNC

#include "head.h"
char getQuantLastSymbols(FILE *fileIn, unsigned char *byteCode, unsigned char *readBitCount);
void decodeSymbols(FILE *fileIn, FILE *fileOut, unsigned char *byteCode, unsigned char *readBitCount, HuffNode *huffTree, unsigned char quantLastBits);

#endif
