/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 17:53:05 by zcadinot          #+#    #+#             */
/*   Updated: 2026/01/21 19:09:08 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 42

int	main(int argc, char **argv)
{
	char	buffer[BUFFER_SIZE];
	char	*content;
	char	*tmp;
	char	*match;
	int		total;
	int		bytes;
	int		len;
	int		i;

	if (argc != 2 || argv[1][0] == '\0')
		return (1);

	content = NULL;
	total = 0;
	while ((bytes = read(0, buffer, BUFFER_SIZE)) > 0)
	{
		tmp = realloc(content, total + bytes);
		if (!tmp)
		{
			free(content);
			perror("Error");
			return (1);
		}
		content = tmp;
		memmove(content + total, buffer, bytes);
		total += bytes;
	}
	if (bytes < 0)
	{
		free(content);
		perror("Error");
		return (1);
	}
	if (!content)
		return (0);

	len = strlen(argv[1]);
	i = 0;
	while (i < total)
	{
		match = memmem(content + i, total - i, argv[1], len);
		if (!match)
		{
			write(1, content + i, total - i);
			break;
		}
		write(1, content + i, match - (content + i));
		while (len--)
			write(1, "*", 1);
		len = strlen(argv[1]);
		i = match - content + len;
	}
	free(content);
	return (0);
}
