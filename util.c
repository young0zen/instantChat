#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include "util.h"

void
add_name(char *msg, char *name)
{
        char buff[WIDTH + 20] = {0};
        sprintf(buff, "%s: ", name);
        strcpy(buff + strlen(name) + 2, msg);
        strcpy(msg, buff);
}

/* get user input and forward through socket */                                 
void
talk_to(int writeFd, char *name)
{
        /* talk to peer */
        char *buffer = calloc(1, sizeof(char) * WIDTH);                         
        int error;
        size_t wlen = WIDTH;

        while (1) {                                                             
                memset(buffer, 0, WIDTH);                                       
                error = getline(&buffer, &wlen, stdin);                         

                if (error < 0 || strcmp("END\n", buffer) == 0)
                        break;  /* end of input */                              

                add_name(buffer, name);
                write(writeFd,buffer, WIDTH);
        }
                                                                                
        free(buffer);
        close(writeFd);
}

/* start talking using two threads, one for write, one for read */
void
start_talking(int conn, char *name)
{
        pthread_t tid;
        int readFd, writeFd, error;

        readFd = conn;
        writeFd = dup(conn);

        error = pthread_create(&tid, NULL, communicate,
            (void *)(long)readFd);
        if (error != 0) {
                fprintf(stderr, "pthread error\n");
                exit(1);
        }

        talk_to(writeFd, name);

	pthread_kill(tid, SIGINT); 
	/* if writing thread ends early, stop reading thread */
              
        void *retval;
        error = pthread_join(tid, &retval);
}


/* read from socket and print to stdout, thread routine*/                       
void *
communicate(void *arg)
{
	size_t rlen;
        int conn = (int)(long)arg;
        char *buffer = malloc(sizeof(char) * WIDTH + 1);

        while (1) {
                memset(buffer, 0, WIDTH + 1);
                rlen = read(conn, buffer, WIDTH);
                if (rlen == 0) {
			fprintf(stderr, "peer quited, "
			    "press control + c to quit\n");
                        break;
		}
                else if (rlen < 0)
                        err(1, "read");

                fprintf(stdout, "%s", buffer);
        }

        free(buffer);
        close(conn);
        return NULL;
}                                                                               
