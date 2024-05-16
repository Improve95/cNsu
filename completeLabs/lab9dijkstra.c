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

typedef struct Graph {
    int *matrix;
    int beginPath, endPath;
    int quantNodes;
    int quantEdges;
} Graph;

int printPath(Graph *graphInfo, size_t *minDistToNode, size_t *minPathToNodes, int *quantPathMoreInt) {
    for (int i = 0; i < graphInfo->quantNodes; i++) {
        if (minDistToNode[i] == ULLONG_MAX) {
            printf("oo ");
        } else if (minDistToNode[i] > INT_MAX) {
            printf("INT_MAX+ ");
        } else {
            printf("%d ", (int)minDistToNode[i]);
        }
    }
    printf("\n");
    if (graphInfo->beginPath == graphInfo->endPath) {
        printf("%d", graphInfo->beginPath);
    } else {
        int *finalPath = malloc(graphInfo->quantNodes * sizeof(int));
        if (finalPath == NULL) return -1;
        int index = 0;
        for (int v = graphInfo->endPath - 1; v != graphInfo->beginPath - 1; v = minPathToNodes[v], index++) {
            finalPath[index] = v;
            if (minDistToNode[v] == ULLONG_MAX) {
                printf("no path");
                free(finalPath);
                return 0;
            } else if (quantPathMoreInt[v] > 1) {
                printf("overflow");
                free(finalPath);
                return 0;
            }
        }

        for (int j = 0; j < index; j++) {
            printf("%d ", finalPath[j] + 1);
        }
        printf("%d ", graphInfo->beginPath);
        free(finalPath);
    }
    return 0;
}

void arrayInit(Graph *graphInfo, size_t *minDistToNode) {
    int qn = graphInfo->quantNodes;
    for (int i = 0; i < qn; i++) {
        for (int j = 0; j < qn; j++) {
            if (graphInfo->matrix[i * qn + j] == 0) {
                graphInfo->matrix[i * qn + j] = -1;
            }
        }
        minDistToNode[i] = ULLONG_MAX;
    }
}

int dijkstraFindPath(Graph *graphInfo) {
    int qn = graphInfo->quantNodes;
    char *used = calloc(qn, sizeof(char));
    int *quantPathMoreInt = calloc(qn, sizeof(int));
    size_t *minDistToNode = malloc(qn * sizeof(size_t));
    size_t *minPathToNodes = calloc(qn, sizeof(size_t));
    if (used == NULL || minDistToNode == NULL || minPathToNodes == NULL || quantPathMoreInt == NULL) {
        FREE_MEM(used, minDistToNode, minPathToNodes, quantPathMoreInt, -1);
    }
    arrayInit(graphInfo, minDistToNode);

    minDistToNode[graphInfo->beginPath - 1] = 0;
    for (int i = 0; i < qn; i++) {
        int vertex = -1;
        for (int j = 0; j < qn; j++) {
            if (used[j] == 0 && (vertex == -1 || minDistToNode[j] < minDistToNode[vertex])) {
                vertex = j;
            }
        }

        if (minDistToNode[vertex] == ULLONG_MAX) {
            continue;
        }

        used[vertex] = 1;
        for (int j = 0; j < qn; j++) {
            size_t len = minDistToNode[vertex] + graphInfo->matrix[vertex * qn + j];
            if (len > INT_MAX && minDistToNode[j] != ULLONG_MAX) quantPathMoreInt[j]++;
            if (graphInfo->matrix[vertex * qn + j] != -1 && minDistToNode[j] > len) {
                minDistToNode[j] = len;
                minPathToNodes[j] = vertex;
            }
        }
    }

    int ret = printPath(graphInfo, minDistToNode, minPathToNodes, quantPathMoreInt);
    FREE_MEM(used, minDistToNode, minPathToNodes, quantPathMoreInt, ret);
}

int readValue(FILE *fileIn, Graph *graphInfo) {
    if (fscanf(fileIn, "%d", &graphInfo->quantNodes) == -1) {
        STOP_RUNNING("error", NULL, graphInfo, fileIn, -1);
    }
    int qn = graphInfo->quantNodes, qe = 0;
    if (qn < 0 || qn > 5000) {
        STOP_RUNNING("bad number of vertices", NULL, graphInfo, fileIn, -2);
    }
    if (qn == 0) STOP_RUNNING("bad vertex", NULL, graphInfo, fileIn, -2);
    if (fscanf(fileIn, "%d %d", &graphInfo->beginPath, &graphInfo->endPath) == -1) {
        STOP_RUNNING("error", NULL, graphInfo, fileIn, -1);
    }
    if (fscanf(fileIn, "%d", &graphInfo->quantEdges) == -1) {
        STOP_RUNNING("error", NULL, graphInfo, fileIn, -1);
    }
    qe = graphInfo->quantEdges;
    if (qe < 0 || qe > qn*(qn+1)/2) STOP_RUNNING("bad number of edges", NULL, graphInfo, fileIn, -2);
    graphInfo->matrix = calloc(qn * qn, sizeof(int));
    if (graphInfo->matrix == NULL) STOP_RUNNING("error", NULL, graphInfo, fileIn, -1);
    return 0;
}

int main(void) {
    FILE* fileIn = fopen("in.txt", "r");
    if (fileIn == NULL) return -1;

    Graph *graphInfo = malloc(sizeof(Graph));
    if (graphInfo == NULL) STOP_RUNNING("error", NULL, NULL, fileIn, -1);
    int ret = readValue(fileIn, graphInfo);
    if (ret == -1) {
        return -1;
    } else if (ret == -2) {
        return 0;
    }

    long long int s = 0, e = 0, w = 0;
    int qn = graphInfo->quantNodes;
    for (int i = 0; i < graphInfo->quantEdges; i++) {
        if (fscanf(fileIn, "%lld %lld %lld", &s, &e, &w) == -1) {
            STOP_RUNNING("bad number of lines", graphInfo->matrix, graphInfo, fileIn, 0);
        }
        if (s < 1 || s > qn || e < 1 || e > qn) {
            STOP_RUNNING("bad vertex", graphInfo->matrix, graphInfo, fileIn, 0);
        }
        if (w < 0 || w > INT_MAX) STOP_RUNNING("bad length", graphInfo->matrix, graphInfo, fileIn, 0);

        graphInfo->matrix[(s - 1) * qn + (e - 1)] = (int)w;
        graphInfo->matrix[(e - 1) * qn + (s - 1)] = (int)w;
    }

    ret = dijkstraFindPath(graphInfo);
    STOP_RUNNING("", graphInfo->matrix, graphInfo, fileIn, ret);
}
