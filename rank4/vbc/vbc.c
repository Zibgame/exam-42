/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 13:55:24 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/26 16:45:05 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vbc.h"

int main(int argc, char **argv)
{
    int res;
    char  *s;
    if (argc != 2)
		return (1);

    s = argv[1];
    res = parse_expr(&s);
    if (*s)
        error(*s);
    printf("%d\n",res);
    return (0);
}
