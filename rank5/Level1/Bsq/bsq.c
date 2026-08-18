/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsq.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 16:47:49 by zcadinot          #+#    #+#             */
/*   Updated: 2026/08/18 17:27:43 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsq.h"

int do_bsq(FILE *file)
{
    return (0);
}

int do_bsq_path(char *file)
{
    return (0);
}

int main(int argc, char **argv)
{
    printf("BSQ running ... \n");
    if (argc == 1)
    {
        printf("stdin\n");
    }
    else if (argc > 1)
    {
        size_t i = 0;

        while (i < argc - 1)
        {
            do_bsq_path(argv[i]);
            i++;
        }
    }
    return (0);
}
