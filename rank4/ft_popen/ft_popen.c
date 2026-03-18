/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 13:23:06 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/18 14:07:04 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    if (!argv || !file || (type != 'r' && type != 'w'))
    {
        printf("Error:\n");
        return (-1);
    }
    int pipefd[2];
    char buf[100];
    pid_t pid;

    pipe(pipefd);

    /* write(pipefd[1], "s", 1); */
    /* read(pipefd[0], buf,1); */
    /* printf("%s", buf); */

    pid = fork();
    if (pid == -1)
    {
        return (-1);
    }
    if (pid == 0)
    {
        if (type == 'r')
        {
            dup2(pipefd[1], 1);
            close(pipefd[0]);
            close(pipefd[1]);
            execvp(file, argv);
            return (pipefd[0]);
        }
    }
    else
    {
        if (type == 'r')
        {
            close(pipefd[1]);
            return (pipefd[0]);
        }
    }
    return (0);
}

int main(int argc, char **argv)
{
    char buf[100];
    int fd;
    fd = ft_popen("ls", (char *const []){"ls", "-LR", NULL}, 'r');
    read(fd, buf, 100);
    printf("%s", buf);
    return (0);
}
