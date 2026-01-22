/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   powerset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 20:06:16 by zcadinot          #+#    #+#             */
/*   Updated: 2026/01/22 22:32:06 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int	*init_tab(char **argv, int argc)
{
	int	*tab;
	int	i;

	i = 0;
	tab = malloc(sizeof(int) * (argc - 2));
	if (!tab)
		return (0);
	while (i < argc - 2)
	{
		tab[i] = atoi(argv[i + 2]);
		i++;
	}
	return (tab);
}

void	print_subset(int *subset, int size)
{
	int		i;
	char	buf[12];
	int		len;

	i = 0;
	while (i < size)
	{
		len = sprintf(buf, "%d", subset[i]);
		write(1, buf, len);
		if (i + 1 < size)
			write(1, " ", 1);
		i++;
	}
	write(1, "\n", 1);
}

void solve(int *tab, int len_tab, int index, int actual_sum, int *subset, int subset_len, int target)
{
	if (index == len_tab)
	{
		if (actual_sum == target)
		{
			print_subset(subset, subset_len);
		}
		return  ;
	}
	subset[subset_len] = tab[index];
	solve(tab, len_tab, index + 1, actual_sum + tab[index], subset, subset_len + 1, target);
	solve(tab, len_tab, index + 1, actual_sum, subset, subset_len, target);
}

int	main(int argc, char **argv)
{
	int	*tab;
	int	*subset;
	int	target;

	if (argc < 3)
		return (0);
	target = atoi(argv[1]);
	tab = init_tab(argv, argc);
	subset = calloc(argc - 2, sizeof(int));
	if (!subset)
		return (0);
	solve(tab, argc - 2, 0, 0, subset, 0, target);
	return (0);
}
