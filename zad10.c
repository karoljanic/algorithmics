#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t newLcsPosition(const char* lcs, char*** lcss) {    
    size_t i = 0;
    while((*lcss)[i] != NULL) {
        if(strcmp(lcs, (*lcss)[i]) == 0) {
            return 0;
        }
        i++;
    }

    return i + 1;
}

void lcsBacktrack(
    const char *str1, const char *str2, const char* str3,
    const size_t i, const size_t j, const size_t k, const size_t lcsLen, 
    const size_t currLcsLen, char** currLcs, char ***lcss) {
    if(currLcsLen == lcsLen) {
        (*currLcs)[currLcsLen] = '\0';
        const size_t lcsCounter = newLcsPosition(*currLcs, lcss);
        if(lcsCounter > 0) {
            (*lcss)[lcsCounter - 1] = (char*)malloc((currLcsLen + 1) * sizeof(char));
            strncpy((*lcss)[lcsCounter - 1], *currLcs, currLcsLen + 1);
        }
        return;
    }
    
    if(i == 0 || j == 0 || k == 0) {
        return;
    }

    if(str1[i-1] == str2[j-1] && str1[i-1] == str3[k-1]) {
        (*currLcs)[currLcsLen] = str1[i-1];
        lcsBacktrack(str1, str2, str3, i-1, j-1, k-1, lcsLen, currLcsLen + 1, currLcs, lcss);
    } else {
        if (i > 0 && j > 0 && k > 0) {
            lcsBacktrack(str1, str2, str3, i-1, j, k, lcsLen, currLcsLen, currLcs, lcss);
            lcsBacktrack(str1, str2, str3, i, j-1, k, lcsLen, currLcsLen, currLcs, lcss);
            lcsBacktrack(str1, str2, str3, i, j, k-1, lcsLen, currLcsLen, currLcs, lcss);
        }
    }

}

void reverseLcss(char*** lcss) {
    size_t i = 0;
    while((*lcss)[i] != NULL) {
        const size_t len = strlen((*lcss)[i]);
        size_t s = 0;
        size_t e = len - 1;
        while(s < e) {
            char tmp = (*lcss)[i][s];
            (*lcss)[i][s] = (*lcss)[i][e];
            (*lcss)[i][e] = tmp;
            s++;
            e--;
        }

        i++;
    }
}

size_t lcs(const char *str1, const char *str2, const char* str3, char ***lcss) {
    const size_t len1 = strlen(str1);
    const size_t len2 = strlen(str2);
    const size_t len3 = strlen(str3);

    size_t dp[len1+1][len2+1][len3+1];

    for (size_t i = 0; i <= len1; i++) {
        for (size_t j = 0; j <= len2; j++) {
            for (size_t k = 0; k <= len3; k++) {
                if (i == 0 || j == 0 || k == 0) {
                    dp[i][j][k] = 0;
                }
                else if (str1[i-1] == str2[j-1] && str1[i-1] == str3[k-1]) {
                    dp[i][j][k] = dp[i-1][j-1][k-1] + 1;
                }
                else {
                    size_t l1 = dp[i-1][j][k];
                    size_t l2 = dp[i][j-1][k];
                    size_t l3 = dp[i][j][k-1];
                    dp[i][j][k] = (l1 > l2) ? ((l1 > l3) ? l1 : l3) : ((l2 > l3) ? l2 : l3);
                }
            }
        }
    }

    if(dp[len1][len2][len3] != 0) {
        *lcss = (char **)calloc(dp[len1][len2][len3] * sizeof(char *), sizeof(char *));
        char* tmpLcs = (char*)malloc(dp[len1][len2][len3] + 1);
        lcsBacktrack(str1, str2, str3, len1, len2, len3, dp[len1][len2][len3], 0, &tmpLcs, lcss);

        // Free the temporary LCS string
        free(tmpLcs);
    }

    reverseLcss(lcss);

    return dp[len1][len2][len3];
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <str1> <str2> <str3>\n", argv[0]);
        return 1;
    }

    const char *str1 = argv[1];
    const char *str2 = argv[2];
    const char *str3 = argv[3];

    char **lcss = NULL;
    size_t length = lcs(str1, str2, str3, &lcss);

    printf("Length of LCS(%s, %s, %s) = %zu\n", str1, str2, str3, length);
    if (length > 0) {
        printf("LCS(%s, %s, %s) = {\n", str1, str2, str3);
        for(size_t i = 0; i < length; i++) {
            if(lcss[i] != NULL) {
                printf("   %s\n", lcss[i]);
            }
        }
        printf("}\n");
        
        // Free the allocated memory
        for(size_t i = 0; i < length; i++) {
            if(lcss[i] != NULL) {
                free(lcss[i]);
            }
        }
        free(lcss);
    } else {
        printf("No common subsequence found.\n");
    }

    return 0;
}

// TESTCASE: axbcdeyz xaceyz xyzabce