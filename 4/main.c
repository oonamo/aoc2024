#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAP 25
#define CAP_INCREMENT 5

#define ROWS 140
#define COLUMNS 140

#define XMAS_LEN 4
#define XMAS "XMAS"

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
int compare(const void *a, const void *b);
void vec_clear(Vec *);
int vec_pop(Vec *);

bool any_ok(Matrix *, int, int);

Matrix *n_matrix(int rows, int cols);
void appendRowVector(Matrix *m, Vec *v);
void delete_matrix(Matrix *m);

bool row_wise(Matrix *m, unsigned long r, unsigned long c);
bool row_wise_reverse(Matrix *m, unsigned long r, unsigned long c);
bool column_wise(Matrix *m, unsigned long r, unsigned long c);
bool column_wise_reverse(Matrix *m, unsigned long r, unsigned long c);
bool right_diagnoal_wise(Matrix *m, unsigned long r, unsigned long c);
bool left_diagnoal_wise(Matrix *m, unsigned long r, unsigned long c);
bool right_diagnoal_reverse(Matrix *m, unsigned long r, unsigned long c);
bool left_diagnoal_reverse(Matrix *m, unsigned long r, unsigned long c);

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
    Matrix *mat = NULL;
    double result = 0;

    if (fp == NULL)
    {
        printf("Failed to open file '%s'\n", file);
        exit(EXIT_FAILURE);
    }

    vec = n_vec();
    mat = n_matrix(0, COLUMNS);

    char io;
    while ((io = fgetc(fp)) && io != EOF)
    {
        if (io != 'X' && io != 'M' && io != 'A' && io != 'X' && io != 'S')
        {
            append(vec, '.');
        }
        else if (!isspace(io))
        {
            append(vec, io);
        }

        if (io == '\n')
        {
            appendRowVector(mat, vec);
            vec_clear(vec);
        }
    }

    for (unsigned long i = 0; i < mat->rows; i++)
    {
        bool any_ok = false;
        for (unsigned long j = 0; j < mat->columns; j++)
        {
            if (mat->items[i][j] != 'X')
                continue;
            bool res;
            result += row_wise(mat, i, j);
            result += row_wise_reverse(mat, i, j);
            result += column_wise(mat, i, j);
            result += column_wise_reverse(mat, i, j);
            result += right_diagnoal_wise(mat, i, j);
            result += left_diagnoal_wise(mat, i, j);
            result += right_diagnoal_reverse(mat, i, j);
            result += left_diagnoal_reverse(mat, i, j);
        }
    }

    printf("      ");
    for (unsigned long i = 0; i < mat->columns; i++)
    {
        printf("%lu ", i);
    }
    printf("\n");
    for (unsigned long i = 0; i < mat->rows; i++)
    {
        printf("(%lu) [ ", i);
        for (unsigned long j = 0; j < mat->columns; j++)
        {
            printf("%c ", mat->items[i][j]);
        }
        printf("]\n");
    }

    delete_vec(vec);
    delete_matrix(mat);
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

    printf("Reallocating!\n");
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

int vec_pop(Vec *vec) { return vec->items[vec->size--]; }
void vec_clear(Vec *vec) { vec->size = 0; }

bool row_wise(Matrix *m, unsigned long r, unsigned long c)
{
    if (r + XMAS_LEN > m->rows)
        return false;
    for (int i = 0; i < XMAS_LEN; i++)
    {
        if (XMAS[i] != m->items[r + i][c])
        {
            return false;
        }
    }
    return true;
}

bool row_wise_reverse(Matrix *m, unsigned long r, unsigned long c)
{
    if (r < XMAS_LEN - 1)
        return false;

    for (int i = XMAS_LEN - 1; i >= 0; i--)
    {
        if (XMAS[i] != m->items[r - i][c])
        {
            return false;
        }
    }

    return true;
}

bool column_wise(Matrix *m, unsigned long r, unsigned long c)
{
    if (c + XMAS_LEN > m->columns)
        return false;
    for (int i = 0; i < XMAS_LEN; i++)
    {
        if (XMAS[i] != m->items[r][c + i])
        {
            return false;
        }
    }

    return true;
}

bool column_wise_reverse(Matrix *m, unsigned long r, unsigned long c)
{
    if (c < XMAS_LEN - 1)
        return false;
    for (int i = XMAS_LEN - 1; i >= 0; i--)
    {
        if (XMAS[i] != m->items[r][c - i])
        {
            return false;
        }
    }
    return true;
}

bool right_diagnoal_wise(Matrix *m, unsigned long r, unsigned long c)
{
    if (r + XMAS_LEN > m->rows || c + XMAS_LEN > m->columns)
        return false;
    for (int i = 0; i < XMAS_LEN; i++)
    {
        if (XMAS[i] != m->items[r + i][c + i])
        {
            return false;
        }
    }

    return true;
}

bool left_diagnoal_wise(Matrix *m, unsigned long r, unsigned long c)
{
    if (r + XMAS_LEN > m->rows || c < XMAS_LEN - 1)
        return false;

    for (int i = XMAS_LEN - 1; i >= 0; i--)
    {
        if (XMAS[i] != m->items[r + i][c - i])
        {
            return false;
        }
    }
    return true;
}

bool right_diagnoal_reverse(Matrix *m, unsigned long r, unsigned long c)
{
    if (r < XMAS_LEN - 1 || c + XMAS_LEN > m->columns)
        return false;
    for (int i = XMAS_LEN - 1; i >= 0; i--)
    {
        if (XMAS[i] != m->items[r - i][c + i])
        {
            return false;
        }
    }
    return true;
}

bool left_diagnoal_reverse(Matrix *m, unsigned long r, unsigned long c)
{
    if (r < XMAS_LEN - 1 || c < XMAS_LEN - 1)
        return false;
    for (int i = XMAS_LEN - 1; i >= 0; i--)
    {
        if (XMAS[i] != m->items[r - i][c - i])
        {
            return false;
        }
    }
    return true;
}
