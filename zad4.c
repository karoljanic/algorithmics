#include <stdio.h>
#include <string.h>

// Function to match the pattern in the text - naive algorithm
// Returns the index + 1 of the first occurrence of the pattern in the text, 0 if the pattern is not found
// Python equivalent: text.find(pattern)
size_t naivePatternMatch(const char *text, const char *pattern) {
    // use memcmp to compare the pattern with the text

    size_t textLen = 0;
    while(text[textLen] != '\0') {                              // find the end of the text
        textLen++;
    }

    size_t patternLen = 0;
    while(pattern[patternLen] != '\0') {                        // find the end of the pattern
        patternLen++;
    }

    if(patternLen > textLen) {                                  // if the pattern is longer than the text, return 0
        return 0;
    }

    size_t index = 0;
    while(index <= textLen - patternLen) {                      // while the pattern can fit in the text
        if(memcmp(text + index, pattern, patternLen) == 0) {    // compare the pattern with the text
            return index + 1;                                   // return the index + 1 of the first occurrence of the pattern in the text
        }
        index++;                                                // move to the next character in the text
    }

    return 0;                                                   // return 0 if the pattern is not found
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s <text> <pattern>\n", argv[0]);
        return 1;
    }

    const char *text = argv[1];
    const char *pattern = argv[2];

    const size_t index = naivePatternMatch(text, pattern);
    if(index == 0) {
        printf("The pattern \"%s\" is not found in the text \"%s\"\n", pattern, text);
        return 0;
    }
    else {
        printf("The pattern \"%s\" is found in the text \"%s\" at index: %zu\n", pattern, text, index - 1);
    }

    return 0;
}