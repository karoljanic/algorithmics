#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int compareDoubles(const void* a, const void* b) {
    double val_a = *(const double*)a;
    double val_b = *(const double*)b;
    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

double estimate(size_t n) {
    if (n == 0) {
        return 0.0;
    }

    double* zetas = (double*)calloc(n, sizeof(double));
    for (size_t i = 0; i < n; ++i) {
        zetas[i] = (double)rand() / RAND_MAX;
    }

    qsort(zetas, n, sizeof(double), compareDoubles);

    if (n <= 400) {
        free(zetas);
        return (double)n;
    } else {
        free(zetas);
        return 399.0 / zetas[399];
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return -1;
    }

    size_t n = atoi(argv[1]);
    printf("n,estimate\n");
    for(size_t i = 0; i < n; ++i) {
        double estimate_value = estimate(i);
        printf("%zu, %.6f\n", i, estimate_value);
    }

    return 0;
}

// E[n] = 399 / zeta(399)
// Var[n] = n * (n-k+1) / (k-2)