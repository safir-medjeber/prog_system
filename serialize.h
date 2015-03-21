#ifndef SERIALIZE_H_INCLUDED

#define SERIALIZE_H_INCLUDED

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
