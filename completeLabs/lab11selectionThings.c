#include <stdio.h>
#include <stdlib.h>
#define index(a, b) (a) * (maxCapacity + 1) + (b)

int printChoosedThing(int *matrix, int *weightThings, int *values, int qt, int mc, int maxCapacity) {
    int *choosedThing = malloc((qt + 1) * sizeof(int));
    if (choosedThing == NULL) return -1;
    int index = 0;

    while (matrix[index(qt, mc)] != 0) {
        if (matrix[index(qt, mc)] == matrix[index(qt - 1, mc)]) {
            qt--;
        } else {
            qt--;
            mc -= weightThings[qt];
            choosedThing[index] = qt;
            index++;
        }
    }

    for (int i = index - 1; i >= 0; i--) {
        printf("%d %d\n", weightThings[choosedThing[i]], values[choosedThing[i]]);
    }

    free(choosedThing);
    return 0;
}

int selectionThings(int *weightThings, int *values, int quantThing, int maxCapacity) {
    int *matrix = malloc((quantThing + 1) * (maxCapacity + 1) * sizeof(int));
    if (matrix == NULL) return -1;

    for (int i = 0; i <= quantThing; i++) {
        for (int j = 0; j <= maxCapacity; j++) {
            if (i == 0 || j == 0) {
                matrix[index(i, j)] = 0;
            } else {
                if (weightThings[i - 1] > j) {
                    matrix[index(i, j)] = matrix[index(i - 1, j)];
                } else {
                    int prevThingValue = matrix[index(i - 1, j)];
                    int byFormula = values[i - 1] + matrix[index(i - 1, j - weightThings[i - 1])];
                    matrix[index(i, j)] = prevThingValue > byFormula ? prevThingValue : byFormula;
                }
            }
        }
    }

    printf("%d\n", matrix[index(quantThing, maxCapacity)]);
    int ret = printChoosedThing(matrix, weightThings, values, quantThing, maxCapacity, maxCapacity);

    free(matrix);
    return ret;
}

int main(void) {
    int ret = 0;
    FILE* fileIn = fopen("in.txt", "r");
    if (fileIn == NULL) return -1;
    int quantThing = 0, maxCapacity = 0;
    if (fscanf(fileIn, "%d %d", &quantThing, &maxCapacity) == 0) {
        ret = -1;
        goto SR_MAIN1;
    }
    int *weightThings = malloc(quantThing * sizeof(int));
    int *values = malloc(quantThing * sizeof(int));
    if (values == NULL) {
        ret = -1;
        goto SR_MAIN2;
    }
    for (int i = 0; i < quantThing; i++) {
        if (fscanf(fileIn, "%d %d", &weightThings[i], &values[i]) == 0) {
            ret = -1;
            goto SR_MAIN2;
        }
    }

    ret = selectionThings(weightThings, values, quantThing, maxCapacity);

    SR_MAIN2:
    free(weightThings);
    free(values);
    SR_MAIN1:
    fclose(fileIn);
    return ret;
}
