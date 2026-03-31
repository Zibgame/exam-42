/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:24:26 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/31 12:59:58 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int parse_expr(char **s);

void error(char c)
{
    printf("Unexpected token '%c'\n", c);
    exit(1);
}

void error_end()
{
    printf("Unexpected end of file\n");
    exit(1);
}

int parse_digit(char **s)
{
    int val;
    if (!**s)
        return (0);
    if (!isdigit(**s))
        return (0);
    val = **s - '0';
    (*s)++;
    return (val);
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
            error('(');
        if (**s != ')')
            error(**s);
        (*s)++;
        return(val);
    }
    return (parse_digit(s));
}

int parse_term(char **s)
{
    int val = parse_factor(s);
    while (**s)
    {
        if (**s == '*')
        {
            (*s)++;
            val *= parse_factor(s);
        }
        else
            break;
    }
    return (val);
}

int parse_expr(char **s)
{
    int val = parse_term(s);
    while (**s)
    {
        if (**s == '+')
        {
            (*s)++;
            val += parse_term(s);
        }
        else
            break;
    }
    return (val);
}

int main(int argc, char **argv)
{
    int res;
    char *s;

    if (argc != 2)
        return (1);
    s = argv[1];
    if (!*s)
    {
        printf("0\n");
        return (0);
    }
    res = parse_expr(&s);
    while (*s)
    {
        if (*s == ')' && *(s + 1) == '(')
            s += 2;
        else
            error(*s);
    }
    printf("%d\n", res);
    return (0);
}
