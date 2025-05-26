#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>

void simulateRandomAllocation(size_t ballsNum, size_t binsNum, size_t* bins) {
    for (size_t i = 0; i < ballsNum; ++i) {
        size_t bin = rand() % binsNum;
        bins[bin]++;
    }
}

void simulatePowerOfDChoices(size_t ballsNum, size_t binsNum, size_t dChoices, size_t* bins) {
    if (dChoices < 2) {
        fprintf(stderr, "Error: dChoices must be at least 2 for Power of D Choices.\n");
        return;
    }

    for (size_t i = 0; i < ballsNum; ++i) {
        size_t* chosenBins = (size_t*)calloc(binsNum, sizeof(size_t));
        size_t chosenBinsCount = 0;
        do {
            size_t bin = rand() % binsNum;
            if (chosenBins[bin] == 0) {
                chosenBins[bin] = 1;
                chosenBinsCount++;
            }
        }
        while (chosenBinsCount < dChoices);

        size_t minLoad = INT_MAX;
        size_t chosenBinIndex = 0;
        for (size_t j = 0; j < binsNum; ++j) {
            if (chosenBins[j] == 1) {
                if (bins[j] < minLoad) {
                    minLoad = bins[j];
                    chosenBinIndex = j;
                }
            }
        }

        bins[chosenBinIndex]++;

        free(chosenBins);
    }
}

void analyze(size_t* bins, size_t binsNum) {
    size_t maxLoad = 0;
    size_t emptyBins = 0;
    for (size_t i = 0; i < binsNum; ++i) {
        if (bins[i] > maxLoad) {
            maxLoad = bins[i];
        }
        if (bins[i] == 0) {
            emptyBins++;
        }
    }

    size_t* histogram = (size_t*)calloc(maxLoad + 1, sizeof(size_t));
    for (size_t i = 0; i < binsNum; ++i) {
        histogram[bins[i]]++;
    }

    printf("Max load: %zu\n", maxLoad);
    printf("Empty bins: %zu\n", emptyBins);
    printf("Histogram:\n");
    for (size_t i = 0; i <= maxLoad; ++i) {
        if (histogram[i] != 0) {
            printf("%zu: %zu\n", i, histogram[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <ballsNum> <binsNum> <dChoices>\n", argv[0]);
        return -1;
    }

    size_t ballsNum = atoi(argv[1]);
    size_t binsNum = atoi(argv[2]);
    size_t dChoices = atoi(argv[3]);

    if (binsNum == 0 || ballsNum == 0) {
        fprintf(stderr, "Error: Number of balls and bins must be greater than zero.\n");
        return -1;
    }

    srand(time(NULL));

    size_t* bins1 = (size_t*)calloc(binsNum, sizeof(size_t));
    simulateRandomAllocation(ballsNum, binsNum, bins1);

    size_t* bins2 = (size_t*)calloc(binsNum, sizeof(size_t));
    simulatePowerOfDChoices(ballsNum, binsNum, dChoices, bins2);

    printf("Random Allocation:\n");
    analyze(bins1, binsNum);
    printf("E[#empty bins] = %.2f\n", (double)binsNum / pow(2.72, (double)ballsNum / binsNum));
    printf("E[max load] = %.2f\n", (double)ballsNum / binsNum + log(ballsNum) / log(log(ballsNum)));

    printf("\nPower of d Choices (d=%zu):\n", dChoices);
    analyze(bins2, binsNum);
    printf("E[#empty bins] = %.2f\n", (double)binsNum / pow(2.72, (double)ballsNum / binsNum));
    printf("E[max load] = %.2f\n", (double)ballsNum / binsNum + log(log(binsNum)) / dChoices);

    free(bins1);
    free(bins2);

    return 0;
}


// n = balls, m = bins

// Uniform allocation:
// E[#empty bins] = m * (1 - 1/m)^n
// E[max load] = n/m + logn / loglogn ?

// Power of d Choices:
// E[#empty bins] = m * (1 - 1/m)^n ?
// E[max load (power of d choices)] = n/m + loglogm/d
