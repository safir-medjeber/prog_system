#ifndef SERIALIZE_H_INCLUDED
#define SERIALIZE_H_INCLUDED

#define APPEL_OK '0'
#define FONCTION_INCONNUE '1'
#define MAUVAIS_ARGUMENTS '2'
#define PAS_DE_REPONSE '3'
#define ERREUR_TRANSMIT '4'
#define ERREUR_INCONNUE '5'

typedef struct {
  int type;
  void *arg;
} arg;

char *serializeInt(int entier);
char *serializeString(const char* s);
char *serializeArg(arg v);
char *serializeTabArg(unsigned short argc, arg* v);

char * prepareMsgBeforeSend(char* fonction, char* argc, char* structArg);


#endif
