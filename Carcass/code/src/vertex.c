#include <stdlib.h>
#include "vertex.h"

vertex_error_t vertex_scan(FILE *stream, struct vertex *vertex)
{
    if (!stream || !vertex)
        return VERTEX_INCORRECT_ARGUMENTS;

    if (fscanf(stream, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z) != 3)
        return VERTEX_INCORRECT_INPUT;

    return VERTEX_OK;
}

void vertex_print(FILE *stream, const struct vertex *vertex)
{
    if (!stream || !vertex)
        return;

    fprintf(stream, "%lf %lf %lf", vertex->x, vertex->y, vertex->z);
}