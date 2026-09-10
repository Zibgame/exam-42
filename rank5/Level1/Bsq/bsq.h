/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsq.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 16:48:31 by zcadinot          #+#    #+#             */
/*   Updated: 2026/09/10 13:46:16 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct s_map
{
    char **grid;
    int width;
    int height;
} t_map;

typedef struct s_elt
{
    int row;
    char empty;
    char obstacle;
    char full;
} t_elt;

typedef struct s_sqr
{
    int x;
    int y;
    int height;
    int width;
} t_sqr;
