#include <stdio.h>
#include <stdlib.h>

#include "hyperloglog.h"

void shuffle(size_t* arr, size_t n) {
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        size_t temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}


int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return 1;
    }

    const size_t N = strtoul(argv[1], NULL, 10);

    const size_t elementsNumber = N * N;
    size_t* elements = (size_t*)calloc(elementsNumber, sizeof(size_t));
    for(size_t i = 0; i < elementsNumber; i++) {
        elements[i] = i;
    }
    
    shuffle(elements, elementsNumber);

    printf("n,b,raw_estimate,estimate\n");
    for(size_t n = 1; n <= N; n++) {
        for(size_t b = 4; b <= 16; b += 1) {
            HyperLogLog* hll = initializeHyperloglog(b);
            if (!hll) {
                fprintf(stderr, "Failed to initialize HyperLogLog with b=%zu\n", b);
                free(elements);
                return 1;
            }

            for(size_t i = 0; i < n; i++) {
                updateHyperloglog(hll, elements[i]);
            }

            double rawEstimate = estimateRaw(hll);
            double estimateValue = estimate(hll);

            printf("%zu,%zu,%f,%f\n", n, b, rawEstimate, estimateValue);

            hll = freeHyperloglog(hll);
        }
    }

    return 0;
}