#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#include "printMsg.h"

#define ADRESSE "127.0.0.1"
#define PORT 61234

// Gestionnaire de connection pour un client
void *connection_handler(void *socket_desc){
  int lus, i;
  int sock = *(int*)socket_desc;
  char buffer[1024];
  char *msg = "votre msg a ete recu par le serveur\n";
  
  while ((lus=read(sock,buffer,256))>0) {
    printMsg(buffer);
    
    write(sock , msg , strlen(msg));
    perror("write");
    close(sock);
    printf("socket fermee\n");
  }
  
  printf("thread terminee\n");

  return NULL;
}


void init_socaddr(struct sockaddr_in* addr){
  memset(addr,'0', sizeof(*addr));
  addr->sin_family = AF_INET;
  addr->sin_port = htons(PORT);
  addr->sin_addr.s_addr = inet_addr(ADRESSE);
}

int createServerSocket(){
  struct sockaddr_in addr;
  int sock;
  init_socaddr(&addr);

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1){
    perror("Serveur socket");
    return -1;
  }
  if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){ 
    perror("Serveur bind");
    return -1;
  }
  if(listen(sock, 0) == -1){
    perror("Seveur listen");
    return -1;
  }
  
  return sock;
}



int main(int argc,char *argv[]) {
  struct sockaddr_in addr;
  struct sockaddr_in c;
  int sock,client_sock;
  unsigned int l;
  pthread_t th;

  init_socaddr(&addr);
  sock = createServerSocket(); 

   if (sock==-1) {
    perror("socket problem\n");
    exit(EXIT_FAILURE);
  }
 
  while ((client_sock=accept(sock,(struct sockaddr *)&c,&l))) {
    printf("connection du client acceptee\n");
    if( pthread_create( &th, NULL, connection_handler, (void*) &client_sock) < 0){
      perror("could not create thread ");
      return 1;
    }

  }
  if (client_sock < 0) {
    perror("accept failed");
    return 1;
  }
    perror("boucle" );
  return 0;
}

