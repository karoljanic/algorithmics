#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


size_t reservoir_sampling_n = 0;
size_t reservoir_sampling_next_index = 0;

void generate_moments(size_t target_moments_count, size_t* moments) {
    moments[0] = 1;
    for (size_t i = 1; i < target_moments_count; i++) {
        double random_value = (double)rand() / RAND_MAX;
        moments[i] = moments[i - 1] + (size_t)(random_value * target_moments_count / (1 - random_value)) + 1;
    }
}

size_t reservoir_sampling_fast_forward_get(size_t* moments) {
    reservoir_sampling_n++;
    if (reservoir_sampling_n == moments[reservoir_sampling_next_index]) {
        reservoir_sampling_next_index++;
        return reservoir_sampling_n;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <target_moments_count>\n", argv[0]);
        return 1;
    }

    const size_t target_moments_count = strtoul(argv[1], NULL, 10);
    size_t* moments = malloc(target_moments_count * sizeof(size_t));
    if (!moments) {
        perror("Failed to allocate memory for moments");
        return 1;
    }

    srand(time(NULL));
    generate_moments(target_moments_count, moments);

    size_t ticks = 0;
    size_t moments_count = 0;
    while (moments_count < target_moments_count) {
        const size_t sample = reservoir_sampling_fast_forward_get(moments);
        if (sample > 0) {
            printf("%zu ", sample);
            moments_count++;
        }
        ticks++;
    }
    printf("\n");

    free(moments);

    return 0;
}