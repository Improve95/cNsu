#include "../headers/head.h"
#include "../headers/huffTree.h"

char getQuantLastSymbols(FILE *fileIn, unsigned char *byteCode, unsigned char *readBitCount) {
    unsigned char q = 0;
    for (int i = 0; i < 4; i++) {
        int bit = byteReadingByBit(fileIn, byteCode, readBitCount);
        if (bit) q |= (1 << (3 - i));
    }
    return q;
}

void decodeSymbols(FILE *fileIn, FILE *fileOut, unsigned char *byteCode, unsigned char *readBitCount, HuffNode *huffTree, unsigned char quantLastBits) {
    int isLastByte = 0;
    unsigned char byteCodeBuf = getc(fileIn);
    HuffNode *tmp = NULL;
    while (feof(fileIn) == 0) {
        tmp = huffTree;
        while (tmp->frequency == 0) {
            tmp = (((*byteCode) >> (7 - (*readBitCount))) & 1) ? tmp->right : tmp->left;
            (*readBitCount)++;
            if ((*readBitCount) >= 8) {
                (*byteCode) = byteCodeBuf;
                byteCodeBuf = getc(fileIn);
                if (feof(fileIn) != 0) isLastByte = 1;
                (*readBitCount) = 0;
            }
        }

        // для случая, когда разных символов нет
        if (huffTree->frequency == 1) {
            (*readBitCount)++;
            if ((*readBitCount) >= 8) {
                (*byteCode) = byteCodeBuf;
                byteCodeBuf = getc(fileIn);
                if (feof(fileIn) != 0) isLastByte = 1;
                (*readBitCount) = 0;
            }
        }

        fwrite(&(tmp->symbol), 1, 1, fileOut);
        if (isLastByte) break;
    }

    while ((*readBitCount) < quantLastBits) {
        tmp = huffTree;
        while (tmp->frequency == 0) {
            tmp = (((*byteCode) >> (7 - (*readBitCount))) & 1) ? tmp->right : tmp->left;
            (*readBitCount)++;
        }

        // для случая, когда разных символов нет
        if (huffTree->frequency == 1) {
            (*readBitCount)++;
            if ((*readBitCount) >= 8) {
                (*byteCode) = byteCodeBuf;
                (*readBitCount) = 0;
            }
        }

        fwrite(&(tmp->symbol), 1, 1, fileOut);
    }
}
