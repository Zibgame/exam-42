/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 14:31:27 by exam              #+#    #+#             */
/*   Updated: 2026/08/05 14:31:29 by exam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "life.h"

char **init_map(int width, int height)
{
    int h = 0;
    int w = 0;
    char **map = malloc(sizeof(char *) * height);
    while (h < height)
    {
        map[h] = malloc(sizeof(char) * width);
        w = 0;
        while (w < width)
        {
            map[h][w] = ' ';
            w++;
        }
        h++;
    }
    return (map);
}

char *get_stdin()
{
    char *buf = malloc(sizeof(char) * 100 + 10);
    size_t nb_read = 100;
    int byte = 0;
    int total_byte = 0;
    while ((byte = read(STDIN_FILENO, buf, nb_read)) > 0)
    {
        total_byte += byte;
    
    }
    buf[total_byte - 1] = '\0';
    return (buf);
}

char **paint_map(char **map, char *stdin, int width, int height)
{
    int x = 0;
    int y = 0;
    int i = 0;
    int paint = 0;
    while (stdin[i])
    {
        if (stdin[i] == 'x')
        {
            if (paint == 1)
                paint = 0;
            else
                paint = 1;
        }
        if (stdin[i] == 'w')
        {
            if (y - 1 >= 0)
                y -= 1;
        }
        if (stdin[i] == 'a')
        {
            if (x - 1 >= 0)
                x -= 1;
        }
        if (stdin[i] == 's')
        {
            if (y + 1 < height)
                y += 1;
        }
        if (stdin[i] == 'd')
        {
            if (x + 1 < width)
                x += 1;
        }
        if (paint)
        {
            map[y][x] = 'O';
        }
        i++;
    }
    return (map);
}

void print_map(char **map, int width, int height)
{
    int w = 0;
    int h = 0;
    while (h < height)
    {
        w = 0;
        while (w < width)
        {
            putchar(map[h][w]);
            w++;
        }
        putchar('\n');
        h++;
    }
    return ;
}

int check_rule(char **map, int x, int y, int width, int height)
{
    //map[y][x] = 'X';
    int voisin = 0;
    int life = 0;
    //
    // UOO
    // OXO
    // OOO
    //
    if ((x - 1 >=0 && x - 1 < height) && (y - 1 >=0 && y - 1 < width))
    {
        if (map[y - 1][x - 1] == 'O')
        {
            //map[y - 1][x - 1] = 'U';
            voisin++;
        }
    }
    //
    // OUO
    // OXO
    // OOO
    //
    if ((x >= 0 && x < height) && (y - 1 >=0 && x - y < width))
    {
        if (map[y - 1][x] == 'O')
        {
            //map[y - 1][x] = 'U';
            voisin++;
        }
    }
    //
    // OOU
    // OXO
    // OOO
    //
    if ((x + 1 >= 0 && x + 1 < height) && (y - 1 >=0 && x - y < width))
    {
        if (map[y - 1][x + 1] == 'O')
        {
            //map[y - 1][x + 1] = 'U';
            voisin++;
        }
    }

    //
    // OOO
    // UXO
    // OOO
    //
    if ((x - 1 >=0 && x - 1 < height) && (y  >= 0 && y  < width))
    {
        if (map[y ][x - 1] == 'O')
        {
            //map[y ][x - 1] = 'U';
            voisin++;
        }
    }

    //
    // OOO
    // UXO
    // OOO
    //
    if ((x + 1 >=0 && x + 1 < height) && (y  >= 0 && y  < width))
    {
        if (map[y ][x + 1] == 'O')
        {
            //map[y ][x + 1] = 'U';
            voisin++;
        }
    }

    //
    // 0OO
    // OXO
    // OOU
    //
    if ((x - 1 >=0 && x - 1 < height) && (y + 1 >=0 && y + 1 < width))
    {
        if (map[y + 1][x - 1] == 'O')
        {
            //map[y + 1][x - 1] = 'U';
            voisin++;
        }
    }

    //
    // 0OO
    // OXO
    // OOU
    //
    if ((x >=0 && x < height) && (y + 1 >=0 && y + 1 < width))
    {
        if (map[y + 1][x] == 'O')
        {
            //map[y + 1][x] = 'U';
            voisin++;
        }
    }

    //
    // 0OO
    // OXO
    // OOU
    //
    if ((x + 1 >=0 && x + 1 < height) && (y + 1 >=0 && y + 1 < width))
    {
        if (map[y + 1][x + 1] == 'O')
        {
            //map[y + 1][x + 1] = 'U';
            voisin++;
        }
    }
    
    //printf("\n");
    //print_map(map, width, height);

    if (map[y][x] == ' ' && voisin == 3)
    {
        life = 1;
    }
    else if ((voisin == 2 || voisin == 3) && map[y][x] == 'O')
        life = 1;
    else if (voisin < 2)
        life = 0;
    else if (voisin > 3)
        life = 0;
    printf("\n%d\n", voisin);
    return life;
}

void make_iteration(char **map, int iterations, int width, int height)
{
    int i = 0;
    int h = 0;
    int w = 0;
    char **n_map = init_map(width, height);
    n_map = map;
    while (i < iterations)
    {
        while (h < height)
        {
            w = 0;
            while (w < width)
            {
                if (check_rule(map, w, h, width, height) == 1)
                {
                    if ((w >= 0 && w < width - 1) && (h >= 0 && h < height - 1))
                    {
                        n_map[h][w] = 'O';
                    }
                }
                else if (check_rule(map, w, h, width, height) == 0)
                {
                    if ((w >= 0 && w < width - 1) && (h >= 0 && h < height - 1))
                    {
                        n_map[h][w] = ' ';
                    }
                }
                w++;
            }
            h++;
        }
        i++;
    }
    printf("\n");
    print_map(n_map, width, height);
    return ;
}

int main(int argc, char **argv)
{
    if (argc != 4)
        return (0);

    int width, height, iterations = 0;
    width = atoi(argv[1]);
    height = atoi(argv[2]);
    iterations = atoi(argv[3]);
    char *stdin = get_stdin();

    printf(" width = %d\n height = %d\n iterations = %d\n stdins = %s\n", width, height, iterations, stdin);
    char **map = init_map(width, height);
    map = paint_map(map, stdin, width, height);
    print_map(map, width, height);
    make_iteration(map, iterations, width, height);
    return (0);
}