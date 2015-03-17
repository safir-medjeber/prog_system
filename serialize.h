#ifndef SERIALIZE_H_INCLUDED

#define SERIALIZE_H_INCLUDED

typedef struct {
  int type;
  void *arg;
} arg;

int taille_int(int n);
char *serializeInt(int entier);
char *serializeString(char* s);
char *serializeArg(arg v);
//int testSerialize();

#endif
