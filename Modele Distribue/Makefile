test: polonaise.o stack.o shmit2.o
	gcc -o test polonaise.o stack.o shmit2.o

polonaise.o: Polonaise.c Polonaise.h
	gcc -o Polonaise.o -c Polonaise.c -W -Wall 

stack.o: stack.c stack.h
	gcc -o stack.o -c stack.c -W -Wall 

shmit2.o: Polonaise.h stack.h shmit2.c
	gcc -o shmit2.o -c shmit2.c -W -Wall