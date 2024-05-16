#include <stdio.h>
#include <limits.h>

int main() {
    int lenList = 0, count = 0;
    long long int sum = 0, d = 0, max = INT_MAX, min = INT_MIN;

    if (scanf("%d", &lenList) == 0) return 0;
    if (lenList > 10 || lenList < 1) {
        printf("bad input");
        return 0;
    }

    while (scanf("%lld", &d) == 1) {
        if (d < (0 - min) || d > max) {
            printf("bad input");
            return 0;
        }

        sum += d;
        if (sum < (0-min) || sum > max) {
            printf("overflow");
            return 0;
        }

        count += 1;
    }

    if (count != lenList) {
        printf("bad input");
        return 0;
    }

    printf("%lld", sum);
    return 0;
}
