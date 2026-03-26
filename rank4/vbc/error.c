/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 13:56:01 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/26 14:10:23 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vbc.h"

void error(char c)
{
    printf("Unexpected token '%c'\n", c);
    exit(1);
}

void error_end()
{
    printf("Unexpected end of input\n");
    exit(1);
}
