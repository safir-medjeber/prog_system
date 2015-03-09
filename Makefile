CC=gcc
CFLAGS = -Wall -pedantic -std=c99 -D _POSIX_C_SOURCE=200112L -pthread
%:: %.c
	$(CC) $(CFLAGS) $< -o $@
ALL=  client serveur


all: $(ALL)

client: client.c
serveur: serveur.c

clean: 
	rm -rf *~ \#*
clear:
	rm -rf *~ \#* $(ALL) *.o
