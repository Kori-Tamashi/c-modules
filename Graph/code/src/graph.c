#include <stdlib.h>
#include <string.h>
#include "graph.h"

void graph_initialize(struct graph *graph)
{   
    *graph = (struct graph) {0};
}

int graph_is_empty(const struct graph *graph)
{
    if (graph)
        return graph->vertices_amount == 0;
    return 1;
}

int graph_has_vertex(const struct graph *graph, const char *vertex)
{
    if (graph && vertex)
    {
        if (!graph_is_empty(graph))
        {
            for (size_t i = 0; i < graph->vertices_amount; i++)
            {
                if (!strcmp(vertex, graph->vertices[i]))
                    return 1;
            }
        }
    }

    return 0;
}

int graph_has_edge(const struct graph *graph, const char *start_vertex, const char *end_vertex)
{
    if (graph && start_vertex && strlen(start_vertex) && end_vertex && strlen(end_vertex))
    {
        if (!graph_is_empty(graph))
        {
            if (graph_has_vertex(graph, start_vertex) && graph_has_vertex(graph, end_vertex))
            {
                for (size_t i = 0; i < graph->edges_amount; i++)
                {
                    struct edge current_edge = graph->edges[i];
                    
                    if (!strcmp(start_vertex, current_edge.start_vertex) \
                        && !strcmp(end_vertex, current_edge.end_vertex))

                        return 1;
                }
            }
        }
    }

    return 0;
}

graph_error_t graph_add_vertex(struct graph *graph, const char *vertex)
{
    if (!graph || !vertex || !strlen(vertex))
        return _GRAPH_INCORRECT_ARG__;

    for (size_t i = 0; vertex[i] != '\0'; i++)
    {
        if (strchr(_GRAPH_FORBIDDEN_SEPARATORS__, vertex[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    if (graph_has_vertex(graph, vertex))
        return _GRAPH_EXIST__;

    // expanding a dynamic array of vertices

    char **tmp = (char **) realloc(graph->vertices, (graph->vertices_amount + 1) * sizeof(char *));

    if (!tmp)
        return _GRAPH_MEM__;
    else
        graph->vertices = tmp;

    // creating a dynamic copy of the vertex name

    graph->vertices[graph->vertices_amount] = strdup(vertex);

    if (!graph->vertices[graph->vertices_amount])
        return _GRAPH_MEM__;
    else
        graph->vertices_amount++;

    return _GRAPH_OK__;
}

graph_error_t graph_delete_vertex(struct graph *graph, const char *vertex)
{
    if (!graph || !vertex || !strlen(vertex))
        return _GRAPH_INCORRECT_ARG__;

    for (size_t i = 0; vertex[i] != '\0'; i++)
    {
        if (strchr(_GRAPH_FORBIDDEN_SEPARATORS__, vertex[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    if (graph_is_empty(graph))
        return _GRAPH_EMPTY__;

    if (!graph_has_vertex(graph, vertex))
        return _GRAPH_NOT_FOUND__;

    // removing edges from a given vertex

    for (size_t i = 0; i < graph->vertices_amount; i++)
    {
        graph_delete_edge(graph, vertex, graph->vertices[i]);
        graph_delete_edge(graph, graph->vertices[i], vertex);
    }

    // removing a pointer to a vertex using sequential displacement of elements

    for (size_t i = 0; i < graph->vertices_amount; i++)
    {
        if (!strcmp(vertex, graph->vertices[i]))
        {
            free(graph->vertices[i]);

            for (size_t j = i; j < graph->vertices_amount - 1; j++)
                graph->vertices[j] = graph->vertices[j + 1];

            break;
        }
    }

    graph->vertices = (char **) realloc(graph->vertices, (graph->vertices_amount - 1) * sizeof(char *));
    graph->vertices_amount--;

    return _GRAPH_OK__;
}

graph_error_t graph_add_edge(struct graph *graph, const char *start_vertex, const char *end_vertex, size_t edge_length)
{
    if (!graph || !start_vertex || !strlen(start_vertex) || strlen(start_vertex) > _STRING__ \
        || !end_vertex || !strlen(end_vertex) || strlen(end_vertex) > _STRING__)
        return _GRAPH_INCORRECT_ARG__;

    for (size_t i = 0; start_vertex[i] != '\0'; i++)
    {
        if (strchr(_GRAPH_FORBIDDEN_SEPARATORS__, start_vertex[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    for (size_t i = 0; end_vertex[i] != '\0'; i++)
    {
        if (strchr(_GRAPH_FORBIDDEN_SEPARATORS__, end_vertex[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    if (graph_has_edge(graph, start_vertex, end_vertex))
        return _GRAPH_EXIST__;

    struct edge edge_to_add = {0};

    strcpy(edge_to_add.start_vertex, start_vertex);
    strcpy(edge_to_add.end_vertex, end_vertex);
    edge_to_add.length = edge_length;

    struct edge *tmp = (struct edge *) realloc(graph->edges, (graph->edges_amount + 1) * sizeof(struct edge));
    if (!tmp)
        return _GRAPH_MEM__;
    else
    {
        graph->edges = tmp;
        graph->edges[graph->edges_amount] = edge_to_add;
        graph->edges_amount++;
    }

    if (!graph_has_vertex(graph, start_vertex))
        graph_add_vertex(graph, start_vertex);    

    if (!graph_has_vertex(graph, end_vertex))
        graph_add_vertex(graph, end_vertex);

    return _GRAPH_OK__;
}

graph_error_t graph_delete_edge(struct graph *graph, const char *start_vertex, const char *end_vertex)
{
    if (!graph || !start_vertex || !strlen(start_vertex) || strlen(start_vertex) > _STRING__ \
        || !end_vertex || !strlen(end_vertex) || strlen(end_vertex) > _STRING__)
        return _GRAPH_INCORRECT_ARG__;

    for (size_t i = 0; start_vertex[i] != '\0'; i++)
    {
        if (strchr(_GRAPH_FORBIDDEN_SEPARATORS__, start_vertex[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    for (size_t i = 0; end_vertex[i] != '\0'; i++)
    {
        if (strchr(_GRAPH_FORBIDDEN_SEPARATORS__, end_vertex[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    if (!graph_has_edge(graph, start_vertex, end_vertex))
        return _GRAPH_NOT_FOUND__;

    for (size_t i = 0; i < graph->edges_amount; i++)
    {
        struct edge current_edge = graph->edges[i];

        if (!strcmp(start_vertex, current_edge.start_vertex) && !strcmp(end_vertex, current_edge.end_vertex))
        {
            for (size_t j = i; j < graph->edges_amount - 1; j++)
                graph->edges[j] = graph->edges[j + 1];
            
            break;
        }
    }

    graph->edges = (struct edge *) realloc(graph->edges, (graph->edges_amount - 1) * sizeof(struct edge));
    graph->edges_amount--;

    return _GRAPH_OK__;
}

graph_error_t graph_to_dot(const struct graph *graph, const char *folder, const char *filename)
{
    if (!graph || !filename || (folder && !strlen(folder)) || !strlen(filename))
        return _GRAPH_INCORRECT_ARG__;

    for (size_t i = 0; folder && folder[i] != '\0'; i++)
    {
        if (strchr(_GRAPH_FORBIDDEN_SEPARATORS__, folder[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    for (size_t i = 0; filename[i] != '\0'; i++)
    {
        if (strchr(_GRAPH_FORBIDDEN_SEPARATORS__, filename[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    int rc = 0;

    FILE *dot_file = NULL;

    char buffer[_STRING__ + 1];

    // creating folder

    if (folder)
    {
        sprintf(buffer, "%s %s", "mkdir", folder);

        rc = system(buffer);
        if (rc)
            return _GRAPH_OS_ERROR__;    
    }

    // creating file

    folder ? sprintf(buffer, "%s/%s", folder, filename) : sprintf(buffer, "%s", filename);

    dot_file = fopen(buffer, "w");
    if (!dot_file)
    {
        if (folder)
        {
            sprintf(buffer, "%s %s", "rm -r -f", folder);
            system(buffer);    
        }
        
        return _GRAPH_MEM__;
    }
    
    // file processing

    fprintf(dot_file, "digraph picture {\n");

    // edges to dot

    for (size_t i = 0; i < graph->edges_amount; i++)
    {
        struct edge current_edge = graph->edges[i];

        fprintf(dot_file, "\"%s\" -> \"%s\" [label=  %zu];\n", current_edge.start_vertex, current_edge.end_vertex, current_edge.length);
    }

    // vertices (not in edges) to dot

    for (size_t i = 0; i < graph->vertices_amount; i++)
    {
        int vertex_drawed = 0;

        for (size_t j = 0; j < graph->vertices_amount && !vertex_drawed; j++)
        {
            if (graph_has_edge(graph, graph->vertices[i], graph->vertices[j]))
                vertex_drawed = 1;
            else if (graph_has_edge(graph, graph->vertices[j], graph->vertices[i]))
                vertex_drawed = 1;
        }

        if (!vertex_drawed)
            fprintf(dot_file, "\"%s\";\n", graph->vertices[i]);
    }

    fprintf(dot_file, "}");

    fclose(dot_file);

    return _GRAPH_OK__;
}

graph_error_t graph_show(const struct graph *graph)
{
    int rc = _GRAPH_OK__;

    if (!graph)
        rc = _GRAPH_INCORRECT_ARG__;

    if (rc == _GRAPH_OK__)
        rc = graph_to_dot(graph, ".graph_cash", "graph_dependencies.dot");

    if (rc == _GRAPH_OK__)
    {
        rc = system("dot -Tpng .graph_cash/graph_dependencies.dot -o graph.png");
        if (rc)
            return _GRAPH_OS_ERROR__;    
    }
    
    if (rc == _GRAPH_OK__)
    {
        #if defined(__WIN32__)
            system("mspaint graph.png");
        #elif defined(__linux__)
            system("eog graph.png");
        #else
            #error "Unsupported operating system!"
        #endif    
    }

    system("rm -f -r .graph_cash graph.png");

    return rc;
}

size_t graph_adjacency_list_size(const struct graph *graph, const char *vertex)
{
    if (!graph || !vertex)
        return 0;

    size_t adjacency_list_size = 0;

    for (size_t i = 0; i < graph->vertices_amount; i++)
    {
        if (graph_has_edge(graph, vertex, graph->vertices[i]))
            adjacency_list_size++;
    }

    return adjacency_list_size;
}

graph_error_t graph_adjacency_list_fill(const struct graph *graph, const char *vertex, int *adjacency_list)
{
    if (!graph || !vertex || !adjacency_list)
        return _GRAPH_INCORRECT_ARG__;

    for (size_t i = 0, k = 0; i < graph->edges_amount; i++)
    {
        struct edge current_edge = graph->edges[i];
        
        if (!strcmp(vertex, current_edge.start_vertex))
        {
            int end_vertex_finded = 0;

            for (size_t j = 0; j < graph->vertices_amount && !end_vertex_finded; j++)
            {
                if (!strcmp(graph->vertices[j], current_edge.end_vertex))
                {
                    adjacency_list[k++] = j;
                    end_vertex_finded = 1; 
                }
            }
        }

        printf("\n");
    }

    return _GRAPH_OK__;
}

struct matrix *graph_adjacency_matrix_create(const struct graph *graph)
{
    if (!graph)
        return NULL;

    // memory allocation for matrix

    struct matrix *matrix = malloc(sizeof(struct matrix));
    if (!matrix)
        return NULL;

    matrix->values = calloc(graph->vertices_amount, sizeof(int *));
    if (!matrix->values)
    {
        free(matrix);
        return NULL;
    }
    
    for (size_t i = 0; i < graph->vertices_amount; i++)
    {
        matrix->values[i] = malloc(sizeof(int) * graph->vertices_amount);
        if (!matrix->values[i])
        {
            for (size_t j = 0; j < i; j++)
                free(matrix->values[j]);
            free(matrix->values);
            free(matrix);

            return NULL;
        }
    }

    // matrix fill

    for (size_t i = 0; i < graph->vertices_amount; i++)
    {
        for (size_t j = 0; j < graph->vertices_amount; j++)
        {
            if (graph_has_edge(graph, graph->vertices[i], graph->vertices[j]))
            {
                int edge_is_finded = 0;

                for (size_t k = 0; k < graph->edges_amount && !edge_is_finded; k++)
                {
                    struct edge current_edge = graph->edges[k];

                    if (!strcmp(current_edge.start_vertex, graph->vertices[i]) \
                        && !strcmp(current_edge.end_vertex, graph->vertices[j]))
                    {
                        matrix->values[i][j] = current_edge.length;
                        edge_is_finded = 1;    
                    }
                }
            }
            else
                matrix->values[i][j] = INT_MAX;
        }
    }

    matrix->rows = graph->vertices_amount;
    matrix->columns = graph->vertices_amount;

    return matrix;
}

graph_error_t graph_adjacency_matrix_to_dot(const struct graph *graph, const struct matrix *adjacency_matrix, const char *folder, const char *filename)
{
    if (!adjacency_matrix || !folder || !filename)
        return _GRAPH_INCORRECT_ARG__;

    for (size_t i = 0; folder && folder[i] != '\0'; i++)
    {
        if (strchr(_GRAPH_FORBIDDEN_SEPARATORS__, folder[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    for (size_t i = 0; filename[i] != '\0'; i++)
    {
        if (strchr(_GRAPH_FORBIDDEN_SEPARATORS__, filename[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    int rc = 0;

    FILE *dot_file = NULL;

    char buffer[_STRING__ + 1];

    // creating folder

    if (folder)
    {
        sprintf(buffer, "%s %s", "mkdir", folder);

        rc = system(buffer);
        if (rc)
            return _GRAPH_OS_ERROR__;    
    }

    // creating file

    folder ? sprintf(buffer, "%s/%s", folder, filename) : sprintf(buffer, "%s", filename);

    dot_file = fopen(buffer, "w");
    if (!dot_file)
    {
        if (folder)
        {
            sprintf(buffer, "%s %s", "rm -r -f", folder);
            system(buffer);    
        }
        
        return _GRAPH_MEM__;
    }
    
    // creating dot file

    fprintf(dot_file, "digraph picture {\n");
    fprintf(dot_file, "  node [shape=plaintext]\n");
    fprintf(dot_file, "  \"Adjacency matrix\" [label=<\n");
    fprintf(dot_file, "    <table border='0' cellborder='1' cellspacing='0'>\n");


    fprintf(dot_file, "      <tr>\n");
    fprintf(dot_file, "      <td></td>\n");

    for (size_t j = 0; j < graph->vertices_amount; j++)
        fprintf(dot_file, "        <td>%s</td>\n", graph->vertices[j]);
    

    fprintf(dot_file, "      </tr>\n");

    for(size_t i = 0; i < adjacency_matrix->rows; i++) 
    {
        fprintf(dot_file, "      <tr>\n");
        fprintf(dot_file, "      <td>%s</td>\n", graph->vertices[i]);
        for (size_t j = 0; j < adjacency_matrix->columns; j++)
        {
            if (adjacency_matrix->values[i][j] != INT_MAX)
                fprintf(dot_file, "        <td>%d</td>\n", adjacency_matrix->values[i][j]);
            else
                fprintf(dot_file, "        <td>∞</td>\n"); 
        }
        
        fprintf(dot_file, "      </tr>\n");
    }


    fprintf(dot_file, "    </table>\n");
    fprintf(dot_file, "  >]\n");
    fprintf(dot_file, "}\n");

    fclose(dot_file);

    return _GRAPH_OK__;
}

graph_error_t graph_adjacency_matrix_show(const struct graph *graph, const struct matrix *adjacency_matrix)
{
    int rc = _GRAPH_OK__;

    if (!graph || !adjacency_matrix)
        rc = _GRAPH_INCORRECT_ARG__;

    if (rc == _GRAPH_OK__)
        rc = graph_adjacency_matrix_to_dot(graph, adjacency_matrix, ".graph_cash", "graph_adjacency_matrix_dependencies.dot");

    if (rc == _GRAPH_OK__)
    {
        rc = system("dot -Tpng .graph_cash/graph_adjacency_matrix_dependencies.dot -o graph_adjacency_matrix.png");
        if (rc)
            return _GRAPH_OS_ERROR__;    
    }
    
    if (rc == _GRAPH_OK__)
    {
        #if defined(__WIN32__)
            system("mspaint graph_adjacency_matrix.png");
        #elif defined(__linux__)
            system("eog graph_adjacency_matrix.png");
        #else
            #error "Unsupported operating system!"
        #endif    
    }

    system("rm -f -r .graph_cash graph_adjacency_matrix.png");

    return rc;
}

static inline void __graph_dfs_step(struct graph *graph, void (*vertex_processing)(char *vertex_name), int vertex_index, int *new)
{
    if (new[vertex_index] == 0)
        return;

    new[vertex_index] = 0;

    // defining an adjacency list

    size_t adjacent_vertices = graph_adjacency_list_size(graph, graph->vertices[vertex_index]);

    int adjacent_vertices_indexes[adjacent_vertices];

    graph_adjacency_list_fill(graph, graph->vertices[vertex_index], adjacent_vertices_indexes);

    // processing current vertex

    char vertex_copy[_STRING__ + 1];
    strcpy(vertex_copy, graph->vertices[vertex_index]);

    vertex_processing(graph->vertices[vertex_index]);

    for (size_t i = 0; i < graph->edges_amount; i++)
    {
        if (!strcmp(vertex_copy, graph->edges[i].start_vertex))
            strcpy(graph->edges[i].start_vertex, graph->vertices[vertex_index]);
        
        if (!strcmp(vertex_copy, graph->edges[i].end_vertex))
            strcpy(graph->edges[i].end_vertex, graph->vertices[vertex_index]);
    }
    
    // processing vertices from the adjacency list

    for (size_t i = 0, index = adjacent_vertices_indexes[i]; i < adjacent_vertices; i++, index = adjacent_vertices_indexes[i])
        __graph_dfs_step(graph, vertex_processing, index, new); 
}

void graph_dfs(struct graph *graph, void (*vertex_processing)(char *vertex_name))
{
    if (!graph || !vertex_processing)
        return;

    size_t vertices_amount = graph->vertices_amount;

    int new[vertices_amount];

    for (size_t i = 0; i < vertices_amount; i++)
        new[i] = 1;

    for (size_t i = 0; i < vertices_amount; i++)
        __graph_dfs_step(graph, vertex_processing, i, new);
}

struct matrix *graph_floyd_warshall(const struct graph *graph)
{
    if (!graph)
        return NULL;

    struct matrix *matrix = graph_adjacency_matrix_create(graph);
    if (!matrix)
        return NULL;

    for (size_t i = 0; i < graph->vertices_amount; i++)
        matrix->values[i][i] = 0;

    for (size_t i = 0; i < graph->vertices_amount; i++)
    {
        for (size_t u = 0; u < graph->vertices_amount; u++)
        {
            for (size_t v = 0; v < graph->vertices_amount; v++)
            {
                if (graph_has_edge(graph, graph->vertices[u], graph->vertices[i]) \
                    && graph_has_edge(graph, graph->vertices[i], graph->vertices[v]))
                {
                    if (matrix->values[u][v] != 0)
                        matrix->values[u][v] = matrix->values[u][v] > (matrix->values[u][i] + matrix->values[i][v]) ? \
                            (matrix->values[u][i] + matrix->values[i][v]) : matrix->values[u][v];
                    else
                    {
                        if (strcmp(graph->vertices[u], graph->vertices[v]) && matrix->values[u][i] && matrix->values[i][v])
                            matrix->values[u][v] = matrix->values[u][i] + matrix->values[i][v];
                    }
                }
            }
        }
    }

    return matrix;
}

void graph_adjacency_matrix_free(struct matrix *adjacency_matrix)
{
    if (adjacency_matrix)
    {
        for (size_t i = 0; i < adjacency_matrix->rows; i++)
            free(adjacency_matrix->values[i]);
        free(adjacency_matrix->values);
    }

    free(adjacency_matrix);
}

void graph_free(struct graph *graph)
{
    for (size_t i = 0; i < graph->vertices_amount; i++)
        free(graph->vertices[i]);

    free(graph->vertices);
    free(graph->edges);
}