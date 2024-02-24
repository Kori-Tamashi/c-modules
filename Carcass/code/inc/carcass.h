#ifndef CARCASS__
#define CARCASS__

#include <stdio.h>
#include "vertex.h"

/**
 * @def CARCASS_OK
 * @brief Код успешного выполнения операции.
 */
#define CARCASS_OK 0

/**
 * @def CARCASS_INCORRECT_ARGUMENTS
 * @brief Код ошибки - некорректные аргументы.
 */
#define CARCASS_INCORRECT_ARGUMENTS -1

/**
 * @def CARCASS_MEMORY
 * @brief Код ошибки - ошибка выделения памяти.
 */
#define CARCASS_MEMORY -2

/**
 * @def CARCASS_INCORRECT_INPUT
 * @brief Код ошибки - некорректный ввод.
 */
#define CARCASS_INCORRECT_INPUT -3

/**
 * Maximum length of a carcass config string.
 */
#define __CARCASS_STRING 64

#define __CARCASS_MARKER "@CARCASS"

typedef int carcass_error_t;

/**
 * Перечисление плоскостей
 */
enum plane 
{
    XY, // Плоскость XY
    XZ, // Плоскость XZ
    YZ  // Плоскость YZ
};

/**
 * Перечисление функций обработки каркаса
 */
enum carcass_processing_function 
{
    MOVE, // Перемещение каркаса
    ROTATE, // Вращение каркаса
    SCALE // Масштабирование каркаса
};

/**
 * @struct carcass
 * @brief Представляет каркас в трехмерном пространстве.
 *
 * Эта структура используется для представления каркаса с вершинами и методами для их обработки.
 */
struct carcass
{
    struct vertex *vertex_array; // Массив вершин каркаса
    size_t vertex_amount; // Количество вершин каркаса
};

/**
 * @brief Инициализирует каркас нулевыми значениями.
 *
 * @param name Имя каркаса для инициализации.
 */
#define CARCASS_INIT(name) \
    struct carcass name = (struct carcass) {0}

/**
 * @brief Освобождает память, занятую каркасом.
 *
 * @param carcass Указатель на каркас.
 */
void carcass_free(struct carcass *carcass);

/**
 * @brief Получает количество вершин каркаса из файла.
 *
 * @param f Указатель на файл с информацией о каркасе.
 * @return Количество вершин каркаса
 */
size_t carcass_get_vertex_amount(FILE *f);

/**
 * @brief Считывает информацию о каркасе из потока.
 *
 * @param file Поток для считывания информации о каркасе.
 * @param carcass Указатель на каркас.
 * @return Код ошибки
 */
carcass_error_t carcass_scan(FILE *file, struct carcass *carcass);

/**
 * @brief Выводит информацию о каркасе в поток.
 *
 * @param file Поток для вывода информации о каркасе.
 * @param carcass Указатель на каркас.
 */
void carcass_print(FILE *file, const struct carcass *carcass);

/**
 * @brief Добавляет новую вершину каркаса.
 *
 * @param carcass Указатель на каркас.
 * @param vertex Вершина для добавления.
 * @return Код ошибки
 */
carcass_error_t carcass_add_vertex(struct carcass *carcass, struct vertex vertex);

/**
 * @brief Перемещает каркас на заданное расстояние по координатам.
 *
 * @param carcass Указатель на каркас.
 * @param x_move Расстояние по оси X.
 * @param y_move Расстояние по оси Y.
 * @param z_move Расстояние по оси Z.
 */
void carcass_move(struct carcass *carcass, double x_move, double y_move, double z_move);

/**
 * Поворачивает каркас на заданный угол в указанной плоскости.
 *
 * @param carcass Указатель на структуру каркаса
 * @param angle Угол поворота
 * @param x Координата X центра вращения
 * @param y Координата Y центра вращения
 * @param z Координата Z центра вращения
 * @param plane Плоскость в которой осуществляется вращение
 */
void carcass_rotate(struct carcass *carcass, double angle, struct vertex *center, enum plane plane);

/**
 * @brief Масштабирует каркас относительно указанной точки.
 *
 * @param carcass Указатель на каркас.
 * @param koef Коэффициент масштабирования.
 * @param x X-координата точки масштабирования.
 * @param y Y-координата точки масштабирования.
 * @param z Z-координата точки масштабирования.
 */
void carcass_scale(struct carcass *carcass, double koef, struct vertex *center);

#endif // CARCASS__