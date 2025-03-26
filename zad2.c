#include <stdio.h>


// Function to check if str1 is a prefix of str2
// Returns 1 if str1 is a prefix of str2, 0 otherwise
// Python equivalent: str2.startswith(str1)
int isPrefix(const char *str1, const char *str2) {
    size_t index1 = 0;                                      // start from the beginning of the str1
    size_t index2 = 0;                                      // start from the beginning of the str2

    while(str1[index1] != '\0' && str2[index2] != '\0') {   // while both strings are not at the end
        if(str1[index1] != str2[index2]) {  
            return 0;                                       // if the characters are not the same, return 0
        }
        index1++;                                           // move to the next character in str1
        index2++;                                           // move to the next character in str2
    }

    return str1[index1] == '\0';                            // check if str1 is at the end - each character was checked
}

// Function to check if str1 is a suffix of str2
// Returns 1 if str1 is a suffix of str2, 0 otherwise
// Python equivalent: str2.endswith(str1)
int isSuffix(const char *str1, const char *str2) {
    size_t index1 = 0;
    size_t index2 = 0;

    while(str1[index1] != '\0') {                           // find the end of the str1
        index1++;
    }
    while(str2[index2] != '\0') {                           // find the end of the str2
        index2++;
    }

    while(index1 > 0 && index2 > 0) {                       // while both strings are not at the beginning
        if(str1[index1-1] != str2[index2-1]) {
            return 0;                                       // if the characters are not the same, return 0
        }
        index1--;                                           // move to the previous character in str1
        index2--;                                           // move to the previous character in str2
    }

    return index1 == 0;                                     // check if str1 is at the beginning - each character was checked
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s <string1> <string2>\n", argv[0]);
        return 1;
    }

    const char *str1 = argv[1];
    const char *str2 = argv[2];

    printf("\"%s\" is prefix of \"%s\": %s\n", str1, str2, isPrefix(str1, str2) ? "yes" : "no");
    printf("\"%s\" is suffix of \"%s\": %s\n", str1, str2, isSuffix(str1, str2) ? "yes" : "no");

    return 0;
}