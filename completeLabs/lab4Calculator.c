#include <stdio.h>
#include <stdlib.h>
#define STOP_RUNNING(file, struct) do { fclose(file); \
                                free(struct); \
                                return 0; } while(0)

#define STOP_RUNNING_WITH_ERROR(fileIn, struct) do { printf("syntax error"); \
                                    fclose(fileIn); \
                                    free(struct); \
                                    return 0; } while(0)

#define STOP_RUNNING_IN_CREATING_STACK(stack1, stack2, error) do { \
                                    destroyStack(stack1);  \
                                    destroyStack(stack2);  \
                                    return -1; } while(0)

enum Constants {
    EXPRESSIONS_SIZE = 1500,
};
// if function return -1 then error happened

// struct for main expressions
typedef struct ExpressionData {
    char expressionStr[EXPRESSIONS_SIZE];
    int lenExpression;
} expData;

// struct for creating expressions
typedef struct StackForNumAndOper {
    struct StackForNumAndOper* next;
    int num;
    char oper;
} creatingStack;

int push(creatingStack** StackForNumAndOper, int num, char oper) {
    creatingStack* StackForNumAndOper2 = malloc(sizeof(creatingStack));
    if (StackForNumAndOper2 == NULL) return -1;
    StackForNumAndOper2->next = *StackForNumAndOper;
    StackForNumAndOper2->num = num;
    StackForNumAndOper2->oper = oper;
    *StackForNumAndOper = StackForNumAndOper2;
    return 0;
}

void pop(creatingStack** StackForNumAndOper, int* num, char* oper) {
    creatingStack* tmp = *StackForNumAndOper;
    *StackForNumAndOper = tmp->next;

    int returnNum = tmp->num;
    *num = returnNum;

    char returnOper = tmp->oper;
    *oper = returnOper;

    free(tmp);
}

void destroyStack(creatingStack** StackForNumAndOper) {
    while (*StackForNumAndOper != NULL) {
        creatingStack* tmp = *StackForNumAndOper;
        *StackForNumAndOper = tmp->next;
        free(tmp);
    }
}

int validateExp(expData* expCalc) {
    for (int i = 0; i < expCalc -> lenExpression - 1; ++i) {
        char symbolNow = expCalc->expressionStr[i], symbolNext = expCalc->expressionStr[i + 1];
        if ((symbolNow < '0' || symbolNow > '9') && (symbolNow < '(' || symbolNow > '+') && symbolNow != '-' && symbolNow != '/') return -1;
        if ((symbolNow == '(' && symbolNext == ')' ) || (symbolNow == ')' && symbolNext == '(')) return -1;
    }
    if ('*' <= expCalc->expressionStr[expCalc->lenExpression - 1] && expCalc->expressionStr[expCalc->lenExpression - 1] <= '/') return -1;
    if ('*' <= expCalc->expressionStr[0] && expCalc->expressionStr[0] <= '/') return -1;
    return 0;
}

int validateExpWhenCreating(expData *expCalc, int indexInMainExp) {
    int symbolNow = expCalc->expressionStr[indexInMainExp], symbolNext = expCalc->expressionStr[indexInMainExp + 1];
    if ((')' < symbolNow && symbolNow < '0') && (')' < symbolNext && symbolNext < '0')) return -1;

    if (('0' <= symbolNow && symbolNow <= '9') && (symbolNext == '(')) return -1;
    if ((')' < symbolNow && symbolNow < '0') && (symbolNext == ')')) return -1;

    if ((symbolNow == '(') && (')' < symbolNext && symbolNext < '0')) return -1;
    if ((symbolNow == ')') && ('0' <= symbolNext && symbolNext <= '9')) return -1;
    return 0;
}

int getPriorityOperation(char operation) {
    switch (operation) {
        case '+':
            return 1;
        case '-':
            return 1;
        case '*':
            return 2;
        case '/':
            return 2;
        default:
            return 0;
    }
}

char getOperationFromStack(creatingStack** operationStack) {
    creatingStack* tmp = *operationStack;
    return tmp->oper;
}

int addNumInMainStack(expData* expCalc, creatingStack** mainStack, int* indexInMainExp) {
    int num = expCalc->expressionStr[*indexInMainExp] - '0';
    for (; *indexInMainExp < expCalc->lenExpression - 1; *indexInMainExp += 1) {
        if (expCalc->expressionStr[*indexInMainExp + 1] > 47 && expCalc->expressionStr[*indexInMainExp + 1] < 58) {
            num *= 10;
            num += expCalc->expressionStr[*indexInMainExp + 1] - '0';
        } else {
            break;
        }
    }
    if (push(mainStack, num, -1) == -1) return -1; //add oper = -1 if you want to add numbers in stack
    return 0;
}

int binaryOperation(creatingStack** mainStack, creatingStack** operationStack) {
    int a = 0, b = 0, numNotUse = 0;
    char operation = 0, operNotUse = 0;
    pop(operationStack, &numNotUse, &operation);
    pop(mainStack, &a, &operNotUse);
    pop(mainStack, &b, &operNotUse);
    switch (operation) {
        case '+':
            if (push(mainStack, a+b, -1) == -1) return -1;
            break;
        case '-':
            if (push(mainStack, b-a, -1) == -1) return -1;
            break;
        case '*':
            if (push(mainStack, a*b, -1) == -1) return -1;
            break;
        case '/':
            if (a == 0) {
                printf("division by zero");
                return -1;
            }
            if (push(mainStack, b/a, -1) == -1) return -1;
            break;
        default:
            break;
    }
    return 0;
}

int addOperationInOperationStack(creatingStack** mainStack, creatingStack** operationStack, char symbol) {
    if (*operationStack == NULL) {
        if (push(operationStack, -1, symbol) == -1) return -1;
    } else {
        while (*operationStack != NULL && getPriorityOperation(symbol) <= getPriorityOperation(getOperationFromStack(operationStack))) {
            if (binaryOperation(mainStack, operationStack) == -1) return -1;
        }
        if (push(operationStack, -1, symbol) == -1) return -1;
    }
    return 0;
}

int createPostfixExp(expData* expCalc, creatingStack** mainStack, int* indexInMainExp, int* leftBracket, int* rightBracket) {
    creatingStack* operationStack = NULL;
    for (; *indexInMainExp < expCalc->lenExpression; *indexInMainExp += 1) {
        int symbol = (int)expCalc->expressionStr[*indexInMainExp];
        if (validateExpWhenCreating(expCalc, *indexInMainExp) == -1) STOP_RUNNING_IN_CREATING_STACK(mainStack, &operationStack, -1);
        if (symbol >= '0' && symbol <= '9') {
            if (addNumInMainStack(expCalc, mainStack, indexInMainExp)) STOP_RUNNING_IN_CREATING_STACK(mainStack, &operationStack, -1);
        } else if ('*' <= symbol && symbol <= '/') {
            if (addOperationInOperationStack(mainStack, &operationStack, symbol)) STOP_RUNNING_IN_CREATING_STACK(mainStack, &operationStack, -1);
        } else if (symbol == '(') {
            *leftBracket += 1;
            *indexInMainExp += 1;
            createPostfixExp(expCalc, mainStack, indexInMainExp, leftBracket, rightBracket);
        } else if (symbol == ')') {
            *rightBracket += 1;
            while (operationStack != NULL) if (binaryOperation(mainStack, &operationStack) == -1) STOP_RUNNING_IN_CREATING_STACK(mainStack, &operationStack, -1);
            return 0;
        }
    }
    while (operationStack != NULL) if (binaryOperation(mainStack, &operationStack) == -1) STOP_RUNNING_IN_CREATING_STACK(mainStack, &operationStack, -1);
    destroyStack(&operationStack);
    return 0;
}

int calculate(expData* expCalc, int *res) {
    creatingStack* mainStack = NULL;
    int leftBracket = 0, rightBracket = 0, indexInMainExp = 0;
    if (createPostfixExp(expCalc, &mainStack, &indexInMainExp, &leftBracket, &rightBracket) == -1) return -1;
    if (leftBracket != rightBracket) return -1;
    char oper = 0;
    pop(&mainStack, res, &oper);
    return 0;
}

int main() {
    FILE* fileIn = fopen("in.txt", "r");
    if (fileIn == NULL) return 0;

    expData *expCalc = malloc(sizeof(expData));
    if (expCalc == NULL) STOP_RUNNING(fileIn, expCalc);

    int ch = 0, i = 0, res = 0;
    while ((ch = fgetc(fileIn)) != EOF && ch != '\n') {
        expCalc -> expressionStr[i] = ch;
        i++;
    }
    expCalc -> lenExpression = i;

    if (expCalc->lenExpression == 0 || validateExp(expCalc) == -1) STOP_RUNNING_WITH_ERROR(fileIn, expCalc);

    if (calculate(expCalc, &res) == -1) STOP_RUNNING_WITH_ERROR(fileIn, expCalc);
    printf("%d", res);

    STOP_RUNNING(fileIn, expCalc);
}
