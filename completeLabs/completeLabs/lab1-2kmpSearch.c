#include <stdio.h>

#define MAX_PATTERN_SIZE 17
#define BUFFER_SIZE 40

void createTableMaxLenSuffix(char *pattern, int *prefixesLen, int patternLen) {
    int i = 1, j = 0;
    while (i < patternLen) {
        if (pattern[j] == pattern[i]) {
            prefixesLen[i] = j + 1;
            i++;
            j++;
        } else {
            if (j == 0) {
                prefixesLen[i++] = 0;
            } else {
                j = prefixesLen[j - 1];
            }
        }
    }
    for (int k = 0; k < patternLen; k++) {
        printf("%d ", prefixesLen[k]);
    }
    printf("\n");

}

int updateBuffer(FILE* fileIn, int index, char * text) {
    static int read_count = 2;
    int buf = BUFFER_SIZE / 2;
    if (index <= buf * read_count) {
        return 0;
    }
    read_count++;
    return (int)fread(text + ((read_count - 1) % 2) * buf, 1, buf, fileIn);
}

void kmpSearch(FILE* fileIn, int textLen, int patternLen, char *text, char *pattern, int *prefixesLen) {
    int index = 0, j = 0;
    while (textLen + j >= patternLen) {
        int g = j;
        while (g > 0 && text[index % BUFFER_SIZE] != pattern[g]) {
            g = prefixesLen[g - 1];
        }
        if (text[index % BUFFER_SIZE] == pattern[g]) {
            g++;
        }
        index++;
        textLen += updateBuffer(fileIn, index + patternLen, text) - 1;
        if ((g <= j) && (j != 0) && textLen + g >= patternLen) {
            printf("%d %d ", index - j, j);
        }
        if (g == patternLen) {
            printf("%d %d ", index - patternLen + 1, patternLen);
            j = prefixesLen[patternLen - 1];
        }
        else j = g;
    }
}

int main(void) {
    FILE* fileIn = fopen("in.txt", "r");
    if (fileIn == NULL) return -1;

    char pattern[MAX_PATTERN_SIZE];
    int prefixesLen[MAX_PATTERN_SIZE] = {0};

    int ch = 0, i = 0, patternLen = 0;
    while ((ch = fgetc(fileIn)) != EOF && ch != '\n') {
        pattern[i++] = ch;
        patternLen++;
    }

    createTableMaxLenSuffix(pattern, prefixesLen, patternLen);

    int textLen = 0;
    char text[BUFFER_SIZE];
    if (!feof(fileIn)) {
        textLen = fread(text, sizeof(char), BUFFER_SIZE, fileIn);
    }
    if (patternLen > textLen || patternLen == 0) {
        fclose(fileIn);
        return 0;
    }

    kmpSearch(fileIn, textLen, patternLen, text, pattern, prefixesLen);

    fclose(fileIn);
    return 0;
}
