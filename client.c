#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>	
#include "serialize.h"
#include "printMsg.h"
#include "client.h"
#define ADRESSE "127.0.0.1"
#define PORT 61234

int mySocket;

void init_socaddr(struct sockaddr_un* addr){
  memset(addr,0, sizeof(*addr));
  addr->sun_family = AF_UNIX;
  strncpy(addr->sun_path, getenv("HOME"), sizeof(addr->sun_path)-1);
  strncat(addr->sun_path, "/", sizeof(addr->sun_path)-1);
  strncat(addr->sun_path, ".soquette", sizeof(addr->sun_path)-1);
}


int createSocket(){
  struct sockaddr_un addr;
  int sock;
  sock = socket(AF_UNIX, SOCK_STREAM, 0);
  init_socaddr(&addr);
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
  int var2 = 7; 

  arg a[2];
  a[0].type=1;
  a[0].arg=&var2;
  a[1].type=1;
  a[1].arg=&var;


  runClient("multiplie", 2, a );  
}
