#include <stdio.h>
#include <stdlib.h>

void arrOut(int* numList, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d ", numList[i]);
    }
}

void swap(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}

void heapify(int n, int* numList, int i) {
    int indexMaxEl = i;
    int indexLeftEL = 2 * i + 1;
    int indexRightEL = 2 * i + 2;

    if (indexLeftEL < n && numList[indexLeftEL] >= numList[indexMaxEl]) indexMaxEl = indexLeftEL;
    if (indexRightEL < n && numList[indexRightEL] >= numList[indexMaxEl]) indexMaxEl = indexRightEL;

    if (indexMaxEl != i) {
        swap(&numList[i], &numList[indexMaxEl]);
        heapify(n, numList, indexMaxEl);
    }
}

void heapSort(int* numList, int n) {
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(n, numList, i);
    }

    for (int i = n - 1; i >= 0; --i) {
        swap(&numList[0], &numList[i]);
        heapify(i, numList, 0);
    }
}

int main() {
    FILE* fileIn = fopen("in.txt", "r");
    if (fileIn == NULL) {
        return 0;
    }

    int n = 0;
    if (fscanf(fileIn, "%d", &n) == 0) {
        fclose(fileIn);
        return 0;
    }

    int* numList = malloc(n * sizeof(int));
    if (numList == NULL) {
        fclose(fileIn);
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        if (fscanf(fileIn, "%d", &numList[i]) == 0) {
            free(numList);
            fclose(fileIn);
            return 0;
        }
    }

    heapSort(numList, n);
    arrOut(numList, n);

    fclose(fileIn);
    free(numList);
    return 0;
}
