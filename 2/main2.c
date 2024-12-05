#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAP 25
#define CAP_INCREMENT 5
#define DEBUG_FILE "unsafe.txt"

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
void vec_clear(Vec *);
int vec_pop(Vec *);
bool check_subarray(int *, unsigned long size, bool);
bool issafe(Vec *);

bool check_with_exclusion(int *, unsigned long, unsigned long);

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

bool check_with_exclusion(int *vec, unsigned long size, unsigned long ele)
{
    int *copy = NULL;
    copy = malloc(sizeof(int) * (size - 1));

    for (int i = 0; i < size - 1; i++)
    {
        if (i < ele)
        {
            copy[i] = vec[i];
        }
        else
        {
            copy[i] = vec[i + 1];
        }
    }
    printf("\noriginal: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", vec[i]);
    }
    printf("\ncopy: ");

    for (int i = 0; i < size - 1; i++)
    {
        printf("%d ", copy[i]);
    }

    bool only_inc = true;
    bool only_dec = true;
    bool ok = true;

    for (unsigned long i = 0; i < size - 2; i++)
    {
        int diff = copy[i + 1] - copy[i];
        if (diff > 0)
        {
            only_dec = false;
        }

        if (diff < 0)
        {
            only_inc = false;
        }

        if (diff == 0 || abs(diff) > 3)
        {
            ok = false;
            break;
        }
    }

    free(copy);

    bool ret = ok && (only_inc || only_dec);
    printf("%s%s%s, overall: %s\n", ok ? "OK " : " ", only_inc ? "INC" : " ",
           only_dec ? " DEC" : "", ret ? "SAFE" : "UNSAFE");
    return ret;
}

int solve(char *file)
{
    FILE *fp = fopen(file, "r");
    FILE *debug_file = NULL;

    if (strcmp(file, DEBUG_FILE) != 0)
    {
        debug_file = fopen(DEBUG_FILE, "w+");
    }

    Vec *vec = NULL;
    int result = 0;

    if (fp == NULL)
    {
        printf("Failed to open file '%s'\n", file);
        exit(EXIT_FAILURE);
    }

    vec = n_vec();

    char io;

    int num = 0;
    while ((io = fgetc(fp)) && io != EOF)
    {
        if (!isspace(io))
        {
            num = num * 10 + io - '0';
        }
        else
        {
            append(vec, num);
            num = 0;
        }

        if (io == '\n')
        {
            if (issafe(vec))
            {
                result++;
            }
            else if (debug_file != NULL)
            {
                for (unsigned long i = 0; i < vec->size; i++)
                {
                    fprintf(debug_file, "%d ", vec->items[i]);
                }
                fprintf(debug_file, "\n");
            }
            vec_clear(vec);
        }
    }

    delete_vec(vec);
    fclose(fp);

    if (debug_file != NULL)
    {
        fclose(debug_file);
    }
    return result;
}

void consume_line(FILE *fp)
{
    char check = fgetc(fp);
    ungetc(check, fp);

    if (check == '\n')
    {
        return;
    }

    char io;
    while ((io = fgetc(fp)) != '\n' && io != EOF)
        ;
}

bool issafe(Vec *vec) { return check_subarray(vec->items, vec->size, false); }

bool check_subarray(int *items, unsigned long size, bool modified)
{
    bool ok = false;
    int lastnum = items[0];
    int count = 1;
    bool inc = items[1] > lastnum;
    bool anyOk = true;

    if (check_with_exclusion(items, size, 0))
    {
        return true;
    }

    for (unsigned long i = 0; i < size - 1; i++)
    {
        int diff = items[i + 1] - items[i];

        if (abs(diff) < 1 || abs(diff) > 3)
        {
            if (check_with_exclusion(items, size, i) ||
                check_with_exclusion(items, size, i + 1))
            {
                ok = true;
            }
            break;
        }

        if (i + 2 < size)
        {
            int diff2 = items[i + 2] - items[i + 1];
            if ((diff > 0) != (diff2 > 0))
            {
                if (check_with_exclusion(items, size, i) ||
                    check_with_exclusion(items, size, i + 1) ||
                    check_with_exclusion(items, size, i + 2))
                {
                    ok = true;
                }
                break;
            }
        }
    }

    return ok;
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
    if (vec->size >= vec->capacity)
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

int vec_pop(Vec *vec) { return vec->items[vec->size--]; }
void vec_clear(Vec *vec) { vec->size = 0; }
