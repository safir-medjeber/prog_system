CC=gcc
CFLAGS = -Wall -Wextra

FORCLIENT= clientServer.o client.o serialize.o parsing.o printMsg.o serveur.o  serverFunctions.o deserialize.o


TARGETS=  clientServer


all: $(TARGETS)


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

client.o : serialize.h parsing.h client.h printMsg.h
server.o : serveur.h printMsg.h serialize.h serverFunctions.h deserialize.h


clientServer: $(FORCLIENT)
	$(CC) -o $@ $^ $(CFLAGS)






clean: 
	rm -rf *~ \#*
clear:
	rm -rf *~ \#* $(ALL) *.o
