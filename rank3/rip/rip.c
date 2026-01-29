#include <unistd.h>
#include <stdlib.h>

static int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static void	print_solution(char *s)
{
	write(1, s, ft_strlen(s));
	write(1, "\n", 1);
}

static void	count_to_remove(char *s, int *open_rm, int *close_rm)
{
	int	i;
	int	open;

	i = 0;
	open = 0;
	*open_rm = 0;
	*close_rm = 0;
	while (s[i])
	{
		if (s[i] == '(')
			open++;
		else if (s[i] == ')')
		{
			if (open > 0)
				open--;
			else
				(*close_rm)++;
		}
		i++;
	}
	*open_rm = open;
}

static void	backtrack(char *s, char *buf, int i, int balance,
		int open_rm, int close_rm)
{
	if (s[i] == '\0')
	{
		if (balance == 0 && open_rm == 0 && close_rm == 0)
			print_solution(buf);
		return ;
	}
	if (s[i] == '(')
	{
		if (open_rm > 0)
		{
			buf[i] = ' ';
			backtrack(s, buf, i + 1, balance, open_rm - 1, close_rm);
		}
		buf[i] = '(';
		backtrack(s, buf, i + 1, balance + 1, open_rm, close_rm);
	}
	else if (s[i] == ')')
	{
		if (close_rm > 0)
		{
			buf[i] = ' ';
			backtrack(s, buf, i + 1, balance, open_rm, close_rm - 1);
		}
		if (balance > 0)
		{
			buf[i] = ')';
			backtrack(s, buf, i + 1, balance - 1, open_rm, close_rm);
		}
	}
}

int	main(int ac, char **av)
{
	char	*buf;
	int		open_rm;
	int		close_rm;

	if (ac != 2)
		return (1);
	buf = malloc(ft_strlen(av[1]) + 1);
	if (!buf)
		return (1);
	buf[ft_strlen(av[1])] = '\0';
	count_to_remove(av[1], &open_rm, &close_rm);
	backtrack(av[1], buf, 0, 0, open_rm, close_rm);
	free(buf);
	return (0);
}

