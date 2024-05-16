#include <stdio.h>
#include <stdlib.h>
#define FREE_MEM(mess, mem1, mem2, file1, ret) do { printf("%s", mess); \
                                            freeMem(graphInfo);         \
                                            free(mem1);    \
                                            free(mem2);    \
                                            fclose(file1); \
                                            return ret; } while(0)

enum Constants {
    MAX_NODES = 2000, BITS_IN_CHAR = 8,
    IS_CIRCLE = -2, ERROR = -1,
    WHITE = 0, GRAY = 1, BLACK = 2
};

typedef struct NodeInfo {
    unsigned char *edges;
    char color;
} NodeInfo;

typedef struct GraphInfo {
    NodeInfo *graphNodes;
    int *adderList;
    int quantEdges;
    int quantNodes;
    int adderListIndex;
} GraphInfo;

void freeMem(GraphInfo *graphInfo) {
    if (graphInfo != NULL && graphInfo->graphNodes != NULL) {
        for (int i = graphInfo->quantNodes - 1; i > -1; i--)
            free((&graphInfo->graphNodes[i])->edges);
    }
}

int dfsVisit(GraphInfo *graphInfo, NodeInfo *nodeInfo) {
    int ret = 0;
    nodeInfo->color = GRAY;

    int NodeIndex = 0;
    while  (NodeIndex <= graphInfo->quantNodes) {
        while ((nodeInfo->edges[NodeIndex / BITS_IN_CHAR] & (1 << (7 - (NodeIndex % BITS_IN_CHAR)))) == 0 && NodeIndex <= graphInfo->quantNodes)
            NodeIndex++;
        if (NodeIndex > graphInfo->quantNodes) break;

        if (graphInfo->graphNodes[NodeIndex - 1].color == WHITE) {
            ret = dfsVisit(graphInfo, &graphInfo->graphNodes[NodeIndex - 1]);
            if (ret != 0) return ret;
        } else if (graphInfo->graphNodes[NodeIndex - 1].color == GRAY) {
            return IS_CIRCLE;
        }
        NodeIndex++;
    }
    nodeInfo->color = BLACK;

    int index = 0;
    while (&graphInfo->graphNodes[index] != nodeInfo) index++;
    graphInfo->adderList[graphInfo->adderListIndex] = index + 1;
    graphInfo->adderListIndex++;

    return ret;
}

int dfs(GraphInfo *graphInfo) {
    graphInfo->adderList = malloc(graphInfo->quantNodes * sizeof(int));
    if (graphInfo->adderList == NULL) return -1;
    graphInfo->adderListIndex = 0;
    int ret = 0;
    for (int i = 0; i < graphInfo->quantNodes; i++) {
        if (graphInfo->graphNodes[i].color == WHITE) {
            ret = dfsVisit(graphInfo, &graphInfo->graphNodes[i]);
            if (ret != 0) return ret;
        }
    }
    return ret;
}

int topSort(GraphInfo *graphInfo) {
    int ret = dfs(graphInfo);
    if (ret == IS_CIRCLE) {
        printf("%s", "impossible to sort");
    } else if (ret == ERROR) {
        free(graphInfo->adderList);
        return ERROR;
    } else {
        for (int i = graphInfo->adderListIndex - 1; i > -1 ; i--)
            printf("%d ", graphInfo->adderList[i]);
    }
    free(graphInfo->adderList);
    return 0;
}

int main(void) {
    FILE* fileIn = fopen("in.txt", "rb");
    if (fileIn == NULL) return -1;

    GraphInfo *graphInfo = malloc(sizeof(GraphInfo));
    if (graphInfo == NULL) FREE_MEM("Error", NULL, NULL, fileIn, -1);
    graphInfo->graphNodes = NULL;

    if (fscanf(fileIn,"%d", &graphInfo->quantNodes) == -1) FREE_MEM("bad number of lines", NULL, graphInfo, fileIn, 0);
    if (graphInfo->quantNodes > 2000 || graphInfo->quantNodes < 0) FREE_MEM("bad number of vertices", NULL, graphInfo, fileIn, 0);

    if (fscanf(fileIn, "%d", &graphInfo->quantEdges) == -1) FREE_MEM("bad number of lines", NULL, graphInfo, fileIn, 0);
    int qe = graphInfo->quantEdges, qn = graphInfo->quantNodes;
    if (qe < 0 || qe > (qn * (qn + 1) / 2)) FREE_MEM("bad number of edges", NULL, graphInfo, fileIn, 0);

    graphInfo->graphNodes = malloc(graphInfo->quantNodes * sizeof(NodeInfo));
    if (graphInfo->graphNodes == NULL) FREE_MEM("Error", graphInfo, NULL, fileIn, -1);
    for (int i = 0; i < graphInfo->quantNodes; i++) {
        (&graphInfo->graphNodes[i])->edges = calloc(MAX_NODES / BITS_IN_CHAR + 1, sizeof(char));
        if ((&graphInfo->graphNodes[i])->edges == NULL) FREE_MEM("Error", graphInfo->graphNodes, graphInfo, fileIn, -1);
        (&graphInfo->graphNodes[i])->color = WHITE;
    }

    int m = 0, n = 0;
    for (int i = 0; i < graphInfo->quantEdges; i++) {
        if (fscanf(fileIn, "%d %d", &m, &n) == -1) FREE_MEM("bad number of lines", graphInfo->graphNodes, graphInfo, fileIn, 0);
        if (m > qn || m < 0 || n > qn || n < 0) FREE_MEM("bad vertex", graphInfo->graphNodes, graphInfo, fileIn, 0);
        (&graphInfo->graphNodes[m - 1])->edges[n / BITS_IN_CHAR] |= (1 << (7 - (n % BITS_IN_CHAR)));
    }

    if (topSort(graphInfo) != 0) FREE_MEM("error", graphInfo->graphNodes, graphInfo, fileIn, -1);

    FREE_MEM("", graphInfo->graphNodes, graphInfo, fileIn, 0);
}
