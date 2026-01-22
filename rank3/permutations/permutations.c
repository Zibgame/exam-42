/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permutations.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 22:36:25 by zcadinot          #+#    #+#             */
/*   Updated: 2026/01/22 23:31:35 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int	ft_strlen(char *s)
{
	int	i = 0;

	while (s[i])
		i++;
	return (i);
}

void	solve(char *set, int len, int depth, char *current, int *used)
{
	int	i;

	
	if (depth == len)
	{
		write(1, current, len);
		write(1, "\n", 1);
		return ;
	}
	i = 0;
	while (i < len)
	{
		if (!used[i])
		{
			used[i] = 1;
			current[depth] = set[i];
			solve(set, len, depth + 1, current, used);
			used[i] = 0;
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	char	*current;
	int		*used;
	int		len;

	if (argc != 2)
		return (1);

	len = ft_strlen(argv[1]);

	current = malloc(sizeof(char) * (len + 1));
	if (!current)
		return (1);

	used = calloc(len, sizeof(int));
	if (!used)
		return (1);

	solve(argv[1], len, 0, current, used);

	free(current);
	free(used);
	return (0);
}
