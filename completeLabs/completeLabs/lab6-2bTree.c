#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STOP_RUNNING(file, ret) do { fclose(file); \
                                    return ret; } while(0)

typedef struct Node {
    struct Node **child;
    int *keys;
    int n;
} Node;

typedef struct BTree {
    Node* root;
    int power;
} BTree;

char isLeaf(Node *node) {
    return node->child[0] == NULL;
}

void freeMem(Node* node) {
    for (int i = 0; i < node->n + 1 && node->child[i] != NULL; i++)
        freeMem(node->child[i]);
    free(node->keys);
    free(node->child);
    free(node);
}

Node* create(BTree *bTree, char *ret) {
    Node *tmp = malloc(sizeof(Node));
    if (tmp == NULL) {
        *ret = -1;
        return 0;
    }
    tmp->child = calloc(2 * bTree->power, sizeof(Node*));
    tmp->keys = malloc((2 * bTree->power - 1) * sizeof(int));
    if (tmp->child == NULL || tmp->keys == NULL) {
        free(tmp->child);
        free(tmp->keys);
        free(tmp);
        *ret = -1;
        return 0;
    }
    tmp->n = 0;
    return tmp;
}

int getIndex(Node *node, int key) {
    if (node->n <= 0) return 0;
    if (key > node->keys[node->n - 1]) return node->n;
    int left = 0, right = node->n - 1, middle = (right / 2);
    while ((left < right) && (middle < right) && (middle > left)) {
        if (key == node->keys[middle]) {
            return middle;
        } else if (key > node->keys[middle]) {
            left = middle;
            middle = (middle + right) / 2;
        } else {
            right = middle;
            middle = (middle + left) / 2;
        }
    }
    if (key > node->keys[middle]) middle++;
    return middle;
}

void addKey(Node *node, int key, int index) {
    if (node->n > 0) {
        if (index < node->n) {
            memmove(&node->keys[index + 1], &node->keys[index], (node->n - index) * sizeof(int));
        }
        node->keys[index] = key;
    } else {
        node->keys[0] = key;
    }
    node->n++;
}

int split(BTree *bTree, Node *node, int power, int index) {
    char ret = 0;
    Node *rightNode = create(bTree, &ret);
    if (ret != 0) return -1;

    Node *leftNode = node->child[index];
    leftNode->n = power - 1;

    for (int i = 0; i < power - 1; i++) {
        rightNode->keys[i] = leftNode->keys[i + power];
    }
    if (!isLeaf(node)) {
        for (int i = 0; i < power; i++)
            rightNode->child[i] = leftNode->child[i + power];
    }
    rightNode->n = power - 1;

    for (int i = node->n; i > index; i--)
        node->child[i + 1] = node->child[i];
//    memmove(&node->child[index + 2], &node->keys[index + 1], (node->n - index) * sizeof(Node*));
    node->child[index + 1] = rightNode;

    memmove(&node->keys[index + 1], &node->keys[index], (node->n - index) * sizeof(int));
    node->keys[index] = leftNode->keys[power - 1];

    node->n++;
    return ret;
}

int insertNonFull(BTree *bTree, Node* node, int power, int key) {
    int c = getIndex(node, key);
    if (isLeaf(node)) {
        addKey(node, key, c);
    } else {
        if ((node->child[c])->n == (2 * power - 1)) {
            if (split(bTree, node, power, c) != 0) return -1;
            if (key > node->keys[c]) c++;
        }
        if (insertNonFull(bTree, node->child[c], power, key) != 0) return -1;
    }
    return 0;
}

int insert(BTree *bTree, int key) {
    char ret = 0;
    if (bTree->root == NULL) {
        bTree->root = create(bTree, &ret);
        if (ret != 0) return -1;
    }

    Node* root = bTree->root;
    if (root->n >= (2 * bTree->power - 1)) {
        Node *s = create(bTree, &ret);
        if (ret != 0) return -1;

        bTree->root = s;
        s->n = 0;
        s->child[0] = root;
        if (split(bTree, s, bTree->power, 0) != 0) return -1;
        if (insertNonFull(bTree, s, bTree->power, key) != 0) return -1;
    } else {
        if (insertNonFull(bTree, bTree->root, bTree->power, key) != 0) return -1;
    }

    return 0;
}

int bTree(FILE* fileIn, int power, int quantityVertexes) {
    BTree * bTree = malloc(sizeof(BTree));
    if (bTree == NULL) return -1;
    bTree->root = NULL;
    bTree->power = power;

    int key = 0;
    for (int i = 0; i < quantityVertexes; i++) {
        if (fscanf(fileIn, "%d", &key) == 0) return -1;
        if (insert(bTree, key) == -1) {
            freeMem(bTree->root);
            free(bTree);
            return -1;
        };
    }

    int height = 1;
    Node* node = bTree->root;
    while (!isLeaf(node)) {
        height++;
        node = node->child[0];
    }
    printf("%d", height);

    freeMem(bTree->root);
    free(bTree);
    return 0;
}

int main() {
    FILE* fileIn = fopen("in.txt", "r");
    if (fileIn == NULL) return 0;

    int power = 0; int quantityVertexes = 0, ret = 0;
    if (fscanf(fileIn, "%d", &power) == 0) STOP_RUNNING(fileIn, -1);
    if (fscanf(fileIn, "%d", &quantityVertexes) == 0) STOP_RUNNING(fileIn, -1);

    if (quantityVertexes == 0) {
        printf("%d", 0);
    } else {
        ret = bTree(fileIn, power, quantityVertexes);
    }

    STOP_RUNNING(fileIn, ret);
}
