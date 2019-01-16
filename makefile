CFLAG= -Wall -std=c99
LIBS= -pthread
TARGET= client server
.PHONY= all
.DEFAULT_GOAL= all

all: $(TARGET)

client: client.o util.o
	gcc $(LIBS) -o client client.o util.o

client.o: client.c
	gcc -c client.c

util.o: util.h util.c
	gcc -c util.c

server: server.o util.o
	gcc $(LIBS) -o server server.o util.o

server.o: server.c
	gcc -c server.c util.c

clean:
	rm -rf *.o $(TARGET)

