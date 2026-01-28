/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   n_queens.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 10:21:31 by zcadinot          #+#    #+#             */
/*   Updated: 2026/01/28 11:37:25 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int	check_argv(char **argv)
{
	int	nb;

	nb = atoi(argv[1]);
	if (nb >= 4 || nb == 1)
		return (1);
	/* fprintf(stderr, "The argument you choose is incorect"); */
	return (0);
}

void	print_solution(int *pos, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		fprintf(stdout, "%d", pos[i]);
		if (i + 1 < n)
			fprintf(stdout, " ");
		i++;
	}
	fprintf(stdout, "\n");
}

int	check_argc(int argc)
{
	if (argc == 2)
		return (1);
	/* fprintf(stderr, "Number of argument is incorect"); */
	return (0);
}

int	is_safe(int *pos, int col)
{
	int i;

	i = 0;
	while (i < col)
	{
		if (pos[i] == pos[col])
		{
			return (0);
		}
		if (abs(pos[i] - pos[col]) == col - i)
		{
			return(0);
		}
		i++;
	}
	return (1);
}

void solve(int *pos, int col, int n)
{
	int row;
	if (col == n)
	{
		print_solution(pos, n);
		return ;
	}
	row = 0;
	while (row < n)
	{
		pos[col] = row;
		if (is_safe(pos, col))
		{
			solve(pos, col + 1, n);
		}
		row++;
	}
}

int	main(int argc, char **argv)
{
	int	*pos;
	int	n;
	if (!check_argc(argc) || !check_argv(argv))
		return (1);

	n = atoi(argv[1]);
	pos = calloc(n, sizeof(int));
	if (!pos)
		return (1);

	solve(pos, 0, n);
	free(pos);
	return (0);
}
