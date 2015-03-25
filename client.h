#ifndef CLIENT_H_INCLUDED

#define CLIENT_H_INCLUDED


void init_socaddr(struct sockaddr_un* addr);
int createSocket();
int runClient(char *fonction, int argc, arg *argv);

int appel_externe(const char *fonction, unsigned short argc,  arg* argv);
int sendData(char *send,  unsigned short argc );
#endif
