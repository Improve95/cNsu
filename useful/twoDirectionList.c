#include <stdio.h>
#include <stdlib.h>

typedef struct twoDirectionList {
    int val;
    struct twoDirectionList* next;
    struct twoDirectionList* prev;
} twodir;

typedef struct dblLinkedList {
    size_t size;
    twodir* head;
    twodir* tail;
} dblListInfo;

dblListInfo* createTwoDirList() {
    dblListInfo *tmp = malloc(sizeof(dblListInfo));
    tmp->size = 0;
    tmp->head = tmp->tail = NULL;
    return tmp;
}

void destroyDblLInkedList(dblListInfo* list) {
    twodir *tmp = list->head;
    twodir *next = NULL;
    while(tmp) {
        next = tmp->next;
        free(tmp);
        tmp = next;
    }
    free(list);
    list = NULL;
}

void pushFront(dblListInfo* list, int el) {
    twodir* tmp = malloc(sizeof(twodir));
    tmp->val = el;
    tmp->next = list->head;
    tmp->prev = NULL;
    if (list->head) {
        list->head->prev = tmp;
    }
    list->head = tmp;

    if (list->tail == NULL) {
        list->tail = tmp;
    }
    list->size++;
}

void pushBack(dblListInfo* list, int el) {
    twodir* tmp = malloc(sizeof(twodir));
    tmp->val = el;
    tmp->prev = list->tail;
    tmp->next = NULL;
    if (list->tail) {
        list->tail->next = tmp;
    }
    list->tail = tmp;

    if (list->head == NULL) {
        list->head = tmp;
    }
    list->size++;
}

int popFront(dblListInfo* list) {
    twodir* tmp = list->head;
    int el = tmp->val;
    list->head = tmp->next;

    free(tmp);
    list->size--;
    return el;
}

int popBack(dblListInfo* list) {
    twodir* tmp = list->tail;
    int el = tmp->val;
    list->tail = tmp->prev;

    if (list->tail) {
        list->tail->next = NULL;
    }

    free(tmp);
    list->size--;
    return el;
}


twodir* getNth(dblListInfo* list, size_t index) {
    twodir* tmp = NULL;
    if (index <= (list->size / 2)) {
        tmp = list->head;
        for (size_t i = 0; (i < index) && (tmp->next != NULL); i++) {
            tmp = tmp->next;
        }
    } else {
        tmp = list->tail;
        for(size_t i = 0; (i < list->size - index - 1) && (tmp->prev != NULL); i++) {
            tmp = tmp->prev;
        }
    }
    return tmp;
}

void insertNth(dblListInfo* list, size_t index, int el) {
    if (index == 0) {
        pushFront(list, el);
        return;
    }
    if (index == list->size - 1) {
        pushBack(list, el);
        return;
    }
    twodir* tmp = getNth(list, index - 1);
    twodir* ins = malloc(sizeof(twodir));
    ins->val = el;

    ins->next = tmp->next;
    tmp->next->prev = ins;
    tmp->next = ins;
    ins->prev = tmp;
    list->size++;
}

int PopNth(dblListInfo* list, size_t index) {
    if (index == 0) {
        return popFront(list);
    }
    if (index == list->size - 1) {
        return popBack(list);
    }
    twodir* tmp = getNth(list, index);
    int el = tmp->val;
    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;

    free(tmp);
    list->size--;
    return el;
}

void foo(dblListInfo* list) {
    pushFront(list, 1);
    pushFront(list, 2);
    pushFront(list, 3);
    pushFront(list, 4);
    pushFront(list, 5);
    pushBack(list, -1);
    pushBack(list, -2);
    pushBack(list, -3);
}

int main() {
    dblListInfo *list = createTwoDirList();
    foo(list);

    printf("%zu\n", list->size);
    printf("%d\n", getNth(list, 0)->val);
    printf("%d\n", getNth(list, 6)->val);

    destroyDblLInkedList(list);
}