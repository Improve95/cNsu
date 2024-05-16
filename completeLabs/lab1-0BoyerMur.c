#include <stdio.h>
#include <stdlib.h>

int* createShiftList(unsigned char* exampleStr, int lenExample, int* mainShift) {
    static int shiftList[256] = {0};
    for (int i = 0; i < lenExample - 1; ++i) {
        shiftList[(int)exampleStr[i]] = lenExample - i - 1;
    }

    if (shiftList[(int)exampleStr[lenExample - 1]] == 0) {
        shiftList[(int)exampleStr[lenExample - 1]] = lenExample;
    }

    *mainShift = lenExample;
    return shiftList;
}

int readSymbols(FILE* file, unsigned char* mainStr, int listPart, int listSize) {
    return (int)fread(mainStr + listSize * listPart, sizeof(char), listSize, file);
}

int readAddSymbolsInCircle(FILE* fileIn, unsigned char* mainStr, int bufSize, int mainStrPos, int* partListAdd, int* lenMainStr) {
    int readQuant = *lenMainStr;
    if (*partListAdd == 0) {
        if ((mainStrPos % (2 * bufSize)) >= bufSize) {
            *lenMainStr += readSymbols(fileIn, mainStr, *partListAdd, bufSize);
            *partListAdd = 1;
        }
    }
    else {
        if ((mainStrPos % (2 * bufSize)) < bufSize) {
            *lenMainStr += readSymbols(fileIn, mainStr, *partListAdd, bufSize);
            *partListAdd = 0;
        }
    }
    readQuant = *lenMainStr - readQuant;
    return readQuant;
}

void findStr(FILE* fileIn, FILE* fileOut, unsigned char* exampleStr, int lenExample) {
    int lenMainStr = 0, quantReadingSymbols = 0, mainStrPos = 0;
    int checkFileEnd = 1, partListAdd = 0, mainShift = 0;
    int* shiftList = createShiftList(exampleStr, lenExample, &mainShift);

    const int bufSize = 1024*1024;
    const int listSize = 2 * bufSize;
    unsigned char* mainStr = malloc(listSize);
    if (mainStr == NULL) return;

    lenMainStr = readSymbols(fileIn, mainStr, partListAdd, bufSize);
    quantReadingSymbols = lenMainStr;
    partListAdd = 1;

    while (checkFileEnd) {
        int checkMainStrEnd = 1;
        while (checkMainStrEnd) {
            for (int i = lenExample - 1, checkSymbolsQuantity = 0; i >= 0 && checkSymbolsQuantity < lenExample; --i) {
                if (mainStrPos + i >= lenMainStr) {
                    checkMainStrEnd = 0;
                    break;
                }

                fprintf(fileOut, "%d ", mainStrPos + i + 1);

                int nowElPos = (mainStrPos + i) % (listSize);
                if (mainStr[nowElPos] != exampleStr[i]) {
                    if (i == lenExample - 1) {
                        if (shiftList[(int)mainStr[nowElPos]] != 0) {
                            mainStrPos += shiftList[(int)mainStr[nowElPos]];
                            break;
                        } else {
                            mainStrPos += mainShift;
                            break;
                        }
                    }
                    else {
                        mainStrPos += shiftList[(int)exampleStr[lenExample - 1]];
                        break;
                    }
                } else { 
                    checkSymbolsQuantity++; 
                }

                if (checkSymbolsQuantity >= lenExample) {
                    mainStrPos += mainShift;
                    quantReadingSymbols = readAddSymbolsInCircle(fileIn, mainStr, bufSize, mainStrPos, &partListAdd, &lenMainStr);
                }
            }
            quantReadingSymbols = readAddSymbolsInCircle(fileIn, mainStr, bufSize, mainStrPos, &partListAdd, &lenMainStr);
            if (quantReadingSymbols < bufSize) checkFileEnd = 0;
        }
    }
    free(mainStr);
    return;
}

int main() {
    FILE* fileIn = fopen("in.txt", "r");
    FILE* fileOut = fopen("out.txt", "w");

    unsigned char exampleStr[16];
    int i = 0, lenExample = 0, ch;

    if (fileIn == NULL) {
        fclose(fileIn);
        return 0;
    }

    while ((ch = fgetc(fileIn)) != EOF && ch != '\n') {
        exampleStr[i++] = ch;
        lenExample++;
    }

    findStr(fileIn, fileOut, exampleStr, lenExample);
    fclose(fileIn);
    fclose(fileOut);
    return 0;
}
