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
void sort_vec(Vec *);

bool valid_order(Vec *, Vec *, Vec *);
int evaluate_order(Vec *, Vec *, Vec *);
void fix_order(unsigned long, Vec *, Vec *, Vec *);

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
    Vec *preorders = NULL;
    Vec *postorders = NULL;
    Vec *orders = NULL;
    double result = 0;

    if (fp == NULL)
    {
        printf("Failed to open file '%s'\n", file);
        exit(EXIT_FAILURE);
    }

    preorders = n_vec();
    postorders = n_vec();
    orders = n_vec();

    char io;
    bool ordersDone = false;

    int num = 0;
    while ((io = fgetc(fp)) && io != EOF)
    {
        if (io >= '0' && io <= '9')
        {
            num = num * 10 + io - '0';
        }

        if (io == ',')
        {
            ordersDone = true;
        }

        if (ordersDone && io == ',')
        {
            append(orders, num);
            num = 0;
        }

        if (!ordersDone && io == '|')
        {
            append(preorders, num);
            num = 0;
        }

        if (io == '\n')
        {
            if (ordersDone)
            {
                append(orders, num);
                result += evaluate_order(preorders, postorders, orders);
                orders->size = 0;
            }
            else if (num != 0)
            {
                append(postorders, num);
            }
            num = 0;
        }
    }

    delete_vec(preorders);
    delete_vec(postorders);
    delete_vec(orders);
    return result;
}

bool next_is_valid(unsigned long i, Vec *pre, Vec *post, Vec *ord)
{
    if (i >= ord->size)
        return true;

    Vec *o_order = n_vec();

    for (unsigned long j = 0; j < pre->size; j++)
    {
        if (ord->items[i] == pre->items[j])
        {
            append(o_order, post->items[j]);
        }
    }

    for (unsigned long j = 0; j < o_order->size; j++)
    {
        for (unsigned long k = 0; k < ord->size; k++)
        {
            if (ord->items[k] == o_order->items[j])
            {
                if (i >= k)
                {
                    delete_vec(o_order);
                    return false;
                }
            }
        }
    }

    delete_vec(o_order);
    return next_is_valid(i + 1, pre, post, ord);
}

bool valid_order(Vec *pre, Vec *post, Vec *ord)
{
    return next_is_valid(0, pre, post, ord);
}

void fix_order(unsigned long i, Vec *pre, Vec *post, Vec *ord)
{
    if (valid_order(pre, post, ord))
    {
        return;
    }

    Vec *o_order = n_vec();

    for (unsigned long j = 0; j < pre->size; j++)
    {
        if (ord->items[i] == pre->items[j])
        {
            append(o_order, post->items[j]);
        }
    }

    for (unsigned long j = 0; j < o_order->size; j++)
    {
        for (unsigned long k = 0; k < ord->size; k++)
        {
            if (ord->items[k] == o_order->items[j])
            {
                if (i >= k) // swap
                {
                    int tmp = ord->items[i];
                    ord->items[i] = ord->items[k];
                    ord->items[k] = tmp;
                    delete_vec(o_order);
                    fix_order(k, pre, post, ord);
                    return;
                }
            }
        }
    }

    delete_vec(o_order);
    fix_order(i + 1, pre, post, ord);
}

int evaluate_order(Vec *pre, Vec *post, Vec *ord)
{
    if (!valid_order(pre, post, ord))
    {
        fix_order(0, pre, post, ord);
        int mid = ord->size / 2;
        return ord->items[mid];
    }

    return 0;
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
