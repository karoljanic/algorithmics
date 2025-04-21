#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ALPHABET_SIZE 4
#define CODON_LENGTH 3

#define START_CODONS 1
#define STOP_CODONS 3
#define MIN_MIDDLE_DISTANCE 30

#define START_CODON_TYPE 1
#define STOP_CODON_TYPE 2

const char *startCodons[] = {"ATG"};
const char *stopCodons[] = {"TAA", "TAG", "TGA"};

int32_t startHashes[START_CODONS];
int32_t stopHashes[STOP_CODONS];

int32_t letterCode(char c) {
    switch(c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        default: return -1;
    }
}

int32_t calculateHash(const char *str, size_t len) {
    int32_t hashValue = 0;
    for(size_t i = 0; i < len; i++) {
        hashValue = hashValue * ALPHABET_SIZE + letterCode(str[i]);
    }
    return hashValue;
}

int32_t updateHash(int32_t oldHash, char oldChar, char newChar, size_t qk) {
    int32_t qk1 = qk * letterCode(oldChar);
    int32_t newHash = (oldHash - qk1) * ALPHABET_SIZE + letterCode(newChar);
    return newHash;
}

void initialize_hashes() {
    for(size_t i = 0; i < START_CODONS; i++) {
        startHashes[i] = calculateHash(startCodons[i], CODON_LENGTH);
    }

    for(size_t i = 0; i < STOP_CODONS; i++) {
        stopHashes[i] = calculateHash(stopCodons[i], CODON_LENGTH);
    }
}

int isStartCodon(int32_t hashValue) {
    for(size_t i = 0; i < START_CODONS; i++) {
        if(startHashes[i] == hashValue) {
            return 1;
        }
    }
    return 0;
}

int isStopCodon(int32_t hashValue) {
    for(size_t i = 0; i < STOP_CODONS; i++) {
        if(stopHashes[i] == hashValue) {
            return 1;
        }
    }
    return 0;
}

void match_dna(const char *text, size_t **matches) {
    const size_t text_length = strlen(text);
    int32_t qk = 1;
    for(size_t i = 0; i < CODON_LENGTH - 1; i++) {
        qk = qk * ALPHABET_SIZE;
    }

    int32_t window_hash = calculateHash(text, CODON_LENGTH);
    size_t* tmp = (size_t *)calloc(sizeof(size_t) * (strlen(text) + 1), sizeof(size_t));
    for(size_t i = 0; i <= text_length - CODON_LENGTH; i++) {
        if(isStartCodon(window_hash)) {
            tmp[i] = START_CODON_TYPE;
        } else if(isStopCodon(window_hash)) {
            tmp[i] = STOP_CODON_TYPE;
        }

        if(i < text_length - CODON_LENGTH) {
            window_hash = updateHash(window_hash, text[i], text[i + CODON_LENGTH], qk);
        }
    }

    int found = 0;
    size_t begin = 0;
    size_t middle_distance = 0;
    for(size_t i = 0; i < text_length; i++) {
        if(tmp[i] == START_CODON_TYPE) {
            found = 1;
            begin = i;
            middle_distance = 0;
            i += CODON_LENGTH - 1;
        } else if(tmp[i] == STOP_CODON_TYPE) {
            if(found && middle_distance >= MIN_MIDDLE_DISTANCE) {
                (*matches)[begin] = i + CODON_LENGTH;
                found = 0;
                begin = 0;
                middle_distance = 0;
                i += CODON_LENGTH - 1;
            }
            else {
                found = 0;
                begin = 0;
                middle_distance = 0;
            }
        }
        else {
            if(found) {
                middle_distance++;
            }
        }
    }

    free(tmp);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s <text>\n", argv[0]);
        return 1;
    }

    const char *text = argv[1];

    initialize_hashes();

    size_t *matches = (size_t *)calloc(sizeof(size_t) * (strlen(text) + 1), sizeof(size_t));
    match_dna(text, &matches);
    for(size_t i = 0; i < strlen(text); i++) {
        if(matches[i] != 0) {
            printf("Matched: [%zu:%zu]: ", i, matches[i]);
            printf("%.*s\n", i - matches[i] + 1, text + i);
        }
    }

    free(matches);

    return 0;
}