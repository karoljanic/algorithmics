#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void generateBinarySequences(size_t n, char*** sequences) {
    const size_t sequences_number = (1 << n);
    *sequences = (char**) malloc(sizeof(char*) * sequences_number);

    for(size_t i = 0; i < (1 << n); i++) {
        (*sequences)[i] = (char*) malloc(sizeof(char) * (n + 1));
        for (size_t j = 0; j < n; j++) {
            (*sequences)[i][j] = (i & (1 << (n - j - 1))) ? '1' : '0';
        }
        (*sequences)[i][n] = '\0';
    }
}

size_t lcs(const char *str1, const char *str2) {
    const size_t len1 = strlen(str1);
    const size_t len2 = strlen(str2);
    size_t dp[len1+1][len2+1];

    for (size_t i = 0; i <= len1; i++) {
        for (size_t j = 0; j <= len2; j++) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            }
            else if (str1[i-1] == str2[j-1]) {
                dp[i][j] = 1 + dp[i-1][j-1];
            }
            else {
                dp[i][j] = (dp[i-1][j] > dp[i][j-1]) ? dp[i-1][j] : dp[i][j-1];
            }
        }
    }

    return dp[len1][len2];
}

double expectedLcsLength(size_t n) {
    char **sequences;
    generateBinarySequences(n, &sequences);
    
    size_t sum_of_lcs = 0;
    for(size_t seq1 = 0; seq1 < (1 << n); seq1++) {
        for(size_t seq2 = 0; seq2 < (1 << n); seq2++) {
            size_t lcs_length = lcs(sequences[seq1], sequences[seq2]);
            sum_of_lcs += lcs_length;
        }
    }

    for(size_t i = 0; i < (1 << n); i++) {
        free(sequences[i]);
    }
    free(sequences);

    return ((double)sum_of_lcs) / ((1 << n) * (1 << n));
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <length>\n", argv[0]);
        return 1;
    }

    size_t n = (size_t)atoi(argv[1]);
    printf("E[LCS({0,1}^%zu, {0,1}^%zu)] = %f\n", n, n, expectedLcsLength(n));
}