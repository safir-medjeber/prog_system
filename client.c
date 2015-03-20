#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "serialize.h"

#define ADRESSE "127.0.0.1"
#define PORT 61234


void init_socaddr(struct sockaddr_in* addr){
  memset(addr,'0', sizeof(*addr));
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


int runClient(){
  int socket;
  char buffer[256];
  socket = createSocket();
  
  if (socket==-1) {
    perror("socket problem\n");
    exit(EXIT_FAILURE);
  }
 
  char *n = "salut toi";
  printf("j'envoi le mot : %s\n", n);
  write(socket,n,strlen(n));
  perror("write ");
  read(socket,buffer,256);
  printf("%s", buffer);
  perror("read");
  close(socket);
  exit(EXIT_SUCCESS);

}


int appel_externe(const char *fonction, unsigned short argc, arg argv){
  char* a = serializeString(fonction);
  char* b= serializeInt(argc);
  char* c =  serializeArg(argv);
  
  char* send= prepareMsgBeforeSend(a ,b , c);
  //envoyer le char* send a travers la socket
  printMsg(send);
  printf("\n");
   
  return 0;
  
}

int appel_externe2(const char *fonction, unsigned short argc, arg* argv){
  char* a = serializeString(fonction);
  char* b= serializeInt(argc);
  char* c =  serializeTabArg(argc, argv);
  
  char* send= prepareMsgBeforeSend(a ,b , c);
  //envoyer le char* send a travers la socket
  printMsg(send);
  printf("\n");
   
  return 0;
  
}

int main(int argc,char *argv[]) {

  //  runClient();
  arg tab[3];
  arg a, b, c;
  int var =4356;
  char *var2= "coucou";
  int var3 = 754;
  a.type =2;
  a.arg=var2;
  b.type = 1;
  b.arg = &var;
  c.type=1;
  c.arg=&var3;
  tab[0]=a;
  tab[1]=b;
  tab[2]=c;
  //appeler cette methode dans le client
  //appel_externe("plus", 1, a);
  appel_externe2("plus", 3, tab);
  
}
