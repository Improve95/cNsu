#include <stdio.h>
#include <stdlib.h>

typedef struct list {
    int val;
    struct list* next;
} onedir;

void push(onedir** List, int el) {
    onedir* tmp = malloc(sizeof(onedir));
    tmp->next = *List;
    tmp->val = el;
    *List = tmp;
}

int pop(onedir** List) {
    onedir* tmp = *List;
    *List = tmp->next;
    int el = tmp->val;
    free(tmp);
    return el;
}

void destroyStack(onedir** List) {
    while (*List != NULL) {
        onedir* tmp = *List;
        *List = tmp->next;
        free(tmp);
    }
}

onedir* getNth(onedir* List, int n) {
    int i = 0;
    while (i < n && List) {
        List = List->next;
        i++;
    }
    return List;
}

onedir* getLast(onedir* head) {
    while (head->next) {
        head = head->next;
    }
    return head;
}

onedir* getLastButOne(onedir* head) {
    while (head->next->next) {
        head = head->next;
    }
    return head;
}

void pushBack(onedir* head, int el) {
    onedir* last = getLast(head);
    onedir* tmp = malloc(sizeof(onedir));
    tmp->val = el;  
    tmp->next = NULL;
    last->next = tmp;
}

void popBack(onedir** List) {
    if (!List) return;
    if (!(*List)) return;

    onedir* lastbn = getLastButOne(*List);
    if (lastbn == NULL) {
        free(*List);
        *List = NULL;
    } else {
        free(lastbn->next);
        lastbn->next = NULL;
    }
}

void insert(onedir* List, size_t n, int el) {
    size_t i = 0;
    while (i < n && List->next) {
        List = List->next;
        i++;
    }
    onedir* tmp = malloc(sizeof(onedir));
    tmp->val = el;
    if (List->next) {
        tmp->next = List->next;
    } else {
        tmp->next = NULL;
    }
    List->next = tmp;
}

int deleteNth(onedir** List, size_t n) {
    if (n == 0) {
        return pop(List);
    } else {
        onedir* prev = getNth(*List, n-1);
        onedir* elm = prev->next;
        int val = elm->next;

        prev->next = elm->next;
        free(elm);
        return val;
    }
}

void foo(onedir** List) {
    push(List, 1);
    push(List, 2);
    push(List, 3);
    push(List, 4);
    push(List, 5);
}

int main() {
    onedir* List = NULL;
    foo(&List);
    destroyStack(&List);
}
