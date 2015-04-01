#ifndef DESERIALIZE_H_INCLUDED
#define DESERIALIZE_H_INCLUDED

int veriFonction(char* fonction,int nbArg,int sock);

int* setArg(int a);

char* setArg2(char* a);

char* apply_function(int fonc,arg* argu,int nbArg);

char* deserialize(char* buffer,int* c,int type);// type = 1 pour entier, 2 pour string

void erreur(char err,int sock);

arg* getArg(char* buffer,int nbArg,int c,int sock);

int getNbArg(char*buffer,int * c);

#endif
