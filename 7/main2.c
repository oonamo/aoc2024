#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAP 25
#define CAP_INCREMENT 5

typedef struct
{
    unsigned long long size;
    unsigned long long capacity;
    unsigned long long *items;
} Vec;

typedef struct
{
    unsigned long long columns;
    unsigned long long rows;
    unsigned long long **items;
} Matrix;

Vec *n_vec();
void delete_vec(Vec *);
void append(Vec *, unsigned long long);
void sort_vec(Vec *);

Matrix *n_matrix(int rows, int cols);
void appendRowVector(Matrix *m, Vec *v);
void delete_matrix(Matrix *m);

int compare(const void *a, const void *b);
bool possible(Vec *, unsigned long long, unsigned long long,
              unsigned long long);

void consume_line(FILE *);

unsigned long long solve(char *file);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("USAGE: %s [input]", argv[0]);
        exit(EXIT_FAILURE);
    }

    unsigned long long result = solve(argv[1]);

    printf("Result: %llu", result);
}

unsigned long long solve(char *file)
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
    unsigned long long num = 0;
    while ((io = fgetc(fp)) && io != EOF)
    {
        if (io >= '0' && io <= '9')
        {
            num = num * 10 + io - '0';
        }

        if (io == ':')
        {
            append(vec, num);
            num = 0;
        }

        if (io == ' ' && num != 0)
        {
            append(vec, num);
            num = 0;
        }

        if (io == '\n')
        {
            append(vec, num);
            printf("%llu: ", vec->items[0]);
            for (unsigned long i = 1; i < vec->size; i++)
                printf("%llu ", vec->items[i]);
            printf("\n");

            if (possible(vec, 1, vec->items[1], vec->items[0]))
            {
                result += vec->items[0];
            }

            vec->size = 0;
            num = 0;
        }
    }

    delete_vec(vec);
    return result;
}

unsigned long long concat_long(unsigned long long lhs, unsigned long long rhs)
{
    unsigned long long pow = 10;
    while (rhs >= pow)
    {
        pow *= 10;
    }
    /*printf("lhs=%llu, rhs=%llu, ret=%llu\n", lhs, rhs, lhs * pow + rhs);*/
    return lhs * pow + rhs;
}

int depth(unsigned long long n)
{
    int depth = 0;
    unsigned long long pow = 10;
    while (n >= pow)
    {
        pow *= 10;
        depth++;
    }
    return depth;
}

bool possible(Vec *v, unsigned long long i, unsigned long long cur,
              unsigned long long target)
{
    if (i + 1 == v->size && cur == target)
        return true;

    if (cur > target)
        return false;

    if (i + 2 > v->size)
        return false;

    if (depth(cur) + depth(v->items[i + 1]) > 20)
    {
        return (possible(v, i + 1, cur * v->items[i + 1], target) ||
                possible(v, i + 1, cur + v->items[i + 1], target));
    }

    /*printf("i = %llu\n", i);*/

    return (possible(v, i + 1, cur * v->items[i + 1], target) ||
            possible(v, i + 1, cur + v->items[i + 1], target) ||
            possible(v, i + 1, concat_long(cur, v->items[i + 1]), target));
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

void append(Vec *vec, unsigned long long val)
{
    if (vec->size <= vec->capacity)
    {
        if (vec->capacity == 0)
        {
            vec->capacity = INIT_CAP;
            vec->items = malloc(sizeof(unsigned long long) * vec->capacity);
        }
        else
        {
            vec->capacity += CAP_INCREMENT;
            vec->items =
                realloc(vec->items, sizeof(unsigned long long) * vec->capacity);
        }
    }
    vec->items[vec->size++] = val;
}
