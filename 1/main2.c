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

Vec *n_vec();
void delete_vec(Vec *);
void append(Vec *, int);
int solve(char *file);

int compare(const void *a, const void *b) { return *(int *)a - *(int *)b; }

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("USAGE: %s [input]", argv[0]);
        exit(EXIT_FAILURE);
    }

    int result = solve(argv[1]);

    printf("Result: %d\n", result);
}

int solve(char *file)
{
    FILE *fp = fopen(file, "r");
    Vec *vec_left = NULL;
    Vec *vec_right = NULL;
    int result = 0;

    if (fp == NULL)
    {
        printf("Failed to open file '%s'\n", file);
        exit(EXIT_FAILURE);
    }

    vec_left = n_vec();
    vec_right = n_vec();

    char io = ' ';
    bool isLeft = true;
    while (io != EOF)
    {
        int num = 0;
        while (!isspace(io) && io != EOF)
        {
            if (!isspace(io))
            {
                num = num * 10 + io - '0';
            }
            io = fgetc(fp);
        }

        if (num != 0)
        {
            if (isLeft)
            {
                append(vec_left, num);
            }
            else
            {
                append(vec_right, num);
            }

            isLeft = !isLeft;
        }

        while (isspace(io) && io != EOF)
            io = fgetc(fp);
    }

    qsort(vec_left->items, vec_left->size, sizeof(int), compare);
    qsort(vec_right->items, vec_right->size, sizeof(int), compare);

    for (unsigned long i = 0; i < vec_left->size; i++)
    {
        int frequency = 0;
        for (unsigned long j = 0; j < vec_right->size; j++)
        {
            if (vec_right->items[j] > vec_left->items[i])
            {
                break;
            }
            if (vec_left->items[i] == vec_right->items[j])
            {
                frequency++;
            }
        }
        result += vec_left->items[i] * frequency;
    }

    delete_vec(vec_left);
    delete_vec(vec_right);
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

    free(vec->items);
    vec->items = NULL;
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
