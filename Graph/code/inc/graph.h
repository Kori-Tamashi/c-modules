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
#define _GRAPH_FORBIDDEN_SEPARATORS__ "\"\'#%()><{}-/\\|:;,"

/**
 * \brief Positive return code
*/
#define _GRAPH_OK__ 0  

/**
 * \brief Memory shortage error
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

/**
 * \brief Matrix
 * 
 * \param values Matrix values
 * \param rows Amount of rows in matrix
 * \param columns Amount of columns in matrix
 */
struct matrix
{
    int **values;
    size_t rows;
    size_t columns;
};

/**
 * \brief Edge of graph
 * 
 * \param start_vertex Name of start vertex
 * \param end_vertex Name of end vertex
 * \param length Length of edge
 */
struct edge
{
    char start_vertex[_STRING__ + 1]; 
    char end_vertex[_STRING__ + 1];   
    size_t length;                    
};

/**
 * \brief Graph
 * 
 * \param vertices Dynamic array of vertices names
 * \param vertices_amount Length of vertices array
 * \param edges Dynamic array of edges
 * \param edges_amount Length of edges array
 */
struct graph
{
    char **vertices;        
    size_t vertices_amount; 
    struct edge *edges;     
    size_t edges_amount;    
};

/**
 * \brief Data type for errors that occur during the operation of functions
 */
typedef int graph_error_t;

/**
 * \brief Initialization of graph by zero
 * 
 * \param[in] graph Graph descriptor
 * 
 * \note - If the graph descriptor is NULL, the function will not cause a segmentation error
 */
void graph_initialize(struct graph *graph);

/**
 * \brief Checking for graph emtiness
 * 
 * \param[in] graph Graph descriptor
 * 
 * \return `1` - `True` / `0` - `False`
 * 
 * \note - If incorrect arguments are passed, the function returns `1` (`True`)
*/
int graph_is_empty(const struct graph *graph);

/**
 * \brief Checking for the presence of a vertex in the graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] vertex Vertex name
 * 
 * \return `1` - `True` / `0` - `False`
 * 
 * \note - If incorrect arguments are passed, the function returns `0` (`False`)
*/
int graph_has_vertex(const struct graph *graph, const char *vertex);

/**
 * \brief Checking for the presence of a edge in the graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] start_vertex Start vertex name
 * \param[in] end_vertex End vertex name
 * 
 * \return `1` - `True` / `0` - `False`
 * 
 * \note - If incorrect arguments are passed, the function returns `0` (`False`)
*/
int graph_has_edge(const struct graph *graph, const char *start_vertex, const char *end_vertex);

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
 * \brief Draw graph using Graphviz and show it
 * 
 * \param[in] graph Graph descriptor
 * 
 * \return `_GRAPH_OK__`, `_GRAPH_MEM__`, `_GRAPH_INCORRECT_ARGS__`, `_GRAPH_OS_ERROR__`
 * 
 * \note - Linux: the graph is demonstrated using `eog`
 * \note - Windows: the graph is demonstrated using `mspaint`
 * \note - The function creates a separate folder for temporary files and deletes it at the end of the work
*/
graph_error_t graph_show(const struct graph *graph);

/**
 * \brief Creating a dot file by graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] folder Folder name
 * \param[in] filename File name
 * 
 * \return `_GRAPH_OK__`, `_GRAPH_INCORRECT_ARG__`,`_GRAPH_MEM__`, `_GRAPH_OS_ERROR__`
 * 
 * \note - The pointer to the `folder` string can take the `NULL` value. In this case, the folder will not be created
*/
graph_error_t graph_to_dot(const struct graph *graph, const char *folder, const char *filename);

/**
 * \brief Counting the number of adjacent vertices (the size of the adjacency list)
 * 
 * \param[in] graph Graph descriptor
 * \param[in] vertex Vertex name
 * 
 * \return The number of adjacent vertices
 * 
 * \note - If the arguments is incorrect, the function returns 0
*/
size_t graph_adjacency_list_size(const struct graph *graph, const char *vertex);

/**
 * \brief Filling in the adjacency list
 * 
 * \param[in] graph Graph descriptor
 * \param[in] vertex Vertex name
 * \param[in] size Adjacency list size
 * \param[out] adjacency_list Adjacency list descriptor
 * 
 * \return `_GRAPH_OK__`, `_GRAPH_INCORRECT_ARG__`
 * 
*/
graph_error_t graph_adjacency_list_fill(const struct graph *graph, const char *vertex, int *adjacency_list);

/**
 * \brief Graph traversal using a depth-first search algorithm
 * 
 * \param[in] graph Graph descriptor
 * \param[in] vertex_processing Vertex processing function
 * 
 * \note - If the input arguments are incorrect, the function will not work
*/
void graph_dfs(struct graph *graph, void (*vertex_processing)(char *vertex_name));

/**
 * \brief Creating adjacency matrix by graph
 * 
 * \param[in] graph Graph descriptor
 * 
 * \return Adjacency matrix descriptor
 * 
 * \note - If errors occur, the function returns NULL
 */
struct matrix *graph_adjacency_matrix_create(const struct graph *graph);

/**
 * \brief Creating a dot file of adjacency matrix of graph
 * 
 * \param[in] graph Graph descriptor
 * \param[in] adjacency_matrix Adjacency matrix descriptor
 * \param[in] folder Folder name
 * \param[in] filename File name
 * 
 * \return `_GRAPH_OK__`, `_GRAPH_INCORRECT_ARG__`,`_GRAPH_MEM__`, `_GRAPH_OS_ERROR__`
 * 
 * \note - The pointer to the `folder` string can take the `NULL` value. In this case, the folder will not be created
*/
graph_error_t graph_adjacency_matrix_to_dot(const struct graph *graph, const struct matrix *adjacency_matrix, const char *folder, const char *filename);

/**
 * \brief Draw graph adjacency matrix using Graphviz and show it
 * 
 * \param[in] graph Graph descriptor
 * \param[in] adjacency_matrix Adjacency matrix descriptor
 * 
 * \return `_GRAPH_OK__`, `_GRAPH_MEM__`, `_GRAPH_INCORRECT_ARGS__`, `_GRAPH_OS_ERROR__`
 * 
 * \note - Linux: the graph adjacency matrix  is demonstrated using `eog`
 * \note - Windows: the graph adjacency matrix  is demonstrated using `mspaint`
 * \note - The function creates a separate folder for temporary files and deletes it at the end of the work
*/
graph_error_t graph_adjacency_matrix_show(const struct graph *graph, const struct matrix *adjacency_matrix);

/**
 * \brief Free adjacency matrix
 * 
 * \param[in] adjacency_matrix Adjacency matrix descriptor
 */
void graph_adjacency_matrix_free(struct matrix *adjacency_matrix);

/**
 * \brief Finding the shortest distance matrix using the Floyd-Warshall algorithm
 * 
 * \param graph Graph descriptor
 * 
 * \return The shortest distance matrix
 * 
 * \note - If errors occur, the function returns NULL
 */
struct matrix *graph_floyd_warshall(const struct graph *graph);

/**
 * \brief Free graph
 * 
 * \param[in] graph Graph descriptor
*/
void graph_free(struct graph *graph);

#endif // GRAPH_H__