#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include "parsing.h"	
#include "serialize.h"
#include "printMsg.h"
#include "client.h"



int mySocket;

void init_socaddr(struct sockaddr_un* addr){
  memset(addr,0, sizeof(*addr));
  addr->sun_family = AF_UNIX;
  strncpy(addr->sun_path, "/tmp/.soquette", sizeof(addr->sun_path)-1);
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
  char* b= serializeInt(argc,1);
  char* c =  serializeTabArg(argc, argv);
  char* send= prepareMsgBeforeSend(a ,b , c);
  int val;
  if(sendData(send)==-1){
    close(mySocket);
    return -1;
  }
  
  val=receiveData();
  close(mySocket);
  return val;
}

int sendData(char * send){
  printf("j'envoi:\n");
  printMsg(send);
  if((write(mySocket, send,(int)strlen(send)))<0){
    perror("write data ");
      return -1;
  }
  return 0;
}



int testReturnValue(char * buffer){
  switch(buffer[0]){
  case APPEL_OK:
    printf("Signal envoyé par la fonction : APPEL OK\n");
    return 0;
  case FONCTION_INCONNUE:
    printf("Signal envoyé par la fonction : FONCTION_INCONNUE\n");
    return 1;
  case MAUVAIS_ARGUMENTS:
    printf("Signal envoyé par la fonction : MAUVAISE_ARGUMENTS\n");
    return 2;
  case PAS_DE_REPONSE:
    printf("Signal envoyé par la fonction : PAS DE REPONSE\n");  
    return 3;
  default:
    printf("buffer %c \n",buffer[0]);
      printf("Signal envoyé par la fonction : ERREUR INCONNUE\n");
    return -1;
  }
}

int receiveData(){
  char buffer[256];
  int val;
  if((read(mySocket,buffer,256))<0){
    perror("read data ");
    return -1;
  }
  val=testReturnValue(buffer);

  if(val==0)
    printReceiveMsg(buffer+1);
  
  return val;
}




int main(int argc,char *argv[]) {
  
  parseArg(argc, argv);
  /*
  int var= 245;
  int var2 = 2; 
  int var3= 300;
  arg a[3];
  a[0].type=1;
  a[0].arg=&var2;
  a[1].type=1;
  a[1].arg=&var;
  a[2].type=1;
  a[2].arg=&var3;
  runClient("merde", 2, a );  */
  return 0;
}
