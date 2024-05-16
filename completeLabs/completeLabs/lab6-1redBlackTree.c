#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define STOP_RUNNING(fileIn, ret) do { fclose(fileIn); \
                                return ret; } while(0)

#define freeMem(mem1, mem2, ret) do { free(mem1); \
                                free(mem2); \
                                return ret; } while(0)
enum Constants {
    MAX_HEIGHT_TREE = 22
};

typedef struct redBlackTree {
    struct redBlackTree *left, *right;
    int value;
    char height;
    char isRed;
} Node;

typedef struct parentList {
    Node *parent;
} Parents;

typedef struct dataOfTree {
    Node *treeMemory;
    Node *topOfTheTree;
    Parents *parents;
    int parentsIndex;
    int index;
} Data;

char getHeight(Node* node) {
    if (node) {
        if (node->isRed == 1) {
            return node->height - 1;
        } else {
            return node->height;
        }
    }
    return 1;
}

void fixedHeight(Node* node) {
    char hr = getHeight(node->right);
    char hl = getHeight(node->left);
    node->height = (hr>hl ? hr:hl) + 1;

}

Node* getUncle1(Data *dataOfTree, Node* grandPa) {
    if (grandPa == NULL) return NULL;
    if (dataOfTree->parents[dataOfTree->parentsIndex - 1].parent == grandPa->left) {
        return grandPa->right;
    } else {
        return grandPa->left;
    }
}

Node* leftRotate(Node* node) {
    Node* tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;

    return tmp;
}

Node* rightRotate(Node* node) {
    Node* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;

    return tmp;
}

void blackUncle(Data* dataOfTree, Node *node, Node * dad, Node *grandPa) {
    if (grandPa->left == dad) { // отец - левый
        if (dad->right == node) { // левый потомок у левого отца
            grandPa->left = leftRotate(dad);
            dataOfTree->parents[dataOfTree->parentsIndex - 2].parent->isRed = 1;
        } else {
            dataOfTree->parents[dataOfTree->parentsIndex - 1].parent->isRed = 0;
            grandPa->isRed = 1;
        }


        if (dataOfTree->parentsIndex < 3) { // дед - корень дерева
            dataOfTree->topOfTheTree = rightRotate(grandPa);
        } else {
            if (dataOfTree->parents[dataOfTree->parentsIndex - 3].parent->left == grandPa) { // дед - левая или правая вершина
                dataOfTree->parents[dataOfTree->parentsIndex - 3].parent->left = rightRotate(grandPa);
            } else {
                dataOfTree->parents[dataOfTree->parentsIndex - 3].parent->right = rightRotate(grandPa);
            }
        }

    } else { // отец - правый
        if (dad->left == node) {
            grandPa->right = rightRotate(dad);
            dataOfTree->parents[dataOfTree->parentsIndex - 2].parent->isRed = 1;
        } else {
            dataOfTree->parents[dataOfTree->parentsIndex - 1].parent->isRed = 0;
            grandPa->isRed = 1;
        }

        if (dataOfTree->parentsIndex < 3) { // дед - корень дерева
            dataOfTree->topOfTheTree = leftRotate(grandPa);
        } else {
            if (dataOfTree->parents[dataOfTree->parentsIndex - 3].parent->right == grandPa) { // дед - левая или правая вершина
                dataOfTree->parents[dataOfTree->parentsIndex - 3].parent->right = leftRotate(grandPa);
            } else {
                dataOfTree->parents[dataOfTree->parentsIndex - 3].parent->left = leftRotate(grandPa);
            }
        }
    }
}

void fixedTree(Data *dataOfTree, Node *node) {
    Node *dad = NULL, *grandPa = NULL, *uncle = NULL;
    if (dataOfTree->parentsIndex >= 1) {
        dad = dataOfTree->parents[dataOfTree->parentsIndex - 1].parent;
    }
    if (dataOfTree->parentsIndex >= 2) {
        grandPa = dataOfTree->parents[dataOfTree->parentsIndex - 2].parent;
        uncle = getUncle1(dataOfTree, grandPa);
    }
    if (dad == NULL) {
        node->isRed = 0;
    } else if (grandPa == NULL) { // отец - корень дерева
        dad->isRed = 0;
    } else if (dad->isRed == 1) {
        if (uncle != NULL && uncle->isRed == 1) { // имеется дядя и он красный
            dad->isRed = 0;
            uncle->isRed = 0;
            grandPa->isRed = 1;
            dataOfTree->parentsIndex -= 2;
            fixedTree(dataOfTree, grandPa);
        } else {
            blackUncle(dataOfTree, node, dad, grandPa);
        }
    }

    fixedHeight(node);
    for (int i = dataOfTree->parentsIndex; i > 0; i--) {
        fixedHeight(dataOfTree->parents[i - 1].parent);
    }
}

void searchPlaceAndInsert(Data *dataOfTree, Node *newNode) {
    Node* tmp = dataOfTree->topOfTheTree;
    Node* parent = NULL;
    int i = 0;
    for (; tmp != NULL; i++) {
        parent = tmp;
        dataOfTree->parents[i].parent = parent;
        if (newNode->value >= tmp->value) {
            tmp = tmp->right;
        } else {
            tmp = tmp->left;
        }
    }

    dataOfTree->parentsIndex = i; //во всех последующих функциях: отец - индекс минус 1, дед - индекс минус 2
    if (newNode->value >= parent->value) {
        parent->right = newNode;
    } else {
        parent->left = newNode;
    }
}

int insert(Data* dataOfTree, int value) {
    Node *newNode = &dataOfTree->treeMemory[dataOfTree->index];
    newNode->value = value;
    newNode->height = 1;
    newNode->isRed = 1;
    newNode->left = NULL;
    newNode->right = NULL;

    dataOfTree->parents = malloc(MAX_HEIGHT_TREE * sizeof(Node));
    if (dataOfTree->parents == NULL) return ENOMEM;
    dataOfTree->parentsIndex = 0;

    if (dataOfTree->topOfTheTree == NULL) {
        dataOfTree->topOfTheTree = newNode;
    } else {
        searchPlaceAndInsert(dataOfTree, newNode);
    }


    fixedTree(dataOfTree, newNode);
    free(dataOfTree->parents);
    return 0;
}

int redBlackTree(FILE* fileIn, int quantityVertexes) {
    Data *dataOfTree = malloc(sizeof(Data));
    if (dataOfTree == NULL) return ENOMEM;

    dataOfTree->topOfTheTree = NULL;
    dataOfTree->treeMemory = malloc(quantityVertexes * sizeof(Node));
    if (dataOfTree->treeMemory == NULL) freeMem(NULL, dataOfTree, ENOMEM);

    int value = 0, ret = 0;
    for (int i = 0; i < quantityVertexes; i++) {
        if (fscanf(fileIn, "%d", &value) == 0) freeMem(dataOfTree->treeMemory, dataOfTree, EIO);

        dataOfTree->index = i;
        ret = insert(dataOfTree, value);
        if (ret != 0) freeMem(dataOfTree->treeMemory, dataOfTree, EIO);
    }

    printf("%d", dataOfTree->topOfTheTree->height);

    freeMem(dataOfTree->treeMemory, dataOfTree, ret);
}

int main() {
    FILE* fileIn = fopen("in.txt", "r");
    if (fileIn == NULL) return 0;

    int quantityVertexes = 0, ret = 0;
    if (fscanf(fileIn, "%d", &quantityVertexes) == 0) STOP_RUNNING(fileIn, -3);

    if (quantityVertexes != 0) {
        ret = redBlackTree(fileIn, quantityVertexes);
    } else {
        printf("%d", 0);
    }
    STOP_RUNNING(fileIn, ret);
}