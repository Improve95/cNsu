#include "../headers/struct.h"

HuffNode *createHuffNodeList(FILE *fileIn, int *quantSymbols, int *ret) {
    HuffNode *huffNodeList = malloc((UCHAR_MAX + 1) * sizeof(HuffNode));
    if (huffNodeList == NULL) {
        *ret = -1;
        return NULL;
    }

    for (int i = 0; i < UCHAR_MAX + 1; i++) {
        huffNodeList[i].left = NULL;
        huffNodeList[i].right = NULL;
        huffNodeList[i].frequency = 0;
        huffNodeList[i].symbol = i;
    }

    unsigned char symbol = getc(fileIn);
    while (feof(fileIn) == 0) {
        if (huffNodeList[symbol].frequency == 0) (*quantSymbols)++;
        huffNodeList[symbol].frequency++;
        symbol = getc(fileIn);
    }
    return huffNodeList;
}

int compare(const HuffNode *a, const HuffNode *b) {
    return b->frequency - a->frequency;
}

void checkByteFullness(FILE *fileOut, unsigned char *byteCode, unsigned char *wroteBitCount) {
    (*wroteBitCount)++;
    if (*wroteBitCount >= 8) {
        fwrite(byteCode, 1, 1, fileOut);
        (*byteCode) = 0;
        (*wroteBitCount) = 0;
    }
}

void calcBitQuant(HuffNode *huffTree, size_t height, size_t *codeSize) {
    if (!huffTree->left) {
        (*codeSize) += huffTree->frequency * height;
    } else {
        height++;
        calcBitQuant(huffTree->left, height, codeSize);
        calcBitQuant(huffTree->right, height, codeSize);
    }
}

void quantLastBits(FILE *fileOut, HuffNode *huffTree, int quantSymbols, size_t quantBitsInAlphabet, unsigned char *byteCode, unsigned char *wroteBitCount) {
    size_t codeSize = 0;
    if (quantSymbols == 1) {
        codeSize = 1 * huffTree->frequency;
    } else {
        calcBitQuant(huffTree, 0, &codeSize);
    }

    unsigned char a = (char)((codeSize + quantBitsInAlphabet + 4) % 8);
    if (a == 0) a = 8;

    for (int i = 0; i < 4; i++) {
        char bit = (a >> (3 - i)) & 1;
        (*byteCode) |= (bit << (7 - *wroteBitCount));
        checkByteFullness(fileOut, byteCode, wroteBitCount);
    }
}

void writeInFileAlphabet(FILE *fileOut, HuffNode *huffTree, unsigned char *byteCode, unsigned char *wroteBitCount, size_t *quantBitsInAlphabet) {
    if (huffTree->left) {
        checkByteFullness(fileOut, byteCode, wroteBitCount);
        (*quantBitsInAlphabet)++;
        writeInFileAlphabet(fileOut, huffTree->left, byteCode, wroteBitCount, quantBitsInAlphabet);
        writeInFileAlphabet(fileOut, huffTree->right, byteCode, wroteBitCount, quantBitsInAlphabet);
    } else {
        (*byteCode) |= (1 << (7 - *wroteBitCount));
        checkByteFullness(fileOut, byteCode, wroteBitCount);
        (*quantBitsInAlphabet)++;

        if (*wroteBitCount > 0) {
            for (int i = 0; i < 8; i++) {
                if ((huffTree->symbol >> (7 - i)) & 1) (*byteCode) |= (1 << (7 - *wroteBitCount));
                checkByteFullness(fileOut, byteCode, wroteBitCount);
            }
            (*quantBitsInAlphabet) += 8;
        } else {
            fwrite(&huffTree->symbol, 1, 1, fileOut);
            (*quantBitsInAlphabet) += 8;
        }
    }
}

int dfsHuffTree(Code *tableCodes, HuffNode *huffTree, char *huffCode, int *tableIndex, char *lenghtCode) {
    if (!huffTree->left) {
        tableCodes[*tableIndex].symbol = huffTree->symbol;

        for (int i = 0; i < *lenghtCode; i++)
            tableCodes[*tableIndex].huffCode[i] = huffCode[i];

        (*tableIndex)++;
    } else {
        (*lenghtCode)++;
        huffCode[(*lenghtCode) - 1] = '0';
        if (dfsHuffTree(tableCodes,huffTree->left, huffCode, tableIndex, lenghtCode) != 0) return -1;

        huffCode[*lenghtCode - 1] = '1';
        if (dfsHuffTree(tableCodes,huffTree->right, huffCode, tableIndex, lenghtCode) != 0) return -1;
        (*lenghtCode)--;
    }
    return 0;
}

int createCodeTable(Code *tableCodes, HuffNode *huffTree) {
    char *huffCode = calloc(256, 1);
    if (huffCode == NULL) return -1;

    int tableIndex = 0;
    char lenghtCode = 0;
    if (huffTree->left) {
        if (dfsHuffTree(tableCodes, huffTree, huffCode, &tableIndex, &lenghtCode) != 0) return -1;
    } else {
        tableCodes[0].symbol = huffTree->symbol;
        tableCodes[0].huffCode[0] = '0';
    }
    free(huffCode);
    return 0;
}

void writeInFile(FILE *fileIn, FILE *fileOut, Code *tableCodes, HuffNode *huffTree, int quantSymbols, int from) {
    unsigned char byteCode = 0, wroteBitCount = 0;
    size_t quantBitsInAlphabet = 0;
    writeInFileAlphabet(fileOut, huffTree, &byteCode, &wroteBitCount, &quantBitsInAlphabet);
    quantLastBits(fileOut, huffTree, quantSymbols, quantBitsInAlphabet, &byteCode, &wroteBitCount);

    rewind(fileIn);
    if (from) fseek(fileIn, 1, SEEK_SET);
    unsigned char symbol = getc(fileIn);
    while (feof(fileIn) == 0) {
        int i = 0;
        while (tableCodes[i].symbol != symbol) i++;
        for (int j = 0; tableCodes[i].huffCode[j] == '1' || tableCodes[i].huffCode[j] == '0'; j++) {
            if (tableCodes[i].huffCode[j] == '1') byteCode |= (1 << (7 - wroteBitCount));
            checkByteFullness(fileOut, &byteCode, &wroteBitCount);
        }
        symbol = getc(fileIn);
    }
    if (wroteBitCount > 0)
        fwrite(&byteCode, 1, 1, fileOut);
}
