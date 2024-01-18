#ifndef GRAPH_H__
#define GRAPH_H__

#include <stdio.h>

// Macro

/**
 * Max length of string
*/
#define _STRING__ 256

/**
 * Forbidden characters for vertex name
*/
#define FORBIDDEN_SEPARATORS "\"\'#%()><{}-/\\|:;,"

/**
 * \brief Positive return code
*/
#define _GRAPH_OK__ 0  

/**
 * \brief Memory allocation error
*/
#define _GRAPH_MEM__ -1

/**
 * \brief Incorrect arguments in function
*/
#define _GRAPH_INCORRECT_ARG__ -2

/**
 * \brief Graph is empty
*/
#define _GRAPH_EMPTY__ -3

/**
 * \brief Object not in graph
*/
#define _GRAPH_NOT_FOUND__ -4

/**
 * \brief Graph already has object
*/
#define _GRAPH_EXIST__ -5

/**
 * \brief Operating system error
*/
#define _GRAPH_OS_ERROR__ -6

// Structs and functions

struct edge
{
    char start_vertex[_STRING__ + 1]; // name of start vertex
    char end_vertex[_STRING__ + 1];   // name of end vertex
    size_t length;                    // length of edge
};

struct graph
{
    char **vertices;        // dynamic array of vertices names
    size_t vertices_amount; // length of vertices array
    struct edge *edges;     // dynamic array of edges
    size_t edges_amount;    // length of edges array
};

typedef int graph_error_t;

/**
 * \brief Initialization of graph by zero
 * 
 * \param[in] graph Graph descriptor
 * 
 * \note If the graph descriptor is NULL, the function will not cause a segmentation error
 */
void graph_initialize(struct graph *graph);

/**
 * \brief Checking for graph emtiness
 * 
 * \param[in] graph Graph descriptor
 * 
 * \return `1` - `True` / `0` - `False`
 * 
 * \note If incorrect arguments are passed, the function returns `1` (`True`)
*/
int graph_is_empty(struct graph *graph);

/**
 * \brief Checking for the presence of a vertex in the graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] vertex Vertex name
 * 
 * \return `1` - `True` / `0` - `False`
 * 
 * \note If incorrect arguments are passed, the function returns `0` (`False`)
*/
int graph_has_vertex(struct graph *graph, const char *vertex);

/**
 * \brief Checking for the presence of a edge in the graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] start_vertex Start vertex name
 * \param[in] end_vertex End vertex name
 * 
 * \return `1` - `True` / `0` - `False`
 * 
 * \note If incorrect arguments are passed, the function returns `0` (`False`)
*/
int graph_has_edge(struct graph *graph, const char *start_vertex, const char *end_vertex);

/**
 * \brief Adding a vertex to a graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] vertex Vertex name
 * 
 * \return `_GRAPH_OK__`, `_GRAPH_MEM__`, `_GRAPH_INCORRECT_ARG__`, `_GRAPH_EXIST__`
 * 
 * \note - You cannot add a copy of an existing vertex
 * \note - You cannot add a vertex with a name of zero length
 * \note - You cannot add a vertex with a name containing special characters - `#%()><{}-/\|:;,` and quotes
*/
graph_error_t graph_add_vertex(struct graph *graph, const char *vertex);

/**
 * \brief Deleting vertex from graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] vertex Vertex name
 * 
 * \return `_GRAPH_OK__`, `_GRAPH_INCORRECT_ARG__`, `_GRAPH_EMPTY__`, `_GRAPH_NOT_FOUND__`
*/
graph_error_t graph_delete_vertex(struct graph *graph, const char *vertex);

/**
 * \brief Adding an edge to a graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] start_vertex Start vertex name
 * \param[in] end_vertex End vertex name
 * 
 * \return `_GRAPH_OK__`, `_GRAPH_MEM__`, `_GRAPH_INCORRECT_ARG__`, `_GRAPH_EXIST__`
 * 
 * \note - You cannot add a copy of an existing edge
 * \note - When adding an edge consisting of new vertices, new vertices will be added to the graph
*/
graph_error_t graph_add_edge(struct graph *graph, const char *start_vertex, const char *end_vertex, size_t edge_length);

/**
 * \brief Deleting edge from graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] start_vertex Start vertex name
 * \param[in] end_vertex End vertex name
 * 
 * \return `_GRAPH_OK__`, `_GRAPH_INCORRECT_ARG__`, `_GRAPH_EMPTY__`, `_GRAPH_NOT_FOUND__`
*/
graph_error_t graph_delete_edge(struct graph *graph, const char *start_vertex, const char *end_vertex);

/**
 * Draw graph using Graphviz and show it
 * 
 * \param[in] graph Graph descriptor
 * 
 * \return `_GRAPH_OK__`, `_GRAPH_MEM__`, `_GRAPH_INCORRECT_ARGS__`, `_GRAPH_OS_ERROR__`
 * 
 * \note - Linux: the graph is demonstrated using `eog`
 * \note - Windows: the graph is demonstrated using `mspaint`
 * \note - The function creates a separate folder for temporary files and deletes it at the end of the work
*/
graph_error_t graph_show(struct graph *graph);

/**
 * \brief Free graph
 * 
 * \param[in] graph Graph descriptor
*/
void graph_free(struct graph *graph);

#endif // GRAPH_H__
