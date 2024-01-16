#ifndef GRAPH_H__
#define GRAPH_H__

#include <stdio.h>

// Macro

/**
 * Max length of string
*/
#define _STRING__ 256

/**
 * \brief Positive return code
*/
#define _GRAPH_OK__ 0  

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

// Structs and functions

struct edge;

struct graph;

typedef int graph_error_t;

/**
 * \brief Initialization of graph by zero
 * 
 * \param[in] graph Graph descriptor
 */
void graph_initialize(struct graph *graph);

/**
 * \brief Checking for graph emtiness
 * 
 * \param[in] graph Graph descriptor
 * 
 * \return `1` - `True` / `0` - `False`
*/
int graph_is_empty(struct graph *graph);

/**
 * \brief Checking for the presence of a vertex in the graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] vertex Vertex name
 * 
 * \return `1` - `True` / `0` - `False`
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
*/
int graph_has_edge(struct graph *graph, const char *start_vertex, const char *end_vertex);

/**
 * \brief Adding a vertex to a graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] vertex Vertex name
 * 
 * \return `_GRAPH_OK__`, `_GRAPH_MEM__`, `_GRAPH_INCORRECT_ARG__`, `_GRAPH_EXIST__`
*/
graph_error_t graph_add_vertex(struct graph *graph, const char *vertex);

/**
 * \brief Deleting vertex from graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] vertex Vertex name
 * 
 * \return `_GRAPH_OK__`, `_GRAPH_MEM__`, `_GRAPH_INCORRECT_ARG__`, `_GRAPH_EXIST__`
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
*/
graph_error_t graph_add_edge(struct graph *graph, const char *start_vertex, const char *end_vertex, size_t edge_length);

/**
 * \brief Deleting edge from graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] start_vertex Start vertex name
 * \param[in] end_vertex End vertex name
 * 
 * \return `_GRAPH_OK__`, `_GRAPH_MEM__`, `_GRAPH_INCORRECT_ARG__`, `_GRAPH_EMPTY__`, `_GRAPH_NOT_FOUND__`
*/
graph_error_t graph_delete_edge(struct graph *graph, const char *start_vertex, const char *end_vertex);

#endif // GRAPH_H__