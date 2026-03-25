/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 16:13:14 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/25 14:06:55 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int exec_cmd(char **cmd, int in, int out)
{
	if (in != -1)
	{
		if (dup2(in, 0) == -1)
		{
			exit(1);
		}
		close(in);
	}
	if (out != -1)
	{
		if (dup2(out, 1) == -1)
		{
			exit(1);
		}
		close (out);
	}
	execvp(cmd[0], cmd);
	exit(1);
}

int closer(int last, int *fd,int in_pipe)
{
    if (last != -1)
    {
        close(last);
    }
    if (in_pipe)
    {
        close(fd[1]);
        close(fd[0]);
    }
    return (1);
}

int	picoshell(char **cmds[])
{
	int		i = 0;
	int		fd[2];
	int		last = -1;
	pid_t	pid = -1;

    while (cmds[i])
    {
        if (cmds[i + 1] && pipe(fd) == -1)
        {
            closer(last, fd, 0);
        }
        pid = fork();
        if (pid == -1)
            return(closer(last, fd, cmds[i + 1] != NULL));

        if (pid == 0)
        {
            if (cmds[i + 1])
            {
				exec_cmd(cmds[i], last, fd[1]);
            }
			else
				exec_cmd(cmds[i], last, -1);
        }
        closer(last, fd, 0);
        if (cmds[i + 1])
        {
            close(fd[1]);
            last = fd[0];
        }
        i++;
    }
    while (wait(NULL) > 0)
        ;
    return (0);
}

static int	count_cmds(int ac, char **av)
{
	int	i;
	int	count;

	i = 1;
	count = 1;
	while (i < ac)
	{
		if (av[i][0] == '|' && av[i][1] == '\0')
			count++;
		i++;
	}
	return (count);
}

static char	***build_cmds(int ac, char **av)
{
	int		i;
	int		j;
	int		k;
	char	***cmds;

	cmds = malloc(sizeof(char **) * (count_cmds(ac, av) + 1));
	if (!cmds)
		return (NULL);
	i = 1;
	k = 0;
	while (i < ac)
	{
		j = i;
		while (j < ac && !(av[j][0] == '|' && av[j][1] == '\0'))
			j++;
		cmds[k] = &av[i];
		if (j < ac)
			av[j] = NULL;
		i = j + 1;
		k++;
	}
	cmds[k] = NULL;
	return (cmds);
}

int	main(int ac, char **av)
{
	char	***cmds;
	int		ret;

	if (ac < 2)
		return (1);
	cmds = build_cmds(ac, av);
	if (!cmds)
		return (1);
	ret = picoshell(cmds);
	free(cmds);
	return (ret);
}
