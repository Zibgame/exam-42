/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:50:53 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/24 13:21:11 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    if (!file || !argv || (type != 'r' && type != 'w'))
    {
        return (-1);
    }

    int pipefd[2];
    pid_t pid = -1;

    if (pipe(pipefd) == -1)
    {
        return (-1);
    }
    pid = fork();
    if (pid == -1)
    {
        return (-1);
    }
    if (pid == 0)
    {
        if (type == 'r')
        {
            if (dup2(pipefd[0],0) == -1)
                exit(-1);
        }
        else if (type == 'w')
        {
            if (dup2(pipefd[1],1) == -1)
                exit(-1);
        }
        else
        {
            close(pipefd[0]);
            close(pipefd[1]);
            exit(-1);
        }
            close(pipefd[0]);
            close(pipefd[1]);
            execvp(file,argv);
            exit(-1);
    }
    else
    {
        if (type == 'r')
        {
            close(pipefd[0]);
            return (pipefd[1]);
        }
    }
    close(pipefd[1]);
    return (pipefd[0]);
}

int main(void)
{
    int fd;
    char buf[256];
    int r;

    fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
    if (fd == -1)
        return (1);

    r = read(fd, buf, 255);
    if (r > 0)
    {
        buf[r] = '\0';
        printf("%s", buf);
    }
    return (0);
}
