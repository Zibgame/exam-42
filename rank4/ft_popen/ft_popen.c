/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:50:53 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/18 15:51:59 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    if (!argv || !file || (type != 'r' && type != 'w'))
        return (-1);

    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1)
        return (-1);

    pid = fork();
    if (pid == -1)
        return (-1);
    if (pid == 0)
    {
        if (type == 'r')
        {
            if (dup2(pipefd[1], 1) == -1)
                exit(-1);
        }
        if (type == 'w')
        {
            if (dup2(pipefd[0], 0) == -1)
                exit(-1);
        else
        {
            close(pipefd[1]);
            close(pipefd[0]);
            exit(-1);
        }
        close(pipefd[1]);
        close(pipefd[0]);
        execvp(file, argv);
        exit(1);
    }
    else
    {
        if (type == 'r')
        {
            close(pipefd[1]);
            return (pipefd[0]);
        }
    }
    close(pipefd[0]);
    return (pipefd[1]);
}

