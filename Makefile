CC=gcc
CFLAGS1 = -Wall -pedantic -std=c99 -D _POSIX_C_SOURCE=200112L -pthread
CFLAGS2 = -Wall -pedantic -std=c99 -D _POSIX_C_SOURCE=200112L 

FORCLIENT= client.o serialize.o
FORSERVER= serveur.o


ALL=  client serveur


all: $(ALL)


%.o: %.c
	$(CC) -c -o $@ $<

client.o : serialize.h

client: $(FORCLIENT)
	$(CC) -o $@ $^ $(CFLAGS2)

serveur: $(FORSERVER)
	$(CC) -o $@ $^ $(CFLAGS1)



clean: 
	rm -rf *~ \#*
clear:
	rm -rf *~ \#* $(ALL) *.o
