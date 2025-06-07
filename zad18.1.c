#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


size_t reservoir_sampling_n = 0;

size_t reservoir_sampling_on_get() {
    reservoir_sampling_n++;
    const double treshold = 1.0 / reservoir_sampling_n;
    const double random_value = drand48();
    if (random_value <= treshold) {
        return reservoir_sampling_n;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <target_moments_count>\n", argv[0]);
        return 1;
    }

    const size_t target_momements_count = strtoul(argv[1], NULL, 10);

    srand48(time(NULL));
    
    size_t ticks = 0;
    size_t moments_count = 0;
    while (moments_count < target_momements_count) {
        const size_t sample = reservoir_sampling_on_get();
        if (sample > 0) {
            printf("%zu ", sample);
            moments_count++;
        }
        ticks++;
    }
    printf("\n");

    return 0;
}
