#include "head.h"
#include <limits.h>
#include "struct.h"
#include "quickSort.h"
#define STOP_RUNNING(mes, mem1, mem2, mem3, mem4, file, ret) do { printf("%s", mes); \
                                                                  free(mem1);        \
                                                                  free(mem2);        \
                                                                  free(mem3);        \
                                                                  free(mem4);        \
                                                                  fclose(file);      \
                                                                  return ret; } while(0)
#define FREE_MEM(mem1, mem2, mem3, ret) do { free(mem1); \
                                             free(mem2); \
                                             free(mem3); \
                                             return ret; } while(0)

int dsuGet(int *parents, int vertex) {
    if (vertex == parents[vertex])
        return vertex;
    parents[vertex] = dsuGet(parents, parents[vertex]);
    return parents[vertex];
}

void dsuUnite(int *parents, int *rank, int a, int b, int *add) {
    a = dsuGet(parents, a);
    b = dsuGet(parents, b);
    if (a != b) {
        if (rank[a] < rank[b]) {
            int c = a;
            a = b;
            b = c;
        }

        parents[b] = a;
        if (rank[a] == rank[b]) {
            rank[a]++;
        }

        *add += 1;
    }
}

int kruskal(Graph *graphInfo) {
    for (int i = 0; i < graphInfo->quantEdges - 1; i++) {
        if (graphInfo->weightList[i] > graphInfo->weightList[i + 1]) {
            quickSort(graphInfo, 0, graphInfo->quantEdges - 1);
            break;
        }
    }

    int *parents = malloc(graphInfo->quantNodes * sizeof(int));
    int *rank = calloc(graphInfo->quantNodes, sizeof(int));
    int *finalEdges = malloc(2 * (graphInfo->quantNodes - 1) * sizeof(int));
    if (parents == NULL || finalEdges == NULL || rank == NULL) FREE_MEM(parents, rank, finalEdges, -1);

    int index = 0, add = 0;
    for (int i = 0; i < graphInfo->quantNodes; i++) {
        parents[i] = i;
    }

    for (int i = 0; i < graphInfo->quantEdges; i++) {
        int st = graphInfo->edge[i]->start;
        int en = graphInfo->edge[i]->end;

        if (dsuGet(parents, st - 1) != dsuGet(parents, en - 1)) {
            finalEdges[index] = st;
            finalEdges[index + 1] = en;
            index += 2;

            dsuUnite(parents, rank, st - 1, en - 1, &add);
        }
    }

    if (add < graphInfo->quantNodes - 1) {
        printf("no spanning tree");
        FREE_MEM(parents, rank, finalEdges, 0);
    }

    for (int i = 0; i < index - 1; i += 2) {
        printf("%d %d\n", finalEdges[i], finalEdges[i + 1]);
    }

    FREE_MEM(parents, rank, finalEdges, 0);
}

int main(void) {
    FILE* fileIn = fopen("in.txt", "rb");
    if (fileIn == NULL) return -1;

    Graph *graphInfo = malloc(sizeof(Graph));
    if (graphInfo == NULL) STOP_RUNNING("error", NULL, NULL, NULL, NULL, fileIn, -1);

    if (fscanf(fileIn, "%d", &graphInfo->quantNodes) == -1) STOP_RUNNING("error", NULL, NULL, NULL, graphInfo, fileIn, -1);

    int qn = graphInfo->quantNodes, qe = 0;
    if (qn < 0 || qn > 5000) STOP_RUNNING("bad number of vertices", NULL, NULL, NULL, graphInfo, fileIn, 0);

    if (fscanf(fileIn, "%d", &graphInfo->quantEdges) == -1) STOP_RUNNING("error", NULL, NULL, NULL, graphInfo, fileIn, -1);

    qe = graphInfo->quantEdges;
    if (qe < 0 || qe > qn*(qn+1)/2) STOP_RUNNING("bad number of edges", NULL, NULL, NULL, graphInfo, fileIn, 0);
    if (qn == 0) STOP_RUNNING("no spanning tree", NULL, NULL, NULL, graphInfo, fileIn, 0);

    graphInfo->edge = malloc(graphInfo->quantEdges * sizeof(Edge*));
    if (graphInfo->edge == NULL) STOP_RUNNING("error", NULL, NULL, NULL, graphInfo, fileIn, -1);

    Edge *edgesList = malloc(graphInfo->quantEdges * sizeof(Edge));
    if (edgesList == NULL) STOP_RUNNING("error", NULL, NULL, graphInfo->edge, graphInfo, fileIn, -1);

    graphInfo->weightList = malloc(graphInfo->quantEdges * sizeof(int));
    if (graphInfo->weightList == NULL) STOP_RUNNING("error", NULL, edgesList, graphInfo->edge, graphInfo, fileIn, -1);

    int s = 0, e = 0;
    long long w = 0;
    for (int i = 0; i < graphInfo->quantEdges; i++) {
        if (fscanf(fileIn, "%d %d %lld", &s, &e, &w) == -1) STOP_RUNNING("bad number of lines", graphInfo->weightList, edgesList, graphInfo->edge, graphInfo, fileIn, 0);
        if (s < 1 || s > qn || e < 1 || e > qn) STOP_RUNNING("bad vertex", graphInfo->weightList, edgesList, graphInfo->edge, graphInfo, fileIn, 0);
        if (w < 0 || w > INT_MAX) STOP_RUNNING("bad length", graphInfo->weightList, edgesList, graphInfo->edge, graphInfo, fileIn, 0);

        edgesList[i].start = s;
        edgesList[i].end = e;
        graphInfo->weightList[i] = (int)w;
        graphInfo->edge[i] = &edgesList[i];
    }

    int ret = kruskal(graphInfo);
    if (ret != 0) STOP_RUNNING("error", graphInfo->weightList, edgesList, graphInfo->edge, graphInfo, fileIn, ret);

    STOP_RUNNING("", graphInfo->weightList, edgesList, graphInfo->edge, graphInfo, fileIn, 0);
}
