#include <stdio.h>
#include <stdlib.h>

void derandomizedZerosequenceGenerator(size_t n, char** sequence) {
    *sequence = (char*) malloc(sizeof(char) * (n + 1));
    for(size_t i = 0; i < n; i++) {
        double expected_if_xi_0 = 1.0 / (2 << (n - i + 1));
        double expected_if_xi_1 = 0;
        if(expected_if_xi_0 > expected_if_xi_1) {
            (*sequence)[i] = '0';
        }
        else {
            (*sequence)[i] = '1';
        }
    }
    (*sequence)[n] = '\n';
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <length>\n", argv[0]);
        return 1;
    }

    size_t n = (size_t)atoi(argv[1]);
    char* sequence;
    derandomizedZerosequenceGenerator(n, &sequence);

    printf("0^%zu = %s", n, sequence);
    
    free(sequence);

    return 0;
}