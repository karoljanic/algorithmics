#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALPHABET_SIZE 256
#define PRIME 16777213

size_t calculateHash(const char *str, const size_t len) {
    size_t hashValue = 0;
    for (size_t i = 0; i < len; i++) {
        hashValue = ((hashValue * ALPHABET_SIZE) % PRIME + str[i]) % PRIME;
    }
    return hashValue;
}

size_t updateHash(const size_t oldHash, const char oldChar, const char newChar, const size_t qk) {
    const size_t qk1 = (qk * oldChar) % PRIME;
    size_t newHash = oldHash >= qk1 ? (oldHash - qk1) % PRIME : (oldHash + PRIME - qk1) % PRIME;
    newHash = ((newHash * ALPHABET_SIZE) % PRIME + newChar) % PRIME;

    return newHash;
}

void rabinKarp(const char* text, const char* pattern, size_t** matches) {
    const size_t pattern_len = strlen(pattern);
    const size_t text_len = strlen(text);

    size_t q_k = 1;
    for (size_t i = 0; i < pattern_len - 1; i++) {
        q_k = q_k * ALPHABET_SIZE % PRIME;
    }

    const size_t pattern_hash = calculateHash(pattern, pattern_len);
    size_t window_hash = calculateHash(text, pattern_len);

    for (size_t i = 0; i <= text_len - pattern_len; i++) {
        if (pattern_hash == window_hash) {
            if (strncmp(&text[i], pattern, pattern_len) == 0) {
                (*matches)[i] = 1;
            }
        }

        if (i < text_len - pattern_len) {
            window_hash = updateHash(window_hash, text[i], text[i + pattern_len], q_k);
        }
    }
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s <text> <pattern>\n", argv[0]);
        return 1;
    }

    const char *text = argv[1];
    const char *pattern = argv[2];

    size_t *matches = (size_t *)calloc(sizeof(size_t) * (strlen(text) + 1), sizeof(size_t));
    rabinKarp(text, pattern, &matches);

    size_t count = 0;
    for (size_t i = 0; i < strlen(text); i++) {
        if (matches[i] != 0) {
            printf("Pattern found at index: %zu\n", i);
        }
    }

    free(matches);    

    return 0;
}