#include <stdio.h>

// Function to find the longest prefix of str1 that is also a suffix of str2
// Returns the length of the longest prefix that is also a suffix
size_t longestSufixPrefix(const char *str1, const char *str2) {
    size_t index1 = 0;
    size_t index2 = 0;
    
    while(str2[index2] != '\0') {                               // find the end of the str2
        index2++;
    }

    size_t longest = 0;
    while(str1[index1] != '\0') {      
        for(size_t k = 0; k < index1 + 1; k++) {                // check if the prefix of str1 is also a suffix of str2
            if(str1[k] != str2[index2 - index1 + k - 1]) {
                break;
            }
            if(k == index1) {                                   // if the prefix of str1 is also a suffix of str2
                longest = index1 + 1;                           // update the length of the longest prefix that is also a suffix
            }
        }

        index1++;
    }

    return longest;
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s <string1> <string2>\n", argv[0]);
        return 1;
    }

    const char *str1 = argv[1];
    const char *str2 = argv[2];

    printf("The longest prefix of \"%s\" that is also a suffix of \"%s\" has length: %zu\n", str1, str2, longestSufixPrefix(str1, str2));

    return 0;
}