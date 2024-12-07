#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAP 25
#define CAP_INCREMENT 5

#define COLUMNS 130

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

typedef struct
{
    Direction dir;
    Pos pos;
} GuardState;

typedef struct
{
    unsigned long size;
    unsigned long capacity;
    GuardState *states;
} state_t;

Vec *n_vec();
void delete_vec(Vec *);
void append(Vec *, int);
void sort_vec(Vec *);
void travel(Pos *, Matrix *, Direction, state_t *);
int brute_force(Pos *, Matrix *);

Matrix *n_matrix(int rows, int cols);
void appendRowVector(Matrix *m, Vec *v);
void delete_matrix(Matrix *m);

void placeObstacle(Pos *pos, Matrix *m);
bool guard_is_stuck(Pos *pos, Pos *starting, Matrix *m);

int compare(const void *a, const void *b);
void undo_travel(Pos *pos, Direction dir);
void updateLocation(Pos *pos, Direction dir);
Direction turn_right(Direction dir);

state_t *n_state();
void delete_state(state_t *);
void append_to_state(state_t *vec, Pos *pos, Direction dir);
bool state_is_unique(state_t *vec, Pos *pos, Direction dir);
bool state_pos_is_unique(state_t *vec, Pos *pos);

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
    result = brute_force(&guard, mat);

    delete_vec(vec);
    delete_matrix(mat);

    return result;
}

int brute_force(Pos *pos, Matrix *m)
{
    int count = 0;
    Pos starting = {
        .col = pos->col,
        .row = pos->row,
    };

    state_t *s = n_state();
    travel(pos, m, UP, s);
    printf("part1: %lu\n", s->size);

    for (unsigned long i = 0; i < s->size; i++)
    {
        GuardState state = s->states[i];

        // Don't place where their already is an obstacle
        if (m->items[state.pos.row][state.pos.col] == OBSTACLE)
        {
            continue;
        }

        char tmp = m->items[state.pos.row][state.pos.col];
        m->items[state.pos.row][state.pos.col] = OBSTACLE;

        if (guard_is_stuck(pos, &starting, m))
        {
            count++;
        }

        pos->row = starting.row;
        pos->col = starting.col;

        m->items[state.pos.row][state.pos.col] = tmp;
    }

    delete_state(s);
    return count;
}

void placeObstacle(Pos *pos, Matrix *m)
{
    m->items[pos->row][pos->col] = OBSTACLE;
}

bool guard_is_stuck(Pos *pos, Pos *starting, Matrix *m)
{
    Direction dir = UP;
    state_t *state = n_state();

    bool is_stuck = false;

    while (true)
    {
        if ((pos->row >= m->rows) || (pos->row < 0) ||
            (pos->col >= m->columns) || (pos->col < 0))
        {
            break;
        }

        if (m->items[pos->row][pos->col] == OBSTACLE)
        {
            if (!state_is_unique(state, pos, dir))
            {
                is_stuck = true;
                break;
            }
            append_to_state(state, pos, dir);

            undo_travel(pos, dir);
            dir = turn_right(dir);
        }

        updateLocation(pos, dir);
    }

    delete_state(state);
    return is_stuck;
}

Direction turn_right(Direction dir)
{
    switch (dir)
    {
    case LEFT:
        return UP;
        break;
    case RIGHT:
        return DOWN;
        break;
    case UP:
        return RIGHT;
        break;
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

void travel(Pos *pos, Matrix *m, Direction dir, state_t *s)
{
    if ((pos->row >= m->rows) || (pos->row < 0) || (pos->col >= m->columns) ||
        (pos->col < 0))
    {
        return;
    }

    if (m->items[pos->row][pos->col] == OBSTACLE)
    {
        undo_travel(pos, dir);
        dir = turn_right(dir);
    }
    else
    {
        if (state_pos_is_unique(s, pos))
        {
            append_to_state(s, pos, dir);
        }
    }

    updateLocation(pos, dir);
    travel(pos, m, dir, s);
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

state_t *n_state()
{
    state_t *s = malloc(sizeof(state_t));
    s->capacity = 0;
    s->size = 0;
    s->states = NULL;
    return s;
}

void delete_state(state_t *s)
{
    if (s == NULL)
        return;

    if (s->states != NULL)
        free(s->states);

    free(s);
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

bool state_pos_is_unique(state_t *vec, Pos *pos)
{
    for (unsigned long i = 0; i < vec->size; i++)
    {
        if (vec->states[i].pos.col == pos->col &&
            vec->states[i].pos.row == pos->row)
        {
            return false;
        }
    }

    return true;
}

bool state_is_unique(state_t *vec, Pos *pos, Direction dir)
{
    for (unsigned long i = 0; i < vec->size; i++)
    {
        if (vec->states[i].dir == dir && vec->states[i].pos.col == pos->col &&
            vec->states[i].pos.row == pos->row)
        {
            return false;
        }
    }

    return true;
}
void append_to_state(state_t *vec, Pos *pos, Direction dir)
{
    if (vec->size <= vec->capacity)
    {
        if (vec->capacity == 0)
        {
            vec->capacity = INIT_CAP;
            vec->states = malloc(sizeof(GuardState) * vec->capacity);
        }
        else
        {
            vec->capacity += CAP_INCREMENT;
            vec->states =
                realloc(vec->states, sizeof(GuardState) * vec->capacity);
        }
    }
    GuardState s = {.dir = dir};

    s.pos.row = pos->row;
    s.pos.col = pos->col;

    vec->states[vec->size++] = s;
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
