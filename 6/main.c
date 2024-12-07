#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAP 25
#define CAP_INCREMENT 5

#define COLUMNS 130
#define ROWS

typedef struct
{
    unsigned long size;
    unsigned long capacity;
    int *items;
} Vec;

typedef enum
{
    GUARD = '^',
    OBSTACLE = '#',
    SPACE = '.',
    MARKED = 'x',
} Obj;

typedef struct
{
    int row;
    int col;
} Pos;

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN
} Direction;

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
int travel(Pos *, Matrix *, Direction, int);

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
    Pos guard = {0, 0};
    unsigned long col = 0, row = 0;
    while ((io = fgetc(fp)) && io != EOF)
    {
        if (!isspace(io))
        {
            append(vec, io);
        }

        if (io == '^')
        {
            guard.row = row;
            guard.col = col;
        }

        if (io == '\n')
        {
            row++;
            col = 0;

            appendRowVector(mat, vec);
            vec->size = 0;
        }
        else
        {
            col++;
        }
    }
    result = travel(&guard, mat, UP, 0);
    delete_vec(vec);
    delete_matrix(mat);
    return result;
}

Direction turn_right(Direction dir)
{
    switch (dir)
    {
    case LEFT:
        return UP;
    case RIGHT:
        return DOWN;
    case UP:
        return RIGHT;
    case DOWN:
        return LEFT;
        break;
    }

    return UP;
}

void updateLocation(Pos *pos, Direction dir)
{
    switch (dir)
    {
    case LEFT:
        pos->col -= 1;
        break;
    case RIGHT:
        pos->col += 1;
        break;
    case UP:
        pos->row -= 1;
        break;
    case DOWN:
        pos->row += 1;
        break;
    }
}

void undo_travel(Pos *pos, Direction dir)
{
    switch (dir)
    {
    case LEFT:
        pos->col += 1;
        break;
    case RIGHT:
        pos->col -= 1;
        break;
    case UP:
        pos->row += 1;
        break;
    case DOWN:
        pos->row -= 1;
        break;
    }
}

int travel(Pos *pos, Matrix *m, Direction dir, int c)
{
    if ((pos->row >= m->rows) || (pos->row < 0) || (pos->col >= m->columns) ||
        (pos->col < 0))
    {
        return c;
    }

    int count = c;

    if (m->items[pos->row][pos->col] == OBSTACLE)
    {
        undo_travel(pos, dir);
        dir = turn_right(dir);
    }
    else
    {
        if (m->items[pos->row][pos->col] != MARKED)
        {
            m->items[pos->row][pos->col] = MARKED;
            count++;
        }
    }

    updateLocation(pos, dir);
    return travel(pos, m, dir, count);
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
