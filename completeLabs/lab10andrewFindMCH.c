#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FULL_ANGLE 360
#define STRAIGHT_ANGLE 180
#define PI 3.14159265358979323846

typedef struct {
    int x, y;
} Point;

int compare(const Point *a, const Point *b) {
    return !(a->x < b->x || (a->x == b->x && a->y < b->y));
}

int cw(Point *a, Point *b, Point *c) {
    double angle1 = atan2((double)b->y - a->y, (double)b->x - a->x) * STRAIGHT_ANGLE / PI;
    double angle2 = atan2((double)c->y - a->y, (double)c->x - a->x) * STRAIGHT_ANGLE / PI;
    int left = angle1 < angle2 ? FULL_ANGLE + (angle1 - angle2) : angle1 - angle2;
    int right = angle2 < angle1 ? FULL_ANGLE + (angle2 - angle1) : angle2 - angle1;
    if (right < left) {
        return -1;
    }
    else if (angle1 == angle2) {
        return 0;
    }
    else {
        return 1;
    }
}

int andrewFindMCH(Point *points, int quantPoints) {
    qsort(points, quantPoints, sizeof(Point), (int(*) (const void *, const void *)) compare);
    Point *p1 = &points[0], *p2 = &points[quantPoints - 1];

    Point *up = malloc(quantPoints * sizeof(Point)), *down = malloc(quantPoints * sizeof(Point));
    if (up == NULL || down == NULL) goto RS_ANDREW;
    int upIndex = 0, downIndex = 0;
    up[upIndex++] = *p1;
    down[downIndex++] = *p1;

    for (int i = 1; i < quantPoints; i++) {
        if (i == quantPoints - 1 || cw(p1, &points[i], p2) > 0) {
            while(upIndex >= 2 && cw(&up[upIndex - 2], &up[upIndex - 1], &points[i]) <= 0) {
                upIndex--;
            }
            up[upIndex++] = points[i];
        }
        if (i == quantPoints - 1 || cw(p1, &points[i], p2) < 0) {
            while(downIndex >= 2 && cw(&down[downIndex - 2], &down[downIndex - 1], &points[i]) >= 0) {
                downIndex--;
            }
            down[downIndex++] = points[i];
        }
    }

    for (int i = 0; i < upIndex; i++){
        printf("%d %d\n", up[i].x, up[i].y);
    }
    for (int i = downIndex - 2; i > 0; i--){
        printf("%d %d\n", down[i].x, down[i].y);
    }

    RS_ANDREW:
    free(up);
    free(down);
    return 0;
}

int main(void) {
    FILE *fileIn = fopen("in.txt", "r");
    if (fileIn == NULL) return -1;

    int quantPoints = 0, ret = 0;
    if (fscanf(fileIn, "%d", &quantPoints) == 0) goto SR_MAIN1;
    if (quantPoints < 0 || quantPoints > 100000) {
        printf("bad number of points");
        goto SR_MAIN1;
    }
    if (quantPoints == 0) goto SR_MAIN1;

    Point *points = malloc(quantPoints * sizeof(Point));
    if (points == NULL) goto SR_MAIN1;
    for (int i = 0; i < quantPoints; i++) {
        if (fscanf(fileIn, "%d %d", &points[i].x, &points[i].y) < 2) {
            printf("bad number of lines");
            goto SR_MAIN2;
        }
    }
    if (quantPoints == 1) {
        printf("%d %d\n", points[0].x, points[0].y);
        goto SR_MAIN2;
    }

    ret = andrewFindMCH(points, quantPoints);

    SR_MAIN2:
    free(points);
    SR_MAIN1:
    fclose(fileIn);
    return ret;
}
