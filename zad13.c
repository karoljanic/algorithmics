#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"

size_t expected_cut_size(Graph* graph, bool* vertices, size_t v) {
    size_t edges = 0;
    for(size_t u = 0; u < graph->n; u++) {
        if(graph->adjacency[v][u] == 1 && (vertices[v] ^ vertices[u])) {
            edges++;
        }
    }

    return edges;
}

size_t derandomizedMaxCut(Graph* graph, bool* verticesA, bool* verticesB) {
    for(size_t v = 0; v < graph->n; v++) {
        size_t expected_if_v_in_A = expected_cut_size(graph, verticesA, v);
        size_t expected_if_v_in_B = expected_cut_size(graph, verticesB, v);

        if(expected_if_v_in_A < expected_if_v_in_B) {
            verticesA[v] = true;
            verticesB[v] = false;
        }
        else {
            verticesA[v] = false;
            verticesB[v] = true;
        }
    }

    size_t maxCutSize = 0;
    for(size_t v1 = 0; v1 < graph->n; v1++) {
        for(size_t v2 = v1 + 1; v2 < graph->n; v2++) {
            if(graph->adjacency[v1][v2] == 1) {
                if(verticesA[v1] && verticesB[v2] || verticesA[v2] && verticesB[v1]) {
                    maxCutSize++;
                }
            }
        }
    }

    return maxCutSize;
}

void test(const char* name, Graph* graph) {
    bool* verticesA = (bool*)calloc(graph->n, sizeof(bool));
    bool* verticesB = (bool*)calloc(graph->n, sizeof(bool));
    size_t maxCutSize = derandomizedMaxCut(graph, verticesA, verticesB);

    printf("%s: n=%zu, m=%zu; max cut=%zu\n", name, graph->n, graph->m, maxCutSize);

    // printf("A: ");
    // for(size_t v = 0; v < graph->n; v++) {
    //     if(verticesA[v]) { printf(" %zu", v); }
    // }
    // printf("\n");

    // printf("B: ");
    // for(size_t v = 0; v < graph->n; v++) {
    //     if(verticesB[v]) { printf(" %zu", v); }
    // }
    // printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <n> <p>\n", argv[0]);
        return 1;
    }

    size_t n = (size_t)atoi(argv[1]);
    float p = (float)atof(argv[2]);

    Graph* clique = initializeClique(n);
    test("clique", clique);
    freeGraph(clique);    

    Graph* star = initializeStar(n);
    test("star", star);
    freeGraph(star);

    Graph* path = initializePath(n);
    test("path", path);
    freeGraph(path);

    Graph* bipartite = initializeBipartite(n / 2);
    test("bipartite", bipartite);
    freeGraph(bipartite);

    Graph* randomGraph = initializeRandomGraph(n, p);
    test("random graph", randomGraph);
    randomGraph = freeGraph(randomGraph);

    return 0;
}