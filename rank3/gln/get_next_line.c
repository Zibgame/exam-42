/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:24:36 by zcadinot          #+#    #+#             */
/*   Updated: 2026/01/22 17:14:59 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr(char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (s[i] == (char)c)
			return (s + i);
		i++;
	}
	if ((char)c == '\0')
		return (s + i);
	return (NULL);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if ((!dest && !src) && n > 0)
		return (NULL);
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}

size_t	ft_strlen(char *s)
{
	size_t	len;

	len = 0;
	if (!s)
		return (0);
	while (s[len])
	{
		len++;
	}
	return (len);
}

int	str_append_mem(char **s1, char *s2, size_t size2)
{
	char	*tmp;
	
	if (!s1 && (!s2 && size2 > 0))
		return (0);
	tmp = malloc((ft_strlen(*s1) + ft_strlen(s2)) + 1);
	if (!tmp)
	{
		return (0);
	}
	ft_memcpy(tmp, *s1, ft_strlen(*s1));
	ft_memcpy(tmp + ft_strlen(*s1), s2, size2);
	tmp[(ft_strlen(*s1) + size2)] = '\0';
	free(*s1);
	*s1 = tmp;
	return (1);
}

int	str_append_str(char **s1, char *s2)
{
	return (str_append_mem(s1, s2, ft_strlen(s2)));
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (dest < src)
	{
		i = 0;
		while (i < n)
		{
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
			i++;
		}
		return (dest);
	}
	else if (dest > src)
	{
		i = n;
		while (i > 0)
		{
			i--;
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		}
		return (dest);
	}
	return (dest);
}

char	*get_next_line(int fd)
{

	static char	*str = NULL;
	char	buffer[BUFFER_SIZE + 1];
	char	*line;
	long	i;
	long	byte;

	byte = 0;
	line = NULL;
	while (!str || !ft_strchr(str, '\n'))
	{
		byte = read(fd, buffer, BUFFER_SIZE);
		if (byte <= 0)
			break ;
		buffer[byte] = '\0';
		str_append_str(&str, buffer);
	}
	if (!str)
		return (NULL);
	i = 0;
	while (str && str[i] && str[i] != '\n')
	{
		i++;
	}
	if (str && str[i] == '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	line = ft_memcpy(line, str, i);
	line[i] = '\0';
	ft_memmove(str, &str[i], ft_strlen(str) - i + 1);
	if (str[0] == '\0')
	{
		free(str);
		str = NULL;
	}
	return (line);
}
