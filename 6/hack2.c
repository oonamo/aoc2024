#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ENC_DIR(dr, dc) (((dr) * 2) + dc)
#define SZ 130

char grid[SZ][SZ];
int rs, cs;

void init(char *);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("USAGE: %s [input]", argv[0]);
        exit(EXIT_FAILURE);
    }
    init(argv[1]);

    static char visited[SZ][SZ];
    int r, c, dr, dc, t = 0, tmp;
    int i, j;

    for (i = 0; i < SZ; i++)
        for (j = 0; j < SZ; j++)
        {
            if (grid[i][j] == '^')
            {
                rs = i;
                cs = j;
                break;
            }
        }
    for (int i = 0; i < SZ; i++)
    {
        for (int j = 0; j < SZ; j++)
        {
            if (grid[i][j] == '.')
            {
                grid[i][j] = '#';
                dr = -1;
                dc = 0;
                r = rs;
                c = cs;
                memset(&visited[0][0], 0, SZ * SZ);
                while (1)
                {
                    if (r + dr < 0 || c + dc < 0 || r + dr >= SZ || c + dc >= SZ)
                    {
                        break;
                    }
                    if (visited[r + dr][c + dc] == ENC_DIR(dr, dc))
                    {
                        t++;
                        break;
                    }
                    if (grid[r + dr][c + dc] == '#')
                    {
                        tmp = dr;
                        dr = dc;
                        dc = -tmp;
                    }
                    else
                    {
                        r += dr;
                        c += dc;
                        visited[r][c] = ENC_DIR(dr, dc);
                    }
                }
                grid[i][j] = '.';
            }
        }
    }
    printf("result %d\n", t);
}

void init(char *file)
{
    FILE *fp = fopen(file, "r");
    char io;
    int c = 0, r = 0;
    while ((io = fgetc(fp)) && io != EOF)
    {
        if (io == '^')
        {
            rs = r;
            cs = c;
        }

        if (io == '\n')
        {
            r++;
            c = 0;
        }
        else
        {
            grid[r][c] = io;
            c++;
        }
    }
}
