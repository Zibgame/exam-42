/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 17:53:05 by zcadinot          #+#    #+#             */
/*   Updated: 2026/01/28 11:29:23 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 42

int	main(int argc, char **argv)
{
	char	buffer[BUFFER_SIZE];
	char	*str = NULL;
	char	*filter;
	size_t	byte;
	size_t	i = 0;
	size_t	j = 0;
	size_t	k = 0;
	size_t	total = 0;
	
	if (argc != 2 || !argv[1] || !argv[1][0])
		return (1);
	while ((byte = read(0, buffer, BUFFER_SIZE)) > 0)
	{
		str = realloc(str,total + byte + 1);
		memmove(str + total, buffer, byte);
		total += byte;
	}
	if (!str)
		return (0);
	str[total] = '\0';
	// printf("%s", str);
	filter = argv[1];
	while (str[i])
	{
		j = 0;
		k = i;
		while (str[k] == filter[j])
		{
			j++;
			k++;
		}
		if (j == strlen(filter))
		{
			j = 0;
			while (j < strlen(filter))
			{
				write(1, "*", 1);
				j++;
			}
			i += strlen(filter);
		}
		else
		{
				write(1, &str[i], 1);
			i++;
		}
	}
	return (0);
}
