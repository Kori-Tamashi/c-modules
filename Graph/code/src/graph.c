#include <stdlib.h>
#include "graph.h"

struct edge
{
    char start[_STRING__ + 1]; // name of start vertex
    char end[_STRING__ + 1];   // name of end vertex
    size_t length;             // length of edge
};

struct graph
{
    char **vertices;        // dynamic array of vertices names
    size_t vertices_amount; // length of vertices array
    struct edge *edges;     // dynamic array of edges
    size_t edges_amount;    // length of edges array
};
