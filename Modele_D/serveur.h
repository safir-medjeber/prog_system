#ifndef SERVEUR_H_INCLUDED

#define SERVEUR_H_INCLUDED

int createServerSocket(char * sock);
int connection_handler(int sock);
void init_socaddr(struct sockaddr_un* addr,char* sock);
void timeOut();
void runAlarm(int time);

int runServer();
#endif
