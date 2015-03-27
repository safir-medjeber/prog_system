#ifndef DESERIALIZE_H_INCLUDED
#define DESERIALIZE_H_INCLUDED

int veriFonction(char* fonction);

int* setArg(int a);

char* setArg2(char* a);

char* apply_function(int fonc,arg* argu,int nbArg);

char* getNomFonction(char* buffer,int taille,int c);

void erreur(char err,int sock);

arg* getArg(char* buffer,int nbArg,int c,int sock);


#endif
