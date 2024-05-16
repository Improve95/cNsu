#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int checkStr(char* str) {
    int sym[10];
    int strLen = (int)(strlen(str));

    for (int i = 0; i < 10; ++i) { 
        sym[i] = 0;
    }

    for (int i = 0; i < strLen; ++i) {
        int intFromSys = str[i] - '0';
        if (intFromSys > 9 || intFromSys < 0) return 0;
        if (sym[intFromSys] >= 1) {
            return 0;
        }
        sym[intFromSys]++;
    }
    return 1;
}

char* swap(char* str, int r, int l) {
    char c = str[r];
    str[r] = str[l];
    str[l] = c;


    int in = r + 1, strLen = (int)(strlen(str));
    for (int i = in, in = 0; i < (r + strLen - 1) / 2 + 1; ++i, ++in) {
        c = str[i];
        str[i] = str[strLen - in - 1];
        str[strLen - in - 1] = c;
    }

    return str;
}

int createBaseSwap(char* str) {
    int lenStr = (int)(strlen(str)), moreRIn = -1, moreLIn = -1, minEl = 0;

    for (int i = lenStr - 1; i >= 0; --i) {
        if (str[i] < str[i + 1]) {
            moreRIn = i;
            minEl = str[i];
            break;
        }
    }

    if (moreRIn == -1) {
        return 0;
    }

    for (int i = lenStr - 1; i >= 0; --i) {
        if (str[i] > minEl) {
            moreLIn = i;
            break;
        }
    }
    swap(str, moreRIn, moreLIn);

    return 1;
}

int generateAllSwap(char* str, int n) {
    for (int i = 0; i < n; ++i) {
        if (createBaseSwap(str) == 0) return 0;
        printf("%s\n", str);
    }

    return 0;
}

int main() {
    FILE* file = fopen("in.txt", "rb");
    if (file == NULL) return 0;

    char str[31];
    int n = 0;

    if (!fgets(str, 31, file)) {
        fclose(file);
        return 0;
    }
    
    str[strlen(str) - 1] = '\0';
    if (fscanf(file, "%d", &n) == 0) { 
        fclose(file);
        return 0; 
    }

    if (checkStr(str) == 0) {
        printf("bad input");
        fclose(file);
        return 0;
    }

    if (generateAllSwap(str, n) == 0) { 
        fclose(file); 
        return 0; 
    }

    fclose(file);
    return 0;
}
