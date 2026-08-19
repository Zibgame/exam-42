/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsq.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 16:48:31 by zcadinot          #+#    #+#             */
/*   Updated: 2026/08/19 01:30:36 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct s_map
{
    char **map;
    int width;
    int height;
} t_map;

typedef struct s_elt
{
    int n_ligne;
    char empty;
    char obstacle;
    char full;
} t_elt;
