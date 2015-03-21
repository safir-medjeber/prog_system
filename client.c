#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "serialize.h"
#include "printMsg.h"
#include "client.h"

#define ADRESSE "127.0.0.1"
#define PORT 61234

int mySocket;

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




int runClient(char *fonction, int argc, arg *argv){
  mySocket = createSocket();
  if(mySocket==-1) {
    perror("socket problem\n");
    exit(EXIT_FAILURE);
  }

  appel_externe(fonction , argc, argv);
 
  return 0;
}



int appel_externe(const char *fonction, unsigned short argc, arg * argv){
  char* a = serializeString(fonction);
  char* b= serializeInt(argc);
  char* c =  serializeTabArg(argc, argv);
  char* send= prepareMsgBeforeSend(a ,b , c);
  sendData(send, argc);
  
   
  return 0;
  
}

int sendData(char * send, unsigned short argc){
  char buffer[256];
  printf("j'envoi:\n");
  printMsg(send);
  write(mySocket, send, strlen(send));
  perror("write ");
  read(mySocket,buffer,256);
  printf("%s", buffer);
  perror("read");
  close(mySocket);
  return 0;
}


int main(int argc,char *argv[]) {
  
  int var= 2;
  char * var2 = "coucou"; 

  arg a[2];
  a[0].type=2;
  a[0].arg=var2;
  a[1].type=1;
  a[1].arg=&var;


  runClient("plus", 2, a );  
}
