/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 13:57:46 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/26 16:50:55 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vbc.h"

int parse_digit(char **s)
{
    int val;
    if (!**s)
        error_end();
    if (!isdigit(**s))
        error(**s);
    val = **s - '0';
    (*s)++;
    return(val);
}

int parse_factor(char **s)
{
    int val;

    if (!**s)
        error_end();
    if (**s == '(')
    {
        (*s)++;
        val = parse_expr(s);
        if (!**s)
            error_end();
        if (**s != ')')
            error(**s);
        (*s)++;
        return(val);
    }
    return(parse_digit(s));
}

int parse_term(char **s)
{
    int val = parse_factor(s);
    while (**s == '*')
    {
        (*s)++;
        val *= parse_factor(s);
    }
    return(val);
}

int parse_expr(char **s)
{
    int val = parse_term(s);

    while (**s == '+')
    {
        (*s)++;
        val += parse_term(s);
    }
    return (val);
}
