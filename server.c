#include <sys/types.h>
#include <sys/socket.h>
#include <err.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "util.h"

#define WIDTH 100

int	bind_to(char *);
int	accept_conn(int fd);

void usage() {
	extern char *__progname;
	fprintf(stdout, "Usage: %s port username\n", __progname);
	exit(1);
}

int main(int argc, char *argv[]) {
	
	if (argc != 3)
		usage();

	char *port = argv[1];
	char *username = argv[2];

	int fd = bind_to(port);
	int conn = accept_conn(fd);
	start_talking(conn, username);
	
	close(fd);
	return 0;
}

int bind_to(char *port) {
	struct addrinfo hints;
	struct addrinfo *rp, *result;
	int s, sfd;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;    	/* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address*/

	s = getaddrinfo(NULL, port, &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,
		    rp->ai_protocol);
		if (sfd == -1)
			continue;

		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;                  /* Success */

		close(sfd);
	}

	if (rp == NULL) {               /* No address succeeded */
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(result);           /* No longer needed */
	return sfd;
}

/* accept incoming connections */
int accept_conn(int fd) {
	int error;
	error = listen(fd, 5);
	if (error == -1) {
		fprintf(stderr, "failed listen\n");
		exit(1);
	}

	int conn = accept(fd, NULL, NULL);
	if (conn == -1) {
		fprintf(stderr, "failed accept\n");
		exit(1);
	}

	return conn;
}
