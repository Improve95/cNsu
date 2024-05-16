#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

int checkNumSys(int b1, int b2) {
    if (b1 > 16 || b1 < 2 || b2 > 16 || b2 < 2) {
        printf("bad input");
        return 1;
    }
    return 0;
}

int checkFirstNumSys(int b1, char *num) {
    static const char symbols[17] = { '.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int lenNum = (int)strlen(num), doteCounter = 0;
    for (int i = 0, check = 0; i < lenNum; i++) {
        check = 0;
        for (int j = 0; j < b1 + 1; j++) {
            if (tolower(num[i]) == symbols[j]) {
                check = 1;
                break;
            }
        }
        if (check == 0) {
            printf("bad input");
            return 1;
        }
        if (num[i] == '.') doteCounter++;
    }
    if (doteCounter > 1) {
        printf("bad input");
        return 1;
    }
    return 0;
}

long long int riseToDegree(int number, int degre) {
    long long int res = 1;
    for (int i = 0; i < degre; i++) {
        res *= number;
    }
    return res;
}

float getNumberFromLetter(char letter) {
    switch (tolower(letter)) {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'a': return 10;
    case 'b': return 11;
    case 'c': return 12;
    case 'd': return 13;
    case 'e': return 14;
    case 'f': return 15;
    default: break;
    }
    return 0;
}

char getLetterFromNumber(int num) {
    switch (num) {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    case 10: return 'a';
    case 11: return 'b';
    case 12: return 'c';
    case 13: return 'd';
    case 14: return 'e';
    case 15: return 'f';
    default: break;
    }
    return 0;
}

void from10ToAnotherIntP(long long int num, int toSys) {
    char resIntPart[100];

    int intPartLen = 0;
    for (int i = 0; num > 0; i++, intPartLen++) {
        resIntPart[i] = getLetterFromNumber(num % toSys);
        num /= toSys;
    }

    if (intPartLen == 0) printf("0");

    for (int i = intPartLen - 1; i > -1; i--) {
        printf("%c", resIntPart[i]);
    }
}

void from10ToAnotherFractalP(double fractalPart, int toSys) {
    char resFractalPart[100];
    
    double fractalPartAdd = fractalPart, intPartFractal;
    int fractalPartLen = 0;
    for (int i = 0; i < 12; i++, fractalPartLen++) {
        fractalPartAdd = fractalPart * toSys;
        resFractalPart[i] = getLetterFromNumber((int)fractalPartAdd);
        fractalPart = modf(fractalPartAdd, &intPartFractal);
    }

    printf(".");

    for (int i = 0; i < fractalPartLen; i++) {
        printf("%c", resFractalPart[i]);
    }
}

int fromB1toTen(int b1, int b2, char num[14]) {
    char wholePart[13], fractPart[13];
    int i, lenWholePart = 0, lenFractPart = 0, lenNum = (int)strlen(num), check = 0;
    for (i = 0; i < lenNum; i++) {
        if (num[i] != '.') {
            wholePart[i] = num[i];
            lenWholePart++;
        }
        else {
            check = 1;
            break;
        }
    }
    if (lenWholePart == 0) {
        printf("bad input");
        return 1;
    }

    for (int j = i + 1; j < lenNum; j++) {
        fractPart[j - i - 1] = num[j];
        lenFractPart++;
    }
    if (lenFractPart == 0 && check) {
        printf("bad input");
        return 1;
    }

    long long int sys10NumWhole = 0;
    int counter = lenWholePart - 1;
    double sys10NumFractal = 0;
    for (int i = 0; i < lenWholePart; i++, counter--) {
        sys10NumWhole += (int)getNumberFromLetter(wholePart[i]) * riseToDegree(b1, counter);
    }

    counter = 1;
    for (int i = 0; i < lenFractPart; i++, counter++) {
        sys10NumFractal += getNumberFromLetter(fractPart[i]) / riseToDegree(b1, counter);
    }

    if (b2 == 10) {
        printf("%f", sys10NumWhole + sys10NumFractal);
    } else if (sys10NumWhole + sys10NumFractal == 0) {
        printf("%d", 0);
    } else {
        from10ToAnotherIntP(sys10NumWhole, b2);
        if (sys10NumFractal != 0) from10ToAnotherFractalP(sys10NumFractal, b2);
    }
    return 0;
}

int main() {
    int b1, b2;
    char num[15];

    if (scanf("%d %d", &b1, &b2) == 0) return 0;
    if (scanf("%14s", num) == 0) return 0;

    if (checkNumSys(b1, b2)) return 0;
    if (checkFirstNumSys(b1, num)) return 0;

    if (fromB1toTen(b1, b2, num)) return 0;

    return 0;
}
