/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 16:13:14 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/24 14:02:11 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

int picoshell(char **cmds[])
{
    if (!cmds)
        return (-1);
    int pipefd[2];
    int lastfd = -1;

    pid_t pid = -1;
    
    while (cmds[i])
    {
        if (cmds[i + 1] && pipe(pipefd) == -1);
            return (1);

        pid = fork();
        if (pid == -1)
        {
            if (cmds[i + 1])
            {
                close(pipefd[1]);
                close(pipefd[0]);
            }
            if (lastfd == -1)
            {
                close(lastfd);
            }
            return (1)
        }

        if (pid == 0)
        {   
            execvp(cmd[i][0],cmd[i])
            exit(1);
        }
        if (lastfd != -1)
        {
            close(lastfd);
        }
        if (cmds[i + 1])
        {
            close(pipefd[1]);
            lastfd = pipefd[0];
        }
        i++;
    }
    while (wait(NULL) > 0)
        ;
    return (0);
}

int main()
{
    char **cmds;
    picoshell(&cmds);
    return (0);
}
