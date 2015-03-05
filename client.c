#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define ADRESSE "127.0.0.1"
#define PORT 61234


void init_socaddr(struct sockaddr_in* addr){
  bzero(addr, sizeof(*addr));
  addr->sin_family = AF_INET;
  addr->sin_port = htons(PORT);
  addr->sin_addr.s_addr = inet_addr(ADRESSE);
  }


int createSocket(){
  struct sockaddr_in addr;
  int sock;
  init_socaddr(&addr);
  sock = socket(PF_INET, SOCK_STREAM, 0);

  if(sock == -1){
    perror("Erreur create client");
    return -1;
  }
  
  if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
    perror("Client connect");
    close(sock);
    return -1;
  }
  return sock;
}


int main(int argc,char *argv[]) {
  int socket;
  socket = createSocket();
  
  if (socket==-1) {
    perror("socket problem\n");
    exit(EXIT_FAILURE);
  }
 
  char *n = "salut toi";
  printf("j'envoi le mot : %s\n", n);
  write(socket,n,strlen(n));
  close(socket);
  exit(EXIT_SUCCESS);
}
