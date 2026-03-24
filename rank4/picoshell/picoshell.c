/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 16:13:14 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/24 16:18:59 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

int picoshell(char **cmds[])
{
    if (!cmds)
        return (1);

    int pipefd[2];
    pid_t pid;
    int last_fd = -1;
    
    while (cmd[i])
    {
        if (!cmd[i + 1] && pipe(pipefd) == -1) 
        {
            if (last_fd != -1)
            {
                close(last_fd);
            }
            return (1);
        }

        pid = fork()
        if (pid == -1)
        {
            if (cmd[i + 1])
            {
                close(pipefd[0]);
                close(pipefd[0]);
            }
            if (last_fd != -1)
            {
                close(last_fd);
            }
            return (1);
        }
        if (pid == 0)
        {
            if (last_fd != -1)
            {
                if (dup2(last_fd, 0) == -1)
                {
                    close(last_fd);
                    exit(0);
                }
                close(last_fd);
            }
            if (cmd[i + 1])
            {
                close(fd[0])
                if (dup2(pipefd[1], 1) == -1)
                {
                    close(pipefd[1]);
                    exit(0);
                }
                close(pipefd[1]);
            }
        }
        execvp(cmds[i][0], cmd[i]);
        exit(1);
    }
    if (last_fd != -1)
    {
        close(last_fd);
    }
    if (cmd[i + 1])
    {
        close(pipefd[1]);
        last_fd = pipefd[0];
    }
    while (wait(null) > 0)
        ;
    return (0);
}

int main()
{
    char **cmds;
    picoshell(&cmds);
    return (0);
}
