/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 13:54:58 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/26 16:40:00 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void error(char c);
void error_end();

int parse_digit(char **s);
int parse_factor(char **s);
int parse_term(char **s);
int parse_expr(char **s);
