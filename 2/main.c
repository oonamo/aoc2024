#define _CRT_SECURE_NO_WARNINGS
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
    char **items;
} Vec;

Vec *n_vec();
void delete_vec(Vec *);
void append(Vec *, char *);

double solve(char *file);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("USAGE: %s [input]", argv[0]);
        exit(EXIT_FAILURE);
    }

    double result = solve(argv[1]);

    printf("Result: %f", result);
}

double solve(char *file)
{
    FILE *fp = fopen(file, "r");
    Vec* vec = NULL;
    double result = 0;

    if (fp == NULL)
    {
        printf("Failed to open file '%s'\n", file);
        exit(EXIT_FAILURE);
    }

    vec = n_vec();

    char io;
    while ((io = fgetc(fp)) && io != EOF) {
    }

    delete_vec(vec);
    return result;
}

Vec *n_vec()
{
    Vec *v = malloc(sizeof(Vec));
    v->capacity = 0;
    v->size = 0;
    v->items = NULL;
    return v;
}

void delete_vec(Vec *vec)
{
    if (vec == NULL)
        return;

    for (unsigned long i = 0; i < vec->size; i++)
    {
        if (vec->items[i] != NULL)
            free(vec->items[i]);
    }

    free(vec);
}

void append(Vec *vec, char *val)
{
    if (vec->size <= vec->capacity)
    {
        if (vec->capacity == 0)
        {
            vec->capacity = INIT_CAP;
            vec->items = malloc(sizeof(char *) * vec->capacity);
        }
        else
        {
            vec->capacity += CAP_INCREMENT;
            vec->items = realloc(vec->items, sizeof(char *) * vec->capacity);
        }
    }
    vec->items[vec->size++] = val;
}

