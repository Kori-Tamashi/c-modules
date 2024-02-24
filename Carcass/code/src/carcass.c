#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "carcass.h"

#define __CARCASS_PI 3.14159265359

void carcass_free(struct carcass *carcass)
{
    if (carcass)
        free(carcass->vertex_array);
}

size_t carcass_get_vertex_amount(FILE *file)
{
    if (!file) 
        return 0;

    size_t vertex_count = 0; // Инициализация счетчика вершин

    char line[__CARCASS_STRING]; // Буфер для чтения строк из файла

    // Находим начало данных о вершинах
    while (fgets(line, __CARCASS_STRING, file))
    {
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0'; // Убираем символ новой строки

        if (line[strlen(line) - 1] == '\r')
            line[strlen(line) - 1] = '\0'; // Убираем символ возврата каретки

        if (!strcmp(line, __CARCASS_MARKER)) // Прекращаем при достижении маркера
            break;
    }

    double x, y, z;

    // Считаем вершины
    while (fscanf(file, "%lf%lf%lf", &x, &y, &z) == 3)
        vertex_count++;

    return vertex_count; 
}

carcass_error_t carcass_scan(FILE *file, struct carcass *carcass)
{
    if (!file || !carcass)
        return CARCASS_INCORRECT_ARGUMENTS; 

    size_t vertex_amount = carcass_get_vertex_amount(file);  // Получение количества вершин

    rewind(file);

    if (!vertex_amount)
        return CARCASS_INCORRECT_INPUT;  // Проверка на количество вершин

    char line[__CARCASS_STRING];

    // Находим начало данных о вершинах
    while (fgets(line, __CARCASS_STRING, file))
    {
        line[strlen(line) - 1] = '\0'; // Убираем символ новой строки

        if (!strcmp(line, __CARCASS_MARKER)) // Прекращаем при достижении маркера
            break;
    }

    // Чтение данных вершин
    for (size_t i = 0; i < vertex_amount; i++)
    {
        struct vertex new_vertex;

        if (vertex_scan(file, &new_vertex) != VERTEX_OK)
            return CARCASS_INCORRECT_INPUT;

        carcass_add_vertex(carcass, new_vertex);
    }

    return CARCASS_OK;
}

carcass_error_t carcass_add_vertex(struct carcass *carcass, struct vertex vertex)
{
    // Проверяем, что указатель на каркас не является NULL
    if (!carcass)
        return CARCASS_INCORRECT_ARGUMENTS;

    // Пытаемся перераспределить память для массива вершин
    struct vertex *tmp = realloc(carcass->vertex_array, 
                                (carcass->vertex_amount + 1) * sizeof(struct vertex));

    // Проверяем, было ли успешно перераспределение памяти
    if (!tmp)
        return CARCASS_MEMORY;
    else
        carcass->vertex_array = tmp;

    // Добавляем новую вершину в массив вершин и увеличиваем количество вершин
    carcass->vertex_array[carcass->vertex_amount] = vertex;
    carcass->vertex_amount++;

    return CARCASS_OK;
}

void carcass_print(FILE *file, const struct carcass *carcass)
{
    if (!file || !carcass)
        return;

    fprintf(file, "%s\n", __CARCASS_MARKER);

    for (size_t i = 0; i < carcass->vertex_amount; i++)
        vertex_print(file, &carcass->vertex_array[i]);
}

void carcass_move(struct carcass *carcass, double x_move, double y_move, double z_move)
{
    // Проверяем, что carcass не является нулевым
    if (!carcass)
        return;

    // Проходим по каждой вершине и перемещаем их
    for (size_t i = 0; i < carcass->vertex_amount; i++)
    {
        // Перемещаем вершину по оси x
        carcass->vertex_array[i].x += x_move;
        
        // Перемещаем вершину по оси y
        carcass->vertex_array[i].y += y_move;
        
        // Перемещаем вершину по оси z
        carcass->vertex_array[i].z += z_move;
    }
}

/**
 * Поворачивает точку (x1, x2) вокруг заданного центра (x1_center, x2_center) на указанный угол.
 *
 * @param x1 Указатель на координату x точки
 * @param x2 Указатель на координату y точки
 * @param angle Угол поворота в градусах
 * @param x1_center Координата x центра поворота
 * @param x2_center Координата y центра поворота
 */
static inline void __rotate_point(double *x1, double *x2, double angle, double x1_center, double x2_center)
{
    double degree = angle * (__CARCASS_PI / 180.0);

    double x1_value = *x1;
    double x2_value = *x2;

    *x1 = x1_center + (x1_value - x1_center) * cos(degree) + (x2_value - x2_center) * sin(degree);
    *x2 = x2_center - (x1_value - x1_center) * sin(degree) + (x2_value - x2_center) * cos(degree);
}

void carcass_rotate(struct carcass *carcass, double angle, struct vertex *center, enum plane plane)
{
    if (!carcass)
        return;

    switch (plane)
    {
    case XY:
        {
            for (size_t i = 0; i < carcass->vertex_amount; i++)
                __rotate_point(&carcass->vertex_array[i].x, \
                                        &carcass->vertex_array[i].y, angle, center->x, center->y);
        }
        break;
    case YZ:
        {
            for (size_t i = 0; i < carcass->vertex_amount; i++)
                __rotate_point(&carcass->vertex_array[i].y, \
                                        &carcass->vertex_array[i].z, angle, center->y, center->z);
        }
        break;
    case XZ:
        {
            for (size_t i = 0; i < carcass->vertex_amount; i++)
                __rotate_point(&carcass->vertex_array[i].x, \
                                        &carcass->vertex_array[i].z, angle, center->x, center->z);
        }
        break;
    default:
        break;
    }
}

/**
 * Масштабирует точку относительно точки (x1_center, x2_center).
 * 
 * @param x1 - указатель на координату x
 * @param x2 - указатель на координату y
 * @param koef - коэффициент масштабирования
 * @param x1_center - координата x центра масштабирования
 * @param x2_center - координата y центра масштабирования
 */
static inline void __scale_point(double *x1, double *x2, double koef, double x1_center, double x2_center)
{
    *x1 = koef * (*x1) + x1_center * (1 - koef);
    *x2 = koef * (*x2) + x2_center * (1 - koef);
}

void carcass_scale(struct carcass *carcass, double koef, struct vertex *center)
{
    // Проверяем, является ли каркас допустимым
    if (!carcass)
        return;

    // Проходим через каждую вершину
    for (size_t i = 0; i < carcass->vertex_amount; i++)
    {
        // Извлекаем координаты x, y, z
        double x = carcass->vertex_array[i].x;
        double y = carcass->vertex_array[i].y;
        double z = carcass->vertex_array[i].z;

        // Масштабируем точку в направлении x
        __scale_point(&carcass->vertex_array[i].x, &y, koef, center->x, center->y);

        // Масштабируем точку в направлении y
        __scale_point(&carcass->vertex_array[i].y, &z, koef, center->y, center->z);

        // Масштабируем точку в направлении z
        __scale_point(&x, &carcass->vertex_array[i].z, koef, center->x, center->z);
    }
}