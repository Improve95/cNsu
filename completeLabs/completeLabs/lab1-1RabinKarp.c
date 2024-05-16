#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STOP_RUNNING(fileIn, fileOut) fclose(fileIn); \
                                      fclose(fileOut);\
                                      return 0

enum Constants {
    VALUE_FOR_HASH_CREATE = 3,
    MAIN_STR_LIST_SIZE = 1024 * 1024 * 2,
};

int createHash(unsigned char* exampleStr, size_t lenExample, int stPosCreate) {
    int hashValue = 0;
    for(size_t i = 0; i < lenExample; ++i) {
        hashValue *= VALUE_FOR_HASH_CREATE;
        hashValue += ((int)exampleStr[stPosCreate + (lenExample - i - 1)]) % VALUE_FOR_HASH_CREATE;
    }
    return hashValue;
}

int changeHash(unsigned char* mainStr, size_t mainStrPos, size_t lenExample, int hashMainStr, int powValue) {
    return (hashMainStr - ((int)mainStr[mainStrPos] % VALUE_FOR_HASH_CREATE)) / VALUE_FOR_HASH_CREATE + (mainStr[mainStrPos + lenExample] % 3) * powValue;
}

int readSymbols(FILE* file, unsigned char* mainStr, size_t quantitySymbolToShift) {
    return (int)fread(mainStr + quantitySymbolToShift, sizeof(unsigned char), MAIN_STR_LIST_SIZE - quantitySymbolToShift, file);
}

int rewriteBuffer(FILE* fileIn, unsigned char* mainStr, size_t* lenMainStr, size_t mainStrPos) {
    size_t quantitySymbolToShift = MAIN_STR_LIST_SIZE - mainStrPos;
    for(size_t i = 0; i < quantitySymbolToShift; ++i) {
        mainStr[i] = mainStr[mainStrPos + i];
    }
    int lenMainStrForCheck = readSymbols(fileIn, mainStr, quantitySymbolToShift);
    if (lenMainStrForCheck == 0) return 0;
    *lenMainStr = lenMainStrForCheck + quantitySymbolToShift;
    return 1;
}

void RabinKarp(FILE* fileIn, FILE* fileOut, unsigned char* mainStr, unsigned char* exampleStr, size_t lenMainStr, size_t lenExample, int hashMainStr, int hashExample, int powValue) {
    size_t shift = 0, posPrintEl = 0, mainStrPos = 0, checkSymbols = 0;
    while (mainStrPos < lenMainStr) {
        if (hashExample == hashMainStr) {
            for(size_t i = 0; i < lenExample; ++i) {
                fprintf(fileOut, "%zu ", posPrintEl + i + 1);
                if (mainStr[mainStrPos + i] == exampleStr[i]) {
                    checkSymbols++;
                } else {
                    shift = i + 1;
                    break;
                }
            }
        } else {
            shift = 1;
        }

        if (mainStrPos + lenExample + shift > lenMainStr) {
            if (rewriteBuffer(fileIn, mainStr, &lenMainStr, mainStrPos) == 0) {
                break;
            }
            mainStrPos = 0;
        }

        for(size_t i = 0; i < shift; ++i) {
            hashMainStr = changeHash(mainStr, mainStrPos + i, lenExample, hashMainStr, powValue);
        }

        posPrintEl += shift;
        mainStrPos += shift;

        if (checkSymbols >= lenExample) {
            mainStrPos += lenExample;
            posPrintEl +=  lenExample;
            hashMainStr = createHash(mainStr, lenExample, mainStrPos);
            checkSymbols = 0;
        }
    }
}

void findExample(FILE* fileIn, FILE* fileOut, unsigned char* exampleStr, size_t lenExample) {
    int hashExample = createHash(exampleStr, lenExample, 0), powValue = pow(3, lenExample - 1);
    fprintf(fileOut, "%d ", hashExample);

    unsigned char* mainStr = malloc(MAIN_STR_LIST_SIZE * sizeof(unsigned char));
    if (mainStr == NULL) return;

    size_t lenMainStr = readSymbols(fileIn, mainStr, 0);
    int hashMainStr = createHash(mainStr, lenExample, 0);

    RabinKarp(fileIn, fileOut, mainStr, exampleStr, lenMainStr, lenExample, hashMainStr, hashExample, powValue);
    free(mainStr);
}

int main() {
    FILE* fileIn = fopen("in.txt", "r");
    if (fileIn == NULL) return 0;

    FILE* fileOut = fopen("out.txt", "w");
    if (fileOut == NULL) {
        fclose(fileIn);
        return 0;
    }

    size_t lenExample = 0, i = 0;
    int ch = 0;
    unsigned char exampleStr[16];

    while ((ch = fgetc(fileIn)) != EOF && ch != '\n') {
        exampleStr[i++] = ch;
        lenExample++;
    }

    findExample(fileIn, fileOut, exampleStr, lenExample);

    STOP_RUNNING(fileIn, fileOut);
}