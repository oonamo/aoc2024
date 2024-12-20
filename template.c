#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAP 25
#define CAP_INCREMENT 5

typedef struct
{
    unsigned long size;
    unsigned long capacity;
    int *items;
} Vec;

typedef struct
{
    unsigned long columns;
    unsigned long rows;
    int **items;
} Matrix;

Vec *n_vec();
void delete_vec(Vec *);
void append(Vec *, int);
void sort_vec(Vec *);

Matrix *n_matrix(int rows, int cols);
void appendRowVector(Matrix *m, Vec *v);
void delete_matrix(Matrix *m);

int compare(const void *a, const void *b);

void consume_line(FILE *);

int solve(char *file);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("USAGE: %s [input]", argv[0]);
        exit(EXIT_FAILURE);
    }

    int result = solve(argv[1]);

    printf("Result: %d", result);
}

int solve(char *file)
{
    FILE *fp = fopen(file, "r");
    Vec *vec = NULL;
    double result = 0;

    if (fp == NULL)
    {
        printf("Failed to open file '%s'\n", file);
        exit(EXIT_FAILURE);
    }

    vec = n_vec();

    char io;
    while ((io = fgetc(fp)) && io != EOF)
    {
    }

    delete_vec(vec);
    return result;
}

void consume_line(FILE *fp)
{
    char io;
    while ((io = fgetc(fp)) != '\n' && io != EOF)
        ;
}

Vec *n_vec()
{
    Vec *v = malloc(sizeof(Vec));
    v->capacity = 0;
    v->size = 0;
    v->items = NULL;
    return v;
}

int compare(const void *a, const void *b) { return *(int *)a - *(int *)b; }
void sort_vec(Vec *vec) { qsort(vec->items, vec->size, sizeof(int), compare); }

void delete_vec(Vec *vec)
{
    if (vec == NULL)
        return;

    if (vec->items != NULL)
    {
        free(vec->items);
    }

    free(vec);
}

void append(Vec *vec, int val)
{
    if (vec->size <= vec->capacity)
    {
        if (vec->capacity == 0)
        {
            vec->capacity = INIT_CAP;
            vec->items = malloc(sizeof(int) * vec->capacity);
        }
        else
        {
            vec->capacity += CAP_INCREMENT;
            vec->items = realloc(vec->items, sizeof(int) * vec->capacity);
        }
    }
    vec->items[vec->size++] = val;
}

Matrix *n_matrix(int rows, int cols)
{
    Matrix *v = malloc(sizeof(Matrix));
    v->rows = rows;
    v->columns = cols;
    if (v->rows > 0)
    {
        v->items = (int **)malloc(sizeof(int *) * (v->rows));
        for (int i = 0; i < cols; i++)
        {
            v->items[i] = malloc(sizeof(int) * cols);
        }
    }
    else
    {
        v->items = NULL;
    }
    return v;
}

void appendRowVector(Matrix *m, Vec *v)
{
    m->rows++;
    m->items = (int **)realloc(m->items, sizeof(int *) * m->rows);
    m->items[m->rows - 1] = malloc(sizeof(int) * m->columns);
    for (unsigned long i = 0; i < m->columns; i++)
    {
        if (i > v->size)
        {
            m->items[m->rows - 1][i] = '.';
        }
        else
        {
            m->items[m->rows - 1][i] = v->items[i];
        }
    }
}

void delete_matrix(Matrix *m)
{
    if (m == NULL)
        return;
    if (m->items != NULL)
    {
        for (unsigned long i = 0; i < m->rows; i++)
        {
            free(m->items[i]);
        }

        free(m->items);
    }
}
