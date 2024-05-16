#include "struct.h"

void swap(Edge **a, Edge **b) {
    Edge *tmp = *a;
    *a = *b;
    *b = tmp;
}

void quickSort(Graph *graph, int startSortPart, int endSortPart) {
    int *weightList = graph->weightList;
    if (startSortPart < endSortPart) {
        int leftPointer = startSortPart + 1, rightPointer = endSortPart;
        int pivot = weightList[startSortPart];

        while (1) {
            while (leftPointer < endSortPart && weightList[leftPointer] < pivot) leftPointer++;
            while (weightList[rightPointer] > pivot) rightPointer--;

            if (leftPointer >= rightPointer) break;
            swap(&graph->edge[leftPointer++], &graph->edge[rightPointer--]);
        }
        swap(&graph->edge[startSortPart], &graph->edge[rightPointer]);

        quickSort(graph, startSortPart, rightPointer - 1);
        quickSort(graph, rightPointer + 1, endSortPart);
    }
}
