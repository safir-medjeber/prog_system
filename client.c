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
  char* b= serializeInt(argc);
  char* c =  serializeTabArg(argc, argv);
  char* send= prepareMsgBeforeSend(a ,b , c);
  if(sendData(send)==-1){
    close(mySocket);
    return -1;
  }
  if(receiveData()==-1){
    close(mySocket);
    return -2;
  }
  close(mySocket);

  return 0;
  
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


int receiveData(){
  char buffer[256];
  if((read(mySocket,buffer,256))<0){
    perror("read data ");
    return -1;
  }

  if(buffer[0]==TIMEOUT){
    printf("timeOUT\n");
    return 5;
  }
    
  if(buffer[0]!= APPEL_OK)
    return -1;
  
  else{
      printf("Signal envoyé par la fonction : %c\n",buffer[0]);
      printf("Valeur de retour : ");
      printVar(buffer+1);
      printf("\n");
  }

  return 0;
}


int parsing(int argc, char *argv[]){
  int i=0 , j=0, k=0, nbrArg[512];
  char *nameFunc, *tmpArg;
  
  if(argc==1)        // Si aucun nom de fonction
    printf("Pas de fonction a appeler\n");
  else {             // Si on donne un nom de fonction
    nameFunc = argv[1]; //On prend le nom de la fonction
    if(argc==2){        //Si la fonction ne prend pas d'arguments   
      arg a[1];
      a[0].type = 0;
      a[0].arg = NULL;
      runClient(nameFunc, 1, a);
    }
    else { //Si la fonction a des arguments
      arg a[argc-2];
      for(i=2;i<argc;i++){
	tmpArg = argv[i];
	if(tmpArg[0]>='0' || tmpArg[0]<='9'){ // Si l'argument est un nombre
	  nbrArg[k] = atoi(tmpArg);
	  a[j].type = 1;
	  a[j].arg = &nbrArg[k];
	  k++;
	  j++;
	}
	else 
	  if ((tmpArg[0]>='A' && tmpArg[0]<='Z') || 
	      (tmpArg[0]>='a' && tmpArg[0]<='z')){ // Si l'argument est un string
	    //Pas encore testé car on a pas encore de fonctions sur les strings
	    a[j].type = 2;
	    a[j].arg = &tmpArg;
	    j++;
	  }
      }
      runClient(nameFunc, argc-2, a);   
    }
  }	
  return 0;
}

int main(int argc,char *argv[]) {
  
  parsing(argc, argv);

  /*  int var= 1350;
  int var2 = 350; 
  int var3= 300;
  arg a[3];
  a[0].type=1;
  a[0].arg=&var2;
  a[1].type=1;
  a[1].arg=&var;
  a[2].type=1;
  a[2].arg=&var3;
  runClient("plus", 3, a );  */
}
