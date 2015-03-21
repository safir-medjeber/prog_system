CC=gcc
CFLAGS1 = -Wall -pthread
CFLAGS2 = -Wall 

FORCLIENT= client.o serialize.o printMsg.o
FORSERVER= serveur.o  printMsg.o


TARGETS=  client serveur


all: $(TARGETS)


%.o: %.c
	$(CC) -c -o $@ $<

client.o : serialize.h client.h printMsg.h

client: $(FORCLIENT)
	$(CC) -o $@ $^ $(CFLAGS2)


server.o : printMsg.h
serveur: $(FORSERVER)
	$(CC) -o $@ $^ $(CFLAGS1)



clean: 
	rm -rf *~ \#*
clear:
	rm -rf *~ \#* $(ALL) *.o
