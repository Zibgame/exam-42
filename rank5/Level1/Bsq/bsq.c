/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsq.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 16:47:49 by zcadinot          #+#    #+#             */
/*   Updated: 2026/09/10 16:13:25 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsq.h"

// ---- DEBUG ----

int print_elt(t_elt *elements)
{
    fprintf(stdout, "elements->row = '%d'\n", elements->row);
    fprintf(stdout, "elements->empty = '%c'\n", elements->empty);
    fprintf(stdout, "elements->obstacle = '%c'\n", elements->obstacle);
    fprintf(stdout, "elements->full = '%c'\n", elements->full);
    return (1);
}

int print_map(t_map *map)
{
    int i = 0;
    fprintf(stdout, "map->height = '%d'\n", map->height);
    fprintf(stdout, "map->width = '%d'\n", map->width);
    while (i < map->height)
    {
        fprintf(stdout, "%s", map->grid[i]);
        i++;
    }
    return (1);
}

// ----      ----

int ft_isdigit(char c)
{
    return (c >= 48 && c <= 57);
}

char *ft_strchr(char *str, char c)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == c)
            return (&str[i]);
        i++;
    }
    if (c == '\0')
        return (&str[i]);
    return (NULL);
}

char *ft_substr(char *src, int start, int len)
{
    char    *str;
    int     i;

    str = malloc(sizeof(char) * (len + 1));
    if (!str)
        return (NULL);
    i = 0;
    while (i < len)
    {
        str[i] = src[start + i];
        i++;
    }
    str[i] = '\0';
    return (str);
}

int free_map(t_map *map)
{
    int i = 0;
    while (i < map->height)
    {
        free(map->grid[i]);
        i++;
    }
    free(map->grid);
    return (0);
}

int load_elt(t_elt *elements, FILE *file)
{
    char *line = NULL;
    size_t n = 0;
    int len = 0;
    int i = 0;
    if ((len = getline(&line, &n, file)) < 0)
    {
        /* fprintf(stderr, "Error: {getline} Fail in [load_elt]\n"); */
        free(line);
        return (0);
    }
    if (len < 5 || line[len - 1] != '\n')
    {
        /* fprintf(stderr, "Error: number of elt invalid\n"); */
        free(line);
        return(0);
    }
    elements->row = 0;
    while (i < len - 4)
    {
        if (!ft_isdigit(line[i]))
        {
            /* fprintf(stderr, "Error: elt->row in file not a digits\n"); */
            free(line);
            return(0);
        }
        elements->row = elements->row * 10 + (line[i] - '0');
        i++;
    }
    if (elements->row <= 0 || line[len - 4] == line[len - 3]
        || line[len - 4] == line[len - 2]
        || line[len - 3] == line[len - 2])
    {
        /* fprintf(stderr, "Error: Duplicate char in elt\n"); */
        free(line);
        return (0);
    }
    elements->empty = line[len - 4];
    elements->obstacle = line[len - 3];
    elements->full = line[len - 2];
    free(line);
    return (1);
}

int load_map(t_map *map, t_elt *elements, FILE *file)
{
    map->height = elements->row;
    map->grid = calloc(map->height, sizeof(char *));
    char *line = NULL;
    char chars[3];
    size_t n = 0;
    int len = 0;
    int i = 0;
    int j = 0;

    chars[0] = elements->empty;
    chars[1] = elements->obstacle;
    chars[2] = '\0';
    if (!map->grid)
    {
        return (0);
    }
    while (i < map->height)
    {
        if ((len = getline(&line, &n, file)) < 0)
        {
            free_map(map);
            /* fprintf(stderr, "Error: {getline} Fail in [load_map]\n"); */
            return (0);
        }
        if (i == 0)
        {
            map->width = len;
        }
        if (map->width != len)
        {
            free(line);
            free_map(map);
            /* fprintf(stderr, "Error: {getline} all the line are not the same len\n"); */
            return (0);
        }
        if (line[len - 1] != '\n')
        {
            free(line);
            free_map(map);
            /* fprintf(stderr, "Error: {getline} line dont finish with /n\n"); */
            return (0);
        }
        if (len <= 1)
        {
            free(line);
            free_map(map);
            return (0);
        }
        j = 0;
        while (j < len - 1)
        {
            if (!ft_strchr(chars, line[j]))
            {
                free(line);
                free_map(map);
                return (0);
            }
            j++;
        }
        map->grid[i] = ft_substr(line, 0, len);
        if (!map->grid[i])
        {
            free(line);
            free_map(map);
            return (0);
        }
        i++;
    }
    free(line);
    return (1);
}

int **create_int_tab(int height, int width)
{
    int **tab = malloc((sizeof(int *) * height));
    if (!tab)
    {
        return (0);
    }
    int h = 0;
    int w = 0;
    while (h < height)
    {
        w = 0;
        tab[h] = malloc(sizeof(int) * width);
        if (!tab[h])
        {
            free(tab);
            return (0);
        }
        while (w < width)
        {
            tab[h][w] = 0;
            w++;
        }
        h++;
    }
    return (tab);
}

int min(int gauche, int diag, int haut)
{
    int res = gauche;
    if (diag < res)
     res = diag;
    if (haut < res)
     res = haut;
    if (gauche < res)
     res = gauche;
    return (res);
}

int print_res(t_sqr *sqr, t_map *map, t_elt *elements)
{
    int h = 0;
    int w = 0;
    while (h < map->height)
    {
        w = 0;
        while (w < map->width - 1)
        {
            if (h <= sqr->y
                    && h >= sqr->y - sqr->height + 1
                    && w <= sqr->x
                    && w >= sqr->x - sqr->width + 1)
                fprintf(stdout, "%c", elements->full);
            else
                fprintf(stdout, "%c", map->grid[h][w]);
            w++;
        }
        fprintf(stdout, "\n");
        h++;
    }
    return (0);
}

int find_sqr(t_sqr *sqr, t_map *map, t_elt *elements)
{
    int h = 0;
    int w = 0;
    int gauche = 0;
    int haut = 0;
    int diag = 0;
    int max  = 0;
    sqr->x = 0;
    sqr->y = 0;
    sqr->width = 0;
    sqr->height = 0;
    int **tab = create_int_tab(map->height , map->width - 1);
    if (!tab)
        return (0);
    while (h < map->height)
    {
        w = 0;
        while (w < map->width - 1)
        {
            gauche = 0;
            haut = 0;
            diag = 0;
            // oo
            // Ux
            if (h >= 0 && w - 1 >= 0)
            {
                gauche = tab[h][w - 1];
            }
            // Uo
            // ox
            if (h - 1 >= 0 && w - 1 >= 0)
            {
                diag = tab[h - 1][w - 1];
            }
            // oU
            // ox
            if (h - 1 >= 0 && w >= 0)
            {
                haut = tab[h - 1][w];
            }
            if (map->grid[h][w] == elements->obstacle)
                tab[h][w] = 0;
            else
                tab[h][w] = 1 + min(haut, gauche, diag);
            if (tab[h][w] > max)
            {
                max = tab[h][w];
                sqr->x = w;
                sqr->y = h;
                sqr->width = max;
                sqr->height = max;
            }
            /* fprintf(stdout,"%c", map->grid[h][w]); */
            w++;
        }
        h++;
    }
    h = 0;
    while (h < map->height)
    {
        free(tab[h]);
        h++;
    }
    free(tab);
    return (1);
}

int do_bsq(FILE *file)
{
    t_elt elements;
    t_map map;
    t_sqr sqr;
    map.grid = NULL;
    map.height = 0;
    map.width = 0;
    if (!load_elt(&elements, file))
    {
        fputs("Error: invalid map\n", stdout);
        fclose(file);
        return (0);
    }
    /* print_elt(&elements); */
    if (!load_map(&map, &elements, file))
    {
        fputs("Error: invalid map\n", stdout);
        fclose(file);
        return (0);
    }
    /* print_map(&map); */
    if (!find_sqr(&sqr ,&map, &elements))
    {
        fputs("Error: allocation failed\n", stdout);
        fclose(file);
        free_map(&map);
        return (1);
    }
    print_res(&sqr, &map, &elements);
    fclose(file);
    free_map(&map);
    return (1);
}

int do_bsq_file(char *path)
{
    FILE *file;
    if ((file = fopen(path, "r")) == NULL)
    {
        /* fprintf(stderr, "Error: {fopen} Fail in [do_bsq_file]\n"); */
        return (0);
    }
    if (!do_bsq(file))
    {
        return (0);
    }
    return (1);
}

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        /* fprintf(stderr, "Error: Wrong Argc\n"); */
        return (0);
    }
    if (argc == 2)
    {
        if (!do_bsq_file(argv[1]))
        {
            return (0);
        }
    }
    else if(argc == 1)
    {
        if (!do_bsq(stdin))
        {
            return (0);
        }
    }
    return (0);
}
