#include "../headers/head.h"
#include "../headers/struct.h"

void freeEncodeTree(HuffNode *huffTree) {
    if (huffTree) {
        freeEncodeTree(huffTree->left);
        freeEncodeTree(huffTree->right);
        if (huffTree->left != NULL) {
            free(huffTree);
        }
    }
}

void freeTree(HuffNode *huffTree) {
    if (huffTree) {
        freeTree(huffTree->left);
        freeTree(huffTree->right);
        free(huffTree);
    }
}

void freeQueue(Queue **pq) {
    while(pq!= NULL) {
        Queue *tmp = *pq;
        *pq = tmp->next;
        freeEncodeTree(tmp->node);
        free(tmp);
    }
}

// encoding tree
size_t countFreqForNode(HuffNode *huffNode, size_t nowFreq) {
    if (huffNode->left == NULL) {
        return huffNode->frequency;
    }
    nowFreq += countFreqForNode(huffNode->left, nowFreq);
    nowFreq += countFreqForNode(huffNode->right, nowFreq);
    return nowFreq;
}

Queue* push(Queue *pq, HuffNode *node, int *ret) {
    Queue *tmp = malloc(sizeof(Queue));
    if (tmp == NULL) {
        *ret = -1;
        return NULL;
    }

    tmp->node = node;
    tmp->next = pq;
    pq = tmp;

    return pq;
}

HuffNode* pop(Queue **pq) {
    Queue *tmp = *pq;
    *pq = tmp->next;

    HuffNode *el = tmp->node;
    free(tmp);
    return el;
}

Queue* insert(Queue *pq, HuffNode *newNode, int *ret) {
    Queue *tmp = pq;
    if (!pq) {
        return push(pq, newNode, ret);
    } else if (newNode->frequency <= pq->node->frequency) {
        return push(pq, newNode, ret);
    } else {
        while (newNode->frequency > tmp->node->frequency && tmp->next) {
            tmp = tmp->next;
        }

        Queue *newPqNode = malloc(sizeof(Queue));
        if (newPqNode == NULL) {
            *ret = -1;
            return NULL;
        }
        newPqNode->node = newNode;

        if (tmp->next) {
            newPqNode->next = tmp->next;
        } else {
            newPqNode->next = NULL;
        }
        tmp->next = newPqNode;
    }
    return pq;
}

HuffNode* merge(Queue **pq, int *ret) {
    HuffNode *newNode = malloc(sizeof(HuffNode));
    if (newNode == NULL) {
        *ret = -1;
        return NULL;
    }

    newNode->left = pop(pq);
    newNode->right = pop(pq);
    newNode->frequency = newNode->left->frequency + newNode->left->frequency;
    newNode->symbol = 0;
    return newNode;
}

HuffNode *createEncodeHuffTree(HuffNode *huffNodeList, int qs, int *ret) {
    Queue *pq = NULL;
    for (int i = 0; i < qs; i++) {
        pq = push(pq, &huffNodeList[i], ret);
        if (*ret != 0) {
            freeQueue(&pq);
            return NULL;
        }
    }

    for (int i = 0; i < qs - 1; i++) {
        HuffNode *newNode = merge(&pq, ret);
        if (*ret != 0) {
            freeQueue(&pq);
            return NULL;
        }

        pq = insert(pq, newNode, ret);
        if (*ret != 0) {
            freeQueue(&pq);
            return NULL;
        }
    }

    HuffNode *huffTree = NULL;
    if (pq != NULL) huffTree = pq->node;
    free(pq);
    return huffTree;
}

// decoding tree
char byteReadingByBit(FILE *fileIn, unsigned char *byteCode, unsigned char *readBitCount) {
    char bit = ((*byteCode) >> (7 - *readBitCount)) & 1;
    (*readBitCount)++;
    if (*readBitCount >= 8) {
        *byteCode = getc(fileIn);
        (*readBitCount) = 0;
    }
    return bit;
}

int createNewNode(HuffNode **huffTree) {
    HuffNode *huffNode = malloc(sizeof(HuffNode));
    if (huffNode == NULL) return -1;
    huffNode->left = NULL;
    huffNode->right = NULL;
    huffNode->frequency = 0;
    huffNode->symbol = 0;
    *huffTree = huffNode;
    return 0;
}

int addNode(FILE *fileIn, HuffNode *huffTree, unsigned char *byteCode, unsigned char *readBitCount) {
    char bit = byteReadingByBit(fileIn, byteCode, readBitCount);
    if (bit) {
        for (int i = 0; i < 8; i++) {
            bit = byteReadingByBit(fileIn, byteCode, readBitCount);
            huffTree->symbol |= bit << (7 - i);
        }
        huffTree->frequency = 1;
    } else {
        if (createNewNode(&huffTree->left) != 0) return -1;
        if (addNode(fileIn, huffTree->left, byteCode, readBitCount) != 0) return -1;

        if (createNewNode(&huffTree->right) != 0) return -1;
        if (addNode(fileIn, huffTree->right, byteCode, readBitCount) != 0) return -1;
    }
    return 0;
}

HuffNode* createDecodeHuffTree(FILE *fileIn, unsigned char *byteCode, unsigned char *readBitCount, int *ret) {
    HuffNode *huffTree = malloc(sizeof(HuffNode));
    if (huffTree == NULL) {
        *ret = -1;
        return NULL;
    }

    huffTree->right = NULL;
    huffTree->left = NULL;
    huffTree->symbol = 0;
    huffTree->frequency = 0;

    (*byteCode) = getc(fileIn);
    if (feof(fileIn) != 0) {
        freeTree(huffTree);
        return NULL;
    }
    if (addNode(fileIn, huffTree, byteCode, readBitCount) == -1) {
        *ret = -1;
        return NULL;
    }

    return huffTree;
}
