#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int ids[1024], id, sfd, mx;
char sb[600000], mb[600000], buf[1024][600000];
fd_set fds;

void err(char *s) {
	write(2, s, strlen(s)); exit(1);
}

void cast(int ex) {
	for (int i = 0; i < mx; i++)
		if (i != ex && i != sfd && FD_ISSET(i, &fds))
			send(i, sb, strlen(sb), 0);
	bzero(sb, sizeof(sb));
}

int main(int ac, char **av) {
	if (ac != 2)
		err("Wrong number of arguments\n");

	// ca normalement tu peut le retrouver dans le code qu'ont te donne
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
		err("Fatal error\n");
	
	// ca aussi c'est dans le code qu'ont te donne pas besoin d'apprendre 2130706433 par coeur
	struct sockaddr_in a;
	a.sin_family=AF_INET;
	a.sin_addr.s_addr=htonl(2130706433);
	a.sin_port=htons(atoi(av[1]))};
	if (bind(sfd, &a, sizeof(a)) < 0 || listen(sfd, 10) < 0)
		err("Fatal error\n");

	mx = sfd + 1;
	FD_ZERO(&fds);
	FD_SET(sfd, &fds);
	fd_set rf;

	while (1) {
		rf = fds;
		if (select(mx, &rf, 0, 0, 0) < 0)
			err("Fatal error\n");

		for (int fd = 0; fd < mx; fd++) {
			if (!FD_ISSET(fd, &rf)) 
				continue;

			if (fd == sfd) {
				int c = accept(sfd, 0, 0);
				if (c < 0)
					continue;
				if (c >= mx)
					mx = c + 1;
				FD_SET(c, &fds);
				ids[c] = id++;
				sprintf(sb, "server: client %d just arrived\n", ids[c]);
				cast(c);
				bzero(buf[c], sizeof(buf[c]));
				continue;
			}

			int r = recv(fd, mb, sizeof(mb), 0);
			if (r <= 0) {
				FD_CLR(fd, &fds);
				sprintf(sb, "server: client %d just left\n", ids[fd]);
				cast(fd);
				close(fd);
				bzero(buf[fd], sizeof(buf[fd]));
				continue;
			}

			int x = strlen(buf[fd]);
			for (int i = 0; i < r; i++) {
				if (x < 599999)
					buf[fd][x++] = mb[i];
				if (mb[i] == '\n') {
					buf[fd][x] = 0;
					sprintf(sb, "client %d: %s", ids[fd], buf[fd]);
					cast(fd);
					bzero(buf[fd], sizeof(buf[fd]));
					x = 0;
				}
			}
			
		}
	}
}
