#ifndef HYPERLOGLOG_H
#define HYPERLOGLOG_H

#include <stdint.h>
#include <math.h>


typedef struct HyperLogLog {
    size_t b;
    size_t m;
    double alpha;
    size_t* registers;
} HyperLogLog;


HyperLogLog* initializeHyperloglog(size_t b) {
    if (b < 4 || b > 16) {
        fprintf(stderr, "b must be in the range [4, 16]\n");
        return NULL;
    }

    HyperLogLog* hll = (HyperLogLog*)calloc(1, sizeof(HyperLogLog));
    if (!hll) {
        perror("Failed to allocate memory for HyperLogLog");
        return NULL;
    }

    hll->b = b;
    hll->m = 1 << b;

    if(hll->m == 16) {
        hll->alpha = 0.673;
    }
    else if(hll->m == 32) {
        hll->alpha = 0.697;
    }
    else if(hll->m == 64) {
        hll->alpha = 0.709;
    }
    else {
        hll->alpha = 0.7213 / (1 + 1.079 / hll->m);
    }

    hll->registers = (size_t*)calloc(hll->m, sizeof(size_t));
    if (!hll->registers) {
        perror("Failed to allocate memory for registers");
        free(hll);
        return NULL;
    }

    return hll;
}

uint32_t fnv32Hash(size_t input) {
    const uint32_t FNV_32_PRIME = 16777619;

    uint32_t h = 2166136261;
    while (input) {
        h ^= (uint8_t)(input & 0xFF);
        h *= FNV_32_PRIME;
        input >>= 8;
    }

    return h;
}

void updateHyperloglog(HyperLogLog* hll, size_t index) {
    if(!hll) {
        fprintf(stderr, "Invalid HyperLogLog instance\n");
        return;
    }

    uint32_t hash = fnv32Hash(index);
    size_t registerIndex = hash & (hll->m - 1);
    size_t w = hash >> hll->b;
    size_t rho = __builtin_clz(w) + 1;

    hll->registers[registerIndex] = fmax(hll->registers[registerIndex], rho);
}

double estimateRaw(HyperLogLog* hll) {
    if (!hll) {
        fprintf(stderr, "Invalid HyperLogLog instance\n");
        return -1.0;
    }

    double sum = 0.0;
    for (size_t i = 0; i < hll->m; i++) {
        sum += pow(2.0, -(double)hll->registers[i]);
    }

    return hll->alpha * hll->m * hll->m * (1.0 / sum);
}

double estimate(HyperLogLog* hll) {
    if (!hll) {
        fprintf(stderr, "Invalid HyperLogLog instance\n");
        return -1.0;
    }

    double rawEstimate = estimateRaw(hll);
    if (rawEstimate < 0) {
        return -1.0;
    }

    // small range correction
    if(rawEstimate <= 2.5 * hll->m) {
        double zeroCount = 0.0;
        for (size_t i = 0; i < hll->m; i++) {
            if (hll->registers[i] == 0) {
                zeroCount++;
            }
        }
        
        if (zeroCount == 0) {
            return rawEstimate;
        }

        return hll->m * log(hll->m / zeroCount);
    }
    
    // large range correction
    double twoPowThirtyTwo = pow(2.0, 32);
    if (rawEstimate > 1.0 / 30.0 * twoPowThirtyTwo) {
        return -1.0 * twoPowThirtyTwo * log(1 - rawEstimate / twoPowThirtyTwo);
    }
    
    return rawEstimate;
}

HyperLogLog* freeHyperloglog(HyperLogLog* hll) {
    if (hll) {
        free(hll->registers);
        free(hll);
    }

    return NULL;
}

#endif // HYPERLOGLOG_H