#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

static void fatal(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

static void send_all(int socket_fd, const char *buffer, size_t length)
{
	size_t sent = 0;

	while (sent < length)
	{
		ssize_t result = send(socket_fd, buffer + sent, length - sent, 0);

		if (result < 0 && errno == EINTR)
			continue;
		if (result <= 0)
			fatal("send");
		sent += (size_t)result;
	}
}

int main(int argc, char **argv)
{
	int socket_fd;
	struct sockaddr_in address;
	char buffer[4096];
	int stdin_open = 1;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
		fatal("socket");
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons((unsigned short)atoi(argv[1]));
	if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) != 1)
		fatal("inet_pton");
	if (connect(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		fatal("connect");
	printf("Connecte a 127.0.0.1:%s (Ctrl-D pour quitter)\n", argv[1]);

	while (1)
	{
		fd_set read_fds;
		int max_fd = socket_fd;

		FD_ZERO(&read_fds);
		FD_SET(socket_fd, &read_fds);
		if (stdin_open)
		{
			FD_SET(STDIN_FILENO, &read_fds);
			if (STDIN_FILENO > max_fd)
				max_fd = STDIN_FILENO;
		}
		if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0)
		{
			if (errno == EINTR)
				continue;
			fatal("select");
		}
		if (FD_ISSET(socket_fd, &read_fds))
		{
			ssize_t received = recv(socket_fd, buffer, sizeof(buffer), 0);

			if (received < 0 && errno == EINTR)
				continue;
			if (received <= 0)
			{
				printf("Serveur deconnecte.\n");
				break;
			}
			if (write(STDOUT_FILENO, buffer, (size_t)received) < 0)
				fatal("write");
		}
		if (stdin_open && FD_ISSET(STDIN_FILENO, &read_fds))
		{
			ssize_t length = read(STDIN_FILENO, buffer, sizeof(buffer));

			if (length < 0 && errno == EINTR)
				continue;
			if (length < 0)
				fatal("read");
			if (length == 0)
			{
				stdin_open = 0;
				shutdown(socket_fd, SHUT_WR);
			}
			else
				send_all(socket_fd, buffer, (size_t)length);
		}
	}
	close(socket_fd);
	return (EXIT_SUCCESS);
}
