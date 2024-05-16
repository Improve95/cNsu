#include "../headers/head.h"
#include "../headers/struct.h"
#include "../headers/huffTree.h"
#include "../headers/encodeFunction.h"
#include "../headers/decodeFunction.h"
#define STOP_RUNNING(mem1, mem2, mem3, ret) do { free(mem1); \
                                                 freeEncodeTree(mem2); \
                                                 free(mem3);     \
                                                 return ret; } while(0)

int encoding(FILE *fileIn, FILE *fileOut, int from) {
    int quantSymbols = 0, ret = 0;
    HuffNode *huffNodeList = createHuffNodeList(fileIn, &quantSymbols, &ret);
    if (ret != 0) return -1;
    qsort(huffNodeList, UCHAR_MAX + 1, sizeof(HuffNode), (int(*) (const void *, const void *)) compare);

    HuffNode *huffTree = createEncodeHuffTree(huffNodeList, quantSymbols, &ret);
    if (ret != 0) {
        free(huffNodeList);
        return -1;
    }

    Code *tableCodes = malloc(quantSymbols * sizeof(Code));
    if (tableCodes == NULL) STOP_RUNNING(NULL, huffTree, huffNodeList, -1);

    if (huffTree != NULL) { // is there at least one character in the source
        if (createCodeTable(tableCodes, huffTree) != 0) STOP_RUNNING(tableCodes, huffTree, huffNodeList, -1);
        writeInFile(fileIn, fileOut, tableCodes, huffTree, quantSymbols, from);
    }
    STOP_RUNNING(tableCodes, huffTree, huffNodeList, 0);
}

int decoding(FILE *fileIn, FILE *fileOut) {
    int ret = 0;
    unsigned char byteCode = 0, readBitCount = 0;
    HuffNode *huffTree = createDecodeHuffTree(fileIn, &byteCode, &readBitCount, &ret);
    if (ret != 0)  {
        freeTree(huffTree);
        return -1;
    }
    if (huffTree == NULL) return 0;
    unsigned char quantLastBits = getQuantLastSymbols(fileIn, &byteCode, &readBitCount);
    decodeSymbols(fileIn, fileOut, &byteCode, &readBitCount, huffTree, quantLastBits);

    freeTree(huffTree);
    return 0;
}
