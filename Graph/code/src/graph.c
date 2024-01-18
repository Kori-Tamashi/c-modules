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
    if (graph && start_vertex && end_vertex)
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

void graph_free(struct graph *graph)
{
    for (int i = 0; i < graph->vertices_amount; i++)
        free(graph->vertices[i]);

    free(graph->vertices);
    free(graph->edges);
}
