#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define STOP_RUNNING(mes, mem1, mem2, file, ret) do { printf("%s", mes); \
                                                      free(mem1);        \
                                                      free(mem2);        \
                                                      fclose(file);      \
                                                      return ret; } while(0)
#define FREE_MEM(mem1, mem2, mem3, mem4, ret) do { free(mem1); \
                                                   free(mem2); \
                                                   free(mem3); \
                                                   free(mem4); \
                                                   return ret; } while(0)
//enum Constants {
//    INF = INT_MAX,
//};

typedef struct Graph {
    unsigned int *matrix;
    int quantNodes;
    int quantEdges;
} Graph;

int primFindMST(Graph *graphInfo) {
    int qn = graphInfo->quantNodes;
    char *used = calloc(qn, sizeof(char));
    unsigned int *minDistToMST= malloc(qn * sizeof(unsigned int));
    int *minEdgeEnd = malloc(qn * sizeof(unsigned int));
    int *finalEdges = malloc(qn * 2 * sizeof(int));
    if (used == NULL || minDistToMST == NULL || minEdgeEnd == NULL || finalEdges == NULL) {
        FREE_MEM(used, minDistToMST, minEdgeEnd, finalEdges, -1);
    }
    int index = 0;

    for (int i = 0; i < qn; i++) {
        for (int j = 0; j < qn; j++) {
            if (graphInfo->matrix[i * qn + j] == 0) {
                graphInfo->matrix[i * qn + j] = 2147483648;
            }
        }
        minDistToMST[i] = 2147483648;
        minEdgeEnd[i] = -1;
    }

    minDistToMST[0] = 0;
    for (int i = 0; i < qn; i++) {
        int minVertex = -1;
        for (int j = 0; j < qn; j++) {
            if (used[j] == 0 && (minVertex == -1 || minDistToMST[j] < minDistToMST[minVertex])) {
                minVertex = j;
            }
        }

        if (minDistToMST[minVertex] == 2147483648) {
            printf("no spanning tree");
            FREE_MEM(used, minDistToMST, minEdgeEnd, finalEdges, 0);
        }

        used[minVertex] = 1;
        if (minEdgeEnd[minVertex] != - 1) {
            finalEdges[index++] = minVertex + 1;
            finalEdges[index++] = minEdgeEnd[minVertex] + 1;
        }

        for (int k = 0; k < qn; k++) {
            if (graphInfo->matrix[minVertex * qn + k] < minDistToMST[k]) {
                minDistToMST[k] = graphInfo->matrix[minVertex * qn + k];
                minEdgeEnd[k] = minVertex;
            }
        }
    }

    for (int i = 0; i < (qn - 1) * 2; i += 2) {
        printf("%d %d\n", finalEdges[i], finalEdges[i + 1]);
    }
    FREE_MEM(used, minDistToMST, minEdgeEnd, finalEdges, 0);
}

int main(void) {
    FILE* fileIn = fopen("in.txt", "r");
    if (fileIn == NULL) return -1;

    Graph *graphInfo = malloc(sizeof(Graph));
    if (graphInfo == NULL) STOP_RUNNING("error", NULL, NULL, fileIn, -1);

    if (fscanf(fileIn, "%d", &graphInfo->quantNodes) == -1) STOP_RUNNING("error", NULL, graphInfo, fileIn, -1);
    int qn = graphInfo->quantNodes, qe = 0;
    if (qn < 0 || qn > 5000) STOP_RUNNING("bad number of vertices", NULL, graphInfo, fileIn, 0);

    if (fscanf(fileIn, "%d", &graphInfo->quantEdges) == -1) STOP_RUNNING("error", NULL, graphInfo, fileIn, -1);
    qe = graphInfo->quantEdges;
    if (qe < 0 || qe > qn*(qn+1)/2) STOP_RUNNING("bad number of edges", NULL, graphInfo, fileIn, 0);
    if (qn == 0 || qe < qn - 1) STOP_RUNNING("no spanning tree", NULL, graphInfo, fileIn, 0);

    graphInfo->matrix = calloc(qn * qn, sizeof(unsigned int));
    if (graphInfo->matrix == NULL) STOP_RUNNING("error", NULL, graphInfo, fileIn, -1);

    long long int s = 0, e = 0, w = 0;
    for (int i = 0; i < graphInfo->quantEdges; i++) {
        if (fscanf(fileIn, "%lld %lld %lld", &s, &e, &w) == -1) STOP_RUNNING("bad number of lines", graphInfo->matrix, graphInfo, fileIn, 0);
        if (s < 1 || s > qn || e < 1 || e > qn) STOP_RUNNING("bad vertex", graphInfo->matrix, graphInfo, fileIn, 0);
        if (w < 0 || w > INT_MAX) STOP_RUNNING("bad length", graphInfo->matrix, graphInfo, fileIn, 0);

        graphInfo->matrix[(s - 1) * qn + (e - 1)] = (unsigned int)w;
        graphInfo->matrix[(e - 1) * qn + (s - 1)] = (unsigned int)w;
    }

    int ret = primFindMST(graphInfo);
    if (ret != 0) STOP_RUNNING("error", graphInfo->matrix, graphInfo, fileIn, ret);

    STOP_RUNNING("", graphInfo->matrix, graphInfo, fileIn, 0);
}
