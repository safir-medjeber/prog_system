#ifndef SERVEUR_H_INCLUDED

#define SERVEUR_H_INCLUDED

int createServerSocket();
int connection_handler(int sock);

void timeOut();
void runAlarm(int time);

int runServer();
#endif
