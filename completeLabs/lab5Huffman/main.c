#include "headers/head.h"
#include "headers/encodeDecode.h"
#define STOP_RUNNING(fileIn, fileOut, ret) do { fclose(fileIn);   \
                                                fclose(fileOut);  \
                                                return ret; } while(0)

int main(int argc, char **argv) {
    int ret = 0;
    if (argc == 2 && strcmp(argv[1], "help") == 0) {
        printf("haffman.exe, \"mode: c - coding; d - decoding\", \"fileIn\", \"fileOut\"");
        return 0;
    }

    if (argc > 4) return -1;

    if (argc == 4) {
        if ((strcmp(argv[1], "c") != 0) && (strcmp(argv[1], "d") != 0)) {
            printf("incorrect mode\n");
            printf("haffman.exe, \"mode: c - coding; d - decoding\", \"fileIn\", \"fileOut\"");
            return -1;
        }

        FILE* fileIn = fopen(argv[2], "rb");
        if (fileIn == NULL) {
            printf("incorrect file name or the file was not created\n");
            printf("haffman.exe, \"mode: c - coding; d - decoding\", \"fileIn\", \"fileOut\"");
            return -1;
        }
        FILE* fileOut = fopen(argv[3], "wb");
        if (fileOut == NULL) {
            printf("incorrect file name or the file was not created\n");
            printf("haffman.exe, \"mode: c - coding; d - decoding\", \"fileIn\", \"fileOut\"");
            fclose(fileIn);
            return -1;
        }

        if (strcmp(argv[1], "c") == 0) {
            ret = encoding(fileIn, fileOut, 0); // 1 - run from git or ide, 0 - run from console
        } else {
            ret = decoding(fileIn, fileOut);
        }
        STOP_RUNNING(fileIn, fileOut, ret);

    } else {
        FILE* fileIn = fopen("in.txt", "rb");
        if (fileIn == NULL) return -1;
        FILE* fileOut = fopen("out.txt", "wb");
        if (fileOut == NULL) {
            fclose(fileIn);
            return -1;
        }

        unsigned char modeSymbol = getc(fileIn);
        if (modeSymbol == 'c') {
            ret = encoding(fileIn, fileOut, 1); // 1 - run from git or ide, 0 - run from console
        } else if (modeSymbol == 'd') {
            ret = decoding(fileIn, fileOut);
        } else {
            printf("set the operating mode in file or run it via the console\n");
            printf("haffman.exe, \"mode: c - coding; d - decoding\", \"fileIn\", \"fileOut\"");
            STOP_RUNNING(fileIn, fileOut, -1);
        }
        STOP_RUNNING(fileIn, fileOut, ret);
    }
}
