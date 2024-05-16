#include <stdio.h>
#include <stdlib.h>
#define STOP_RUNNING(fileIn, numList) fclose(fileIn); \
                                      free(numList);  \
                                      return 0

void arrOut(int* numList, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d ", numList[i]);
    }
}

void swap(int* a, int* b) {
    int c = *a;
    *a = *b;
    *b = c;
}

void quickSort(int* numList, int startSortPart, int endSortPart) {
    if (startSortPart < endSortPart) {
        int leftPointer = startSortPart + 1, rightPointer = endSortPart;
        int pivot = numList[startSortPart];

        while (1) {
            while (leftPointer < endSortPart && numList[leftPointer] < pivot) leftPointer++;
            while (numList[rightPointer] > pivot) rightPointer--;

            if (leftPointer >= rightPointer) break;
            swap(&numList[leftPointer++], &numList[rightPointer--]);
        }
        swap(&numList[startSortPart], &numList[rightPointer]);

        quickSort(numList, startSortPart, rightPointer - 1);
        quickSort(numList, rightPointer + 1, endSortPart);
    }
}

int main() {
    FILE* fileIn = fopen("in.txt", "r");
    if (fileIn == NULL) return 0;

    int n = 0;
    if (fscanf(fileIn, "%d", &n) == 0) {
        fclose(fileIn);
        return 0;
    }

    int* numList = malloc(n * sizeof(int));
    if (numList == NULL) { 
        STOP_RUNNING(fileIn, numList); 
    }

    for (int i = 0; i < n; ++i) {
        if (fscanf(fileIn, "%d", &numList[i]) == 0) { 
            STOP_RUNNING(fileIn, numList); 
        }
    }

    quickSort(numList, 0, n-1);
    arrOut(numList, n);

    STOP_RUNNING(fileIn, numList);
}