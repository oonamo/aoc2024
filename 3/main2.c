#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAP 25
#define CAP_INCREMENT 5

typedef enum
{
    M,
    U,
    L,
    L_PARENT,
    COMMA,
    R_PAREN,
    NUMBER,
    D,
    O,
    N,
    T,
    APOSTROPHE,
} Token;

typedef enum
{
    MUL,
    DO,
    DONT,
    NONE,
} Command;

typedef struct
{
    unsigned long size;
    unsigned long capacity;
    char *items;
} Vec;

Vec *n_vec();
void delete_vec(Vec *);
void append(Vec *, int);
void sort_vec(Vec *);
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
    Token next = M;

    Token discriminator;
    Command cmd;

    Command active_command = DO;

    int lhs = 0;
    int rhs = 0;
    while ((io = fgetc(fp)) && io != EOF)
    {
        if (io >= '0' && io <= '9')
        {
            if (next == COMMA && cmd == MUL)
            {
                lhs = lhs * 10 + io - '0';
            }
            else if (next == R_PAREN && cmd == MUL)
            {
                rhs = rhs * 10 + io - '0';
            }
            else
            {
                goto reset;
            }
            continue;
        }

        switch (io)
        {
        case 'd':
            next = O;
            break;
        case 'o':
            if (next == O)
            {
                next = L_PARENT;
                discriminator = N;
                cmd = DO;
                break;
            }
            else
                goto reset;
        case 'n':
            if (discriminator == N)
            {
                next = APOSTROPHE;
                cmd = NONE;
                break;
            }
            else
                goto reset;
        case '\'':
            if (next == APOSTROPHE)
            {
                next = T;
                break;
            }
            else
                goto reset;
        case 't':
            if (next == T)
            {
                cmd = DONT;
                next = L_PARENT;
                break;
            }
            else
                goto reset;
        case 'm':
            next = U;
            break;
        case 'u':
            if (next == U)
            {
                next = L;
                break;
            }
            else
                goto reset;
        case 'l':
            if (next == L)
            {
                next = L_PARENT;
                cmd = MUL;
                break;
            }
            else
                goto reset;
        case '(':
            if (next == L_PARENT && cmd != NONE)
            {
                if (cmd == MUL)
                {
                    next = COMMA;
                }
                else if (cmd == DO || cmd == DONT)
                {
                    next = R_PAREN;
                }
                break;
            }
            else
                goto reset;
        case ',':
            if (next == COMMA)
            {
                next = R_PAREN;
                break;
            }
            else
                goto reset;
        case ')':
            if (next == R_PAREN)
            {
                if (cmd == DO)
                {
                    active_command = DO;
                }
                else if (cmd == DONT)
                {
                    active_command = DONT;
                }

                if (active_command == DO)
                {
                    result += lhs * rhs;
                }
            }
        default:
            goto reset;
            break;
        }
        continue;

    reset:
        lhs = 0;
        rhs = 0;
        next = M;
        cmd = NONE;
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
