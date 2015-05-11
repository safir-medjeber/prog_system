#ifndef CLIENT_H_INCLUDED

#define CLIENT_H_INCLUDED


void init_socaddr_client(struct sockaddr_un* addr);
int createSocket();

int runClient(char *fonction, int argc, arg *argv);
int testReturnValue(char * buffer);

int appel_externe(const char *fonction, unsigned short argc,  arg* argv);
int sendData(char *send);
int receiveData();



#endif
