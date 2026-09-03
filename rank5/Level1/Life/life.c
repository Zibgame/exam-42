/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 14:31:27 by exam              #+#    #+#             */
/*   Updated: 2026/09/04 01:12:21 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "life.h"

char **init_map(int width, int height)
{
    int h = 0;
    int w = 0;
    char **map = malloc(sizeof(char *) * height);
    if (!map)
        return (NULL);
    while (h < height)
    {
        map[h] = malloc(sizeof(char) * width);
        if (!map[h])
            return (NULL);
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
    size_t nb_read = 100000;
    char *buf = malloc(sizeof(char) * nb_read + 1);
    if (!buf)
        return (NULL);
    int byte = 0;
    int total_byte = 0;
    while ((byte = read(STDIN_FILENO, buf, nb_read)) > 0)
    {
        total_byte += byte;
    }
    if (byte < 0)
        return (free(buf), NULL);
    if (total_byte > 0 &&  buf[total_byte - 1] == '\n')
        buf[total_byte - 1] = '\0';
    else
        buf[total_byte] = '\0';
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

int	check_rule(char **map, int x, int y, int width, int height)
{
	int	voisin;

	voisin = 0;
	if (x > 0 && y > 0 && map[y - 1][x - 1] == 'O')
		voisin++;
	if (y > 0 && map[y - 1][x] == 'O')
		voisin++;
	if (x + 1 < width && y > 0 && map[y - 1][x + 1] == 'O')
		voisin++;
	if (x > 0 && map[y][x - 1] == 'O')
		voisin++;
	if (x + 1 < width && map[y][x + 1] == 'O')
		voisin++;
	if (x > 0 && y + 1 < height && map[y + 1][x - 1] == 'O')
		voisin++;
	if (y + 1 < height && map[y + 1][x] == 'O')
		voisin++;
	if (x + 1 < width && y + 1 < height
		&& map[y + 1][x + 1] == 'O')
		voisin++;
	if (map[y][x] == 'O' && (voisin == 2 || voisin == 3))
		return (1);
	if (map[y][x] == ' ' && voisin == 3)
		return (1);
	return (0);
}

char **copy_map(char **map, int width, int height)
{
    char **copy = init_map(width, height);
    int x = 0;
    int y = 0;
    while (y < height)
    {
        x = 0;
        while (x < width)
        {      
            copy[y][x] = map[y][x];
            x++;
        }
        y++;
    }
    return (copy);
}

static void	freemap(char **map, int height)
{
	int	h;

	h = 0;
	while (h < height)
	{
		free(map[h]);
		h++;
	}
	free(map);
}

char **make_iteration(char **map, int iterations, int width, int height)
{
    int i = 0;
    int h = 0;
    int w = 0;
    char **tmp;
    char **n_map = copy_map(map, width, height);
    while (i < iterations)
    {
        h = 0;
        while (h < height)
        {
            w = 0;
            while (w < width)
            {
				if (check_rule(map, w, h, width, height))
					n_map[h][w] = 'O';
				else
					n_map[h][w] = ' ';
                w++;
            }
            h++;
        }
        tmp = map;
        map = n_map;
        n_map = tmp;
        i++;
    }
    //printf("\n");
    print_map(map, width, height);
    freemap(n_map, height);
    return map;
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

    /* printf(" width = %d\n height = %d\n iterations = %d\n stdins = %s\n\n", width, height, iterations, stdin); */
    char **map = init_map(width, height);
    map = paint_map(map, stdin, width, height);
    /* print_map(map, width, height); */
    map = make_iteration(map, iterations, width, height);
    freemap(map, height);
    free(stdin);
    return (0);
}
