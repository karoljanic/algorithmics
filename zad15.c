#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "graph.h"

void getRandomEdge(Graph* graph, size_t* v1, size_t* v2) {
    size_t r = rand() % graph->m;
    size_t count = 0;
    for (size_t u1 = 0; u1 < graph->n; u1++) {
        for (size_t u2 = u1 + 1; u2 < graph->n; u2++) {
            count += graph->adjacency[u1][u2];
            if (count > r) {
                *v1 = u1;
                *v2 = u2;
                return;
            }
        }
    }
}

Graph* contractGraph(Graph* graph, size_t v1, size_t v2, size_t* mapping) {
    Graph* contractedGraph = initializeEmptyGraph(graph->n - 1);

    size_t currentNewVertex = 1;
    for (size_t v = 0; v < graph->n; v++) {
        if (v == v1 || v == v2) {
            mapping[v] = 0;
        } else {
            mapping[v] = currentNewVertex;
            currentNewVertex++;
        }
    }

    for (size_t u1 = 0; u1 < graph->n; u1++) {
        for (size_t u2 = 0; u2 < graph->n; u2++) {
            size_t newu1 = mapping[u1];
            size_t newu2 = mapping[u2];
            if (newu1 != newu2) {
                contractedGraph->adjacency[newu1][newu2] += graph->adjacency[u1][u2];
                contractedGraph->m += graph->adjacency[u1][u2];
            } else {
                contractedGraph->adjacency[newu1][newu2] = 0; // discard self loops
            }
        }
    }
    contractedGraph->m /= 2;

    return contractedGraph;
}

// Karger's Algorithm
size_t randomizedMinCut(Graph* graph, bool* verticesA, bool* verticesB) {
    Graph* tmpGraph = copyGraph(graph);
    size_t* finalMapping = (size_t*)malloc(sizeof(size_t) * graph->n);
    for(size_t v = 0; v < graph->n; v++) {
        finalMapping[v] = v;
    }

    while(tmpGraph->n > 2) {
        size_t v1, v2;
        getRandomEdge(tmpGraph, &v1, &v2);
        size_t* mapping = (size_t*)malloc(sizeof(size_t) * tmpGraph->n);
        Graph* contractedGraph = contractGraph(tmpGraph, v1, v2, mapping);
        for(size_t v = 0; v < graph->n; v++) {
            finalMapping[v] = mapping[finalMapping[v]];
        }
        free(mapping);
        tmpGraph = freeGraph(tmpGraph);
        tmpGraph = contractedGraph;
    }

    size_t minCutSize = tmpGraph->adjacency[0][1];
    freeGraph(tmpGraph);

    for(size_t v = 0; v < graph->n; v++) {
        if(finalMapping[v] == 0) {
            verticesA[v] = true;
            verticesB[v] = false;
        }
        else {
            verticesA[v] = false;
            verticesB[v] = true;
        }
    }
    free(finalMapping);

    return minCutSize;
}


void test(const char* name, Graph* graph) {
    bool* verticesA = (bool*)calloc(graph->n, sizeof(bool));
    bool* verticesB = (bool*)calloc(graph->n, sizeof(bool));
    size_t minCutSize = randomizedMinCut(graph, verticesA, verticesB);

    printf("%s: n=%zu, m=%zu; min cut=%zu\n", name, graph->n, graph->m, minCutSize);

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

    srand(time(NULL));

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