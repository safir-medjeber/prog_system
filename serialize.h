#ifndef SERIALIZE_H_INCLUDED

#define SERIALIZE_H_INCLUDED
#define PAS_DE_REPONSE '1'
#define MAUVAIS_ARGUMENTS '2'
#define APPEL_OK '3'
#define FONCTION_INCONNUE '4'
typedef struct {
  int type;
  void *arg;
} arg;

int taille_int(int n);
char *serializeInt(int entier);
char *serializeString(const char* s);
char *serializeArg(arg v);
char *serializeTabArg(unsigned short argc, arg* v);

char * prepareMsgBeforeSend(char* fonction, char* argc, char* structArg);
void  printMsg(char * send);
  char* printVar(char* ptr);

//int appel_externe(const char *fonction, unsigned short argc,  arg argv);

#endif
