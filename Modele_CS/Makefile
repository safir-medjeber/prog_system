CC=gcc
CFLAGS = -Wall -Wextra

FORCLIENT= client.o serialize.o parsing.o printMsg.o
FORSERVER= serveur.o  printMsg.o serialize.o serverFunctions.o deserialize.o


TARGETS=  client serveur


all: $(TARGETS)


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

client.o : serialize.h parsing.h client.h printMsg.h 

client: $(FORCLIENT)
	$(CC) -o $@ $^ $(CFLAGS)


server.o : serveur.h printMsg.h serialize.h serverFunctions.h deserialize.h
serveur: $(FORSERVER)
	$(CC) -o $@ $^ $(CFLAGS)



clean: 
	rm -rf *~ \#*
clear:
	rm -rf *~ \#* $(ALL) *.o
