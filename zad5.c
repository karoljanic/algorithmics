#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 256

typedef struct {
    char* pattern;
    size_t pattern_len;
    size_t** transitions;
} DFA;

// pattern[:state] + c endswith pattern[:state+1]
int prefix_sufix(const char* pattern, char c, size_t state) {
    char* prefix = malloc(state + 2);
    strncpy(prefix, pattern, state);
    prefix[state] = c;
    prefix[state + 1] = '\0';

    char* sufix = malloc(state + 2);
    strncpy(sufix, pattern, state + 1);
    sufix[state + 1] = '\0';

    return strcmp(prefix, sufix) == 0;
}

DFA* dfa_init(const char* pattern) {
    DFA* dfa = malloc(sizeof(DFA));
    dfa->pattern = strdup(pattern);
    dfa->pattern_len = strlen(pattern);

    size_t states_num = dfa->pattern_len + 1;
    dfa->transitions = malloc(sizeof(size_t*) * states_num);
    for (size_t i = 0; i < states_num; i++) {
        dfa->transitions[i] = calloc(ALPHABET_SIZE, sizeof(size_t));
    }

    for(size_t state = 0; state < states_num; state++) {
        for(size_t c = 0; c < ALPHABET_SIZE; c++) {
            size_t next_state = state + 1;
            while(next_state > 0 && prefix_sufix(dfa->pattern, c, state) == 0) {
                next_state--;
            }
            dfa->transitions[state][c] = next_state;
        }
    }

    return dfa;
}

void dfa_free(DFA* dfa) {
    size_t states_num = dfa->pattern_len + 1;
    for (size_t i = 0; i < states_num; i++) {
        free(dfa->transitions[i]);
    }
    free(dfa->transitions);
    free(dfa->pattern);
    free(dfa);
}

size_t match(const DFA* dfa, const char* text) {
    if(dfa->pattern_len == 0) {
        return 1;
    }

    size_t text_len = strlen(text);
    size_t state = 0;
    for (size_t i = 0; i < text_len; i++) {
        state = dfa->transitions[state][(size_t)text[i]];
        if (state == dfa->pattern_len) {
            return i - dfa->pattern_len + 2;
        }
    }
    return 0;
}


int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("Usage: %s <pattern> <text>\n", argv[0]);
        return 1;
    }

    const char* pattern = argv[1];
    const char* text = argv[2];

    DFA* dfa = dfa_init(pattern);

    size_t index = match(dfa, text);
    if(index == 0) {
        printf("Pattern not found in the text\n");
    } else {
        printf("Pattern found at index %zu\n", index - 1);
    }

    dfa_free(dfa);

    return 0;
}