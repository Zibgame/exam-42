/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsq.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 16:47:49 by zcadinot          #+#    #+#             */
/*   Updated: 2026/08/19 02:05:25 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsq.h"

// ---- DEBUG ----

void print_elements(t_elt *elements)
{
    fprintf(stdout, "n_ligne = %d\n", elements->n_ligne);
    fprintf(stdout, "empty = %c\n", elements->empty);
    fprintf(stdout, "obstacle = %c\n", elements->obstacle);
    fprintf(stdout, "full = %c\n", elements->full);
    return ;
}

// ----      ----


int load_elements(FILE *file, t_elt *elements)
{
    int ret = 0;
    ret = fscanf(file, "%d %c %c %c", &elements->n_ligne, &elements->empty, &elements->obstacle, &elements->full);
    if (ret != 4)
        return (0);
    if (&elements->n_ligne <= 0)
        return (0);
	if(elements->empty == elements->obstacle || elements->empty == elements->full || elements->obstacle == elements->full)
		return(0);
	if(elements->empty < 32 || elements->empty > 126)
		return(0);
	if(elements->obstacle < 32 || elements->obstacle > 126)
		return(0);
	if(elements->full < 32 || elements->full > 126)
		return(0);
    return (1);
}

int do_bsq(FILE *file)
{
    t_elt elements;
    if (!load_elements(file, &elements))
    {
        fprintf(stdout, "Error Wrong Header \n");
        return (0);
    }
    print_elements(&elements);
    return (1);    
}

int do_bsq_path(char *path)
{
    FILE *file = fopen(path, "r");
    int ret = 0;

    if (!file)
        return (0);
    ret = do_bsq(file);
    fclose(file);
    return (ret);
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        if (!do_bsq(stdin))
            fprintf(stderr, "map error\n");
    }

    else if (argc > 1)
    {
        size_t i = 1;

        while (i < argc)
        {
            do_bsq_path(argv[i]);
            i++;
        }
    }
    return (0);
}
