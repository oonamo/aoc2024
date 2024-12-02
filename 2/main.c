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
void consume_line(FILE *);

bool issafe(Vec *);

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
    int result = 0;

    if (fp == NULL)
    {
        printf("Failed to open file '%s'\n", file);
        exit(EXIT_FAILURE);
    }

    vec = n_vec();

    char io;

    int lastnum = 0;
    int num = 0;

    bool hasFirst = false;
    bool isIncrement = false;
    bool isDecrement = false;

    while ((io = fgetc(fp)) && io != EOF)
    {
        if (!isspace(io))
        {
            num = num * 10 + io - '0';
        }
        else
        {
            if (!hasFirst)
            {
                lastnum = num;
                num = 0;
                hasFirst = true;
                continue;
            }

            int diff = abs(num - lastnum);

            if (!isIncrement && !isDecrement)
            {
                isIncrement = num > lastnum;
                isDecrement = !isIncrement;
            }

            if ((isIncrement && num <= lastnum) ||
                (isDecrement && num >= lastnum) || diff > 3 || diff == 0)
            {
                lastnum = 0;
                hasFirst = false;
                isDecrement = false;
                isIncrement = false;
                consume_line(fp);
            }
            else
            {
                lastnum = num;
            }

            num = 0;
        }

        if (io == '\n' && (isIncrement || isDecrement))
        {
            num = 0;
            lastnum = 0;
            hasFirst = false;
            isDecrement = false;
            isIncrement = false;
            result++;
        }
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

void delete_vec(Vec *vec)
{
    if (vec == NULL)
        return;

    free(vec->items);
    free(vec);
}

void append(Vec *vec, int val)
{
    if (vec->size <= vec->capacity)
    {
        if (vec->capacity == 0)
        {
            vec->capacity = INIT_CAP;
            vec->items = malloc(sizeof(int *) * vec->capacity);
        }
        else
        {
            vec->capacity += CAP_INCREMENT;
            vec->items = realloc(vec->items, sizeof(int *) * vec->capacity);
        }
    }
    vec->items[vec->size++] = val;
}
