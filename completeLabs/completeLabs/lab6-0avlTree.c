#include <stdio.h>
#include <stdlib.h>
#define STOP_RUNNING(fileIn) do { fclose(fileIn); \
                                  return 0; } while(0)

typedef struct avlTreeStruct {
    struct avlTreeStruct* left;
    struct avlTreeStruct* right;
    int value;
    char height;
} Node;

char height(Node* node) {
    return node?node->height:0;
}

void fixHeight(Node* node) {
    char hl = height(node->left);
    char hr = height(node->right);
    node->height = (hl>hr?hl:hr) + 1;
}

char getHeightDiff(Node* node) {
    return height(node->right) - height(node->left);
}

Node* leftRotate(Node* node) {
    Node* tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    fixHeight(node);
    fixHeight(tmp);
    return tmp;
}

Node* rightRotate(Node* node) {
    Node* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    fixHeight(node);
    fixHeight(tmp);
    return tmp;
}

Node* balance(Node* balanceNode) {
    fixHeight(balanceNode);
    char heightDiff = getHeightDiff(balanceNode);

    if (heightDiff == 2) {
        if (getHeightDiff(balanceNode->right) < 0) {
            balanceNode->right = rightRotate(balanceNode->right);
        }
        return leftRotate(balanceNode);
    }

    if (heightDiff == -2) {
        if (getHeightDiff(balanceNode->left) > 0) {
            balanceNode->left = leftRotate(balanceNode->left);
        }
        return rightRotate(balanceNode);
    }

    return balanceNode;
}

Node* insert(Node* rootNode, Node* newNode) {
    if (!rootNode) return newNode;
    if (newNode->value >= rootNode->value) {
        rootNode->right = insert(rootNode->right, newNode);
    } else {
        rootNode->left = insert(rootNode->left, newNode);
    }
    return balance(rootNode);
}

int avlTree(FILE* fileIn, int quantityVertexes) {
    Node* avlTree = malloc(quantityVertexes * sizeof(Node));
    if (avlTree == NULL) return -1;

    int value = 0;
    Node* topOfTheTree = NULL;
    for (int i = 0; i < quantityVertexes; i++) {
        if (fscanf(fileIn, "%d", &value) == 0) {
            free(avlTree);
            return -1;
        }

        avlTree[i].value = value;
        avlTree[i].left = NULL;
        avlTree[i].right = NULL;
        avlTree[i].height = 1;

        topOfTheTree = insert(topOfTheTree, &avlTree[i]);
    }
    printf("%d", topOfTheTree->height);
    free(avlTree);
    return 0;
}

int main() {
    FILE* fileIn = fopen("in.txt", "r");
    if (fileIn == NULL) return 0;

    int quantityVertexes = 0;
    if (fscanf(fileIn, "%d", &quantityVertexes) == 0) STOP_RUNNING(fileIn);

    if (quantityVertexes != 0) {
        if (avlTree(fileIn, quantityVertexes) == -1) STOP_RUNNING(fileIn);
    } else {
        printf("%d", 0);
    }

    STOP_RUNNING(fileIn);
}
