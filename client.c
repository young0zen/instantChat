#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "util.h"

int      connect_to(char *host, char *port);

void
usage()
{
        extern char *__progname;
	fprintf(stdout, "Usage: %s host port username\n", progname);
	exit(1);
}

int
main(int argc, char *argv[])
{
	if (argc != 4)
		usage();

	char *host = argv[1];
	char *port = argv[2];
	char *username = argv[3];

	int fd = connect_to(host, port);
	start_talking(fd, username);
	close(fd);

	return 0;
}

int
connect_to(char *host, char *port)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;    /* Allow IPv4 only*/
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */

	s = getaddrinfo(host, port, &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,
	    	    rp->ai_protocol);
		if (sfd == -1)
			continue;

		if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;                  /* Success */

		close(sfd);
	}

	if (rp == NULL) {               /* No address succeeded */
		fprintf(stderr, "Could not connect\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(result);           /* No longer needed */
	return sfd;			/* return fd */
}
