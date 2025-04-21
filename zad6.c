#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to evaluate polynomial using Horner's method
// a[]: coefficients of the polynomial - a[i] is the coefficient of x^(n-i)
// n: degree of the polynomial
// x: the value at which to evaluate the polynomial
// adds: number of additions performed
// muls: number of multiplications performed
double horner(double a[], const size_t n, const double x, size_t* adds, size_t* muls) {
    double result = a[0];
    for (size_t i = 1; i <= n; i++) {
        result = result * x + a[i];
        (*muls)++;
        (*adds)++;
    }
    return result;
}

double randDouble(double min, double max) {
    return min + ((double)rand() / (RAND_MAX + 1.0)) * (max - min);
}

void testCorrectness() {
    double a[] = {2, 3, 4, 5};
    size_t n = sizeof(a) / sizeof(a[0]) - 1;
    double x = 2.0;
    size_t adds = 0, muls = 0;
    double result = horner(a, n, x, &adds, &muls);
        
    printf("Result: %f. Expected is 41.0\n", result);
    printf("Additions: %zu. Expected is 3\n", adds);
    printf("Multiplications: %zu. Expected is 3\n", muls);
}

void testComplexityForN(size_t n, size_t reps) {
    double* a = (double*)malloc((n + 1) * sizeof(double));
    for (size_t i = 0; i <= n; i++) {
        a[i] = randDouble(0.0, 10.0);
    }

    size_t adds = 0, muls = 0;
    double total_time = 0.0;
    for(size_t rep = 0; rep < reps; rep++) {
        double x = randDouble(0.0, 10.0);
        clock_t start = clock();
        double value = horner(a, n, x, &adds, &muls);
        clock_t end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }

    printf("%zu, %f, %f, %f\n", n, total_time / reps, (double)adds / reps, (double)muls / reps);

    free(a);
}

void testComplexity() {
    printf("n,avg_time,avg_adds,avg_muls\n");
    for(size_t n = 10000; n <= 1000000; n += 10000) {
        testComplexityForN(n, 100);
    }
}

int main() {
    testCorrectness();
    // testComplexity();

    return 0;
}