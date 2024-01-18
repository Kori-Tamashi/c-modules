#include <stdlib.h>
#include <string.h>
#include "graph.h"

void graph_initialize(struct graph *graph)
{   
    *graph = (struct graph) {0};
}

int graph_is_empty(struct graph *graph)
{
    if (graph)
        return graph->vertices_amount == 0;
    return 1;
}

int graph_has_vertex(struct graph *graph, const char *vertex)
{
    if (graph && vertex)
    {
        if (!graph_is_empty(graph))
        {
            for (int i = 0; i < graph->vertices_amount; i++)
            {
                if (!strcmp(vertex, graph->vertices[i]))
                    return 1;
            }
        }
    }

    return 0;
}

int graph_has_edge(struct graph *graph, const char *start_vertex, const char *end_vertex)
{
    if (graph && start_vertex && strlen(start_vertex) && end_vertex && strlen(end_vertex))
    {
        if (!graph_is_empty(graph))
        {
            if (graph_has_vertex(graph, start_vertex) && graph_has_vertex(graph, end_vertex))
            {
                for (int i = 0; i < graph->edges_amount; i++)
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

    for (int i = 0; vertex[i] != '\0'; i++)
    {
        if (strchr(FORBIDDEN_SEPARATORS, vertex[i]))
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

    if (!graph->vertices[graph->vertices_amount + 1])
        return _GRAPH_MEM__;
    else
        graph->vertices_amount++;

    return _GRAPH_OK__;
}

graph_error_t graph_delete_vertex(struct graph *graph, const char *vertex)
{
    if (!graph || !vertex || !strlen(vertex))
        return _GRAPH_INCORRECT_ARG__;

    for (int i = 0; vertex[i] != '\0'; i++)
    {
        if (strchr(FORBIDDEN_SEPARATORS, vertex[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    if (graph_is_empty(graph))
        return _GRAPH_EMPTY__;

    if (!graph_has_vertex(graph, vertex))
        return _GRAPH_NOT_FOUND__;

    // removing edges from a given vertex

    for (int i = 0; i < graph->vertices_amount; i++)
    {
        graph_delete_edge(graph, vertex, graph->vertices[i]);
        graph_delete_edge(graph, graph->vertices[i], vertex);
    }

    // removing a pointer to a vertex using sequential displacement of elements

    for (int i = 0; i < graph->vertices_amount; i++)
    {
        if (!strcmp(vertex, graph->vertices[i]))
        {
            free(graph->vertices[i]);

            for (int j = i; j < graph->vertices_amount - 1; j++)
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

    for (int i = 0; start_vertex[i] != '\0'; i++)
    {
        if (strchr(FORBIDDEN_SEPARATORS, start_vertex[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    for (int i = 0; end_vertex[i] != '\0'; i++)
    {
        if (strchr(FORBIDDEN_SEPARATORS, end_vertex[i]))
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

    for (int i = 0; start_vertex[i] != '\0'; i++)
    {
        if (strchr(FORBIDDEN_SEPARATORS, start_vertex[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    for (int i = 0; end_vertex[i] != '\0'; i++)
    {
        if (strchr(FORBIDDEN_SEPARATORS, end_vertex[i]))
            return _GRAPH_INCORRECT_ARG__;
    }

    if (!graph_has_edge(graph, start_vertex, end_vertex))
        return _GRAPH_NOT_FOUND__;

    for (int i = 0; i < graph->edges_amount; i++)
    {
        struct edge current_edge = graph->edges[i];

        if (!strcmp(start_vertex, current_edge.start_vertex) && !strcmp(end_vertex, current_edge.end_vertex))
        {
            for (int j = i; j < graph->edges_amount - 1; j++)
                graph->edges[j] = graph->edges[j + 1];
            
            break;
        }
    }

    graph->edges = (struct edge *) realloc(graph->edges, (graph->edges_amount - 1) * sizeof(struct edge));
    graph->edges_amount--;

    return _GRAPH_OK__;
}

graph_error_t graph_show(struct graph *graph)
{
    int rc = _GRAPH_OK__;

    FILE *dot_file = NULL;

    if (!graph)
        rc = _GRAPH_INCORRECT_ARG__;
    
    if (rc == _GRAPH_OK__)
    {
        rc = system("mkdir .graph_tmp");
        if (rc)
            rc = _GRAPH_OS_ERROR__;    
    }
    
    if (rc == _GRAPH_OK__)
    {
        dot_file = fopen(".graph_tmp/dot_file.dot", "w");
        if (!dot_file)
            rc = _GRAPH_MEM__;
    }

    if (rc == _GRAPH_OK__)
    {
        fprintf(dot_file, "digraph picture {\n");

        // edges to dot

        for (int i = 0; i < graph->edges_amount; i++)
        {
            struct edge current_edge = graph->edges[i];

            fprintf(dot_file, "\"%s\" -> \"%s\" [label=  %zu];\n", current_edge.start_vertex, current_edge.end_vertex, current_edge.length);
        }

        // vertices (not in edges) to dot

        for (int i = 0; i < graph->vertices_amount; i++)
        {
            int vertex_drawed = 0;

            for (int j = 0; j < graph->vertices_amount && !vertex_drawed; j++)
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

        // create image of graph

        rc = system("dot -Tpng .graph_tmp/dot_file.dot -o graph.png");
        if (rc)
            rc = _GRAPH_OS_ERROR__;
    }
    
    #if defined(__WIN32__)
        system("mspaint graph.png");
    #elif defined(__linux__)
        system("eog graph.png");
    #else
        #error "Unsupported operating system!"
    #endif

    system("rm -f -r .graph_tmp graph.png");

    return rc;
}

void graph_free(struct graph *graph)
{
    for (int i = 0; i < graph->vertices_amount; i++)
        free(graph->vertices[i]);

    free(graph->vertices);
    free(graph->edges);
}
