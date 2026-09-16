/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 00:21:52 by zcadinot          #+#    #+#             */
/*   Updated: 2026/09/16 02:22:28 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void err(char *str)
{
	write(2, str, strlen(str));
	exit(1);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		err("Wrong number of arguments\n");
		return (0);
	}

	// attribuer le fd du socket
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		err("Fatal error\n");

	// cree la struct qui renseigne les setting
	struct sockaddr_in a;
	a.sin_family = AF_INET; // IPV4
	a.sin_addr.s_addr= htonl(INADDR_LOOPBACK); // man 7 ip pour avoir le define du loopback
	a.sin_port = htons(atoi(argv[1])); 

	// init le server avec bind
	if (bind(server_fd, (struct sockaddr *)&a, sizeof(a)) < 0)
		err("Fatal error\n");

	// mettre le serve en ecoute sur 
	if (listen(server_fd, 10) < 0)
		err("Fatal error\n");

	// en gros tu prepare des stocker de fd un pour la list est un pour se qui on un event
	fd_set list_fd; // list des fd a ecouter
	fd_set ready_fd; //list des fd qui on un event est qui faut traiter
	FD_ZERO(&list_fd); // tu mais ta list a 0;
 	FD_SET(server_fd, &list_fd); // tu ajoute server_fd dans la list
	int client_fd = 0;
	int client = 0;
	int max_fd = server_fd;
	int client_id[1024];
	int next_id = 0;
	char	message[100];

	while (1)
	{
		ready_fd = list_fd; // tu copie tout dans ready pour pas repeter
		if (select(max_fd + 1, &ready_fd, 0, 0, 0) < 0) // et ensuite tu select sur server + 1 car commence a 0 est tu ajoute les ready dans la list ready
			err("Fatal error\n");

		client = 0;
		while (client <= max_fd)
		{
			if (FD_ISSET(client, &ready_fd))
			{
				// si une personne veut join
				if (client == server_fd)
				{
					client_fd = accept(server_fd, 0, 0);	
					if (client_fd >= 0)
					{
						if (client_fd > max_fd)
							max_fd = client_fd;
						FD_SET(client_fd, &list_fd);
						client_id[client_fd] = next_id++;
						sprintf(message, "server: client %d just arrived\n", client_id[client_fd]);
						int fd = 0;

						while (fd <= max_fd)
						{
							if (FD_ISSET(fd, &list_fd)
									&& fd != server_fd
									&& fd != client_fd)
								send(fd, message, strlen(message), 0);
							fd++;
						}
					}
				}

			}
			client++;
		}
	}
	return (0);
}
