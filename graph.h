#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <stdlib.h>
#include <time.h>

// undirected multigraph
typedef struct Graph {
    size_t n;
    size_t m;
    size_t** adjacency;
} Graph;

Graph* initializeEmptyGraph(size_t n) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->n = n;
    graph->m = 0;
    graph->adjacency = (size_t**)calloc(n, sizeof(size_t*));
    for(size_t v = 0; v < n; v++) {
        graph->adjacency[v] = (size_t*)calloc(n, sizeof(size_t));
    }

    return graph;
}

Graph* copyGraph(Graph* graph) {
    Graph* graphCopy = initializeEmptyGraph(graph->n);
    graphCopy->n = graph->n;
    graphCopy->m = graph->m;
    for(size_t v1 = 0; v1 < graph->n; v1++) {
        for(size_t v2 = 0; v2 < graph->n; v2++) {
            graphCopy->adjacency[v1][v2] = graph->adjacency[v1][v2];
        }
    }

    return graphCopy;
}

Graph* initializeClique(size_t n) {
    Graph* graph = initializeEmptyGraph(n);
    for(size_t v1 = 0; v1 < n; v1++) {
        for(size_t v2 = v1 + 1; v2 < n; v2++) {
            graph->adjacency[v1][v2] = 1;
            graph->adjacency[v2][v1] = 1;
            graph->m++;
        }
    }

    return graph;
}

Graph* initializeStar(size_t n) {
    Graph* graph = initializeEmptyGraph(n);
    size_t v1 = 0;
    for(size_t v2 = 1; v2 < n; v2++) {
        graph->adjacency[v1][v2] = 1;
        graph->adjacency[v2][v1] = 1;
        graph->m++;
    }

    return graph;
}

Graph* initializePath(size_t n) {
    Graph* graph = initializeEmptyGraph(n);
    for(size_t v = 1; v < n; v++) {
        graph->adjacency[v - 1][v] = 1;
        graph->adjacency[v][v - 1] = 1;
        graph->m++;
    }

    return graph;
}

Graph* initializeBipartite(size_t n) {
    Graph* graph = initializeEmptyGraph(2 * n);
    for(size_t v1 = 0; v1 < n; v1++) {
        for(size_t v2 = 0; v2 < n; v2++) {
            graph->adjacency[v1][n + v2] = 1;
            graph->adjacency[n + v2][v1] = 1;
            graph->m++;
        }
    }

    return graph;
}

Graph* initializeRandomGraph(size_t n, float p) {
    Graph* graph = initializeEmptyGraph(n);
    for(size_t v1 = 0; v1 < n; v1++) {
        for(size_t v2 = v1 + 1; v2 < n; v2++) {
            double r = (double)rand() / RAND_MAX;
            if(r < p) {
                graph->adjacency[v1][v2] = 1;
                graph->adjacency[v2][v1] = 1;
                graph->m++;
            }
        }
    }

    return graph;
}

Graph* freeGraph(Graph* graph) {
    if(graph == NULL) {
        return NULL;
    }

    for(size_t v = 0; v < graph->n; v++) {
        free(graph->adjacency[v]);
    }
    free(graph->adjacency);
    free(graph);

    return NULL;
}

#endif // GRAPH_HPP