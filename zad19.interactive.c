#include <stdio.h>
#include <stdlib.h>

#include "hyperloglog.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <b value>\n", argv[0]);
        return 1;
    }

    const size_t b = strtoul(argv[1], NULL, 10);
    HyperLogLog* hll = initializeHyperloglog(b);

    char command;
    while(scanf("%c", &command) != EOF) {
        if(command == 'q') {
            break;
        }
        else if(command == 'a') {
            size_t index;
            scanf("%zu", &index);
            updateHyperloglog(hll, index);
        }
        else if(command == 'r') {
            printf("%f\n", estimateRaw(hll));
        }
        else if(command == 'e') {
            printf("%f\n", estimate(hll));
        }
    }

    hll = freeHyperloglog(hll);

    return 0;
}