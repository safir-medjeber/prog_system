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
int* resultat;
void init_socaddr_client(struct sockaddr_un* addr,char* soquette){
  memset(addr,0, sizeof(*addr));
  addr->sun_family = AF_UNIX;
  strncpy(addr->sun_path, soquette, sizeof(addr->sun_path)-1);
}


int createSocket(char* soquette){
  struct sockaddr_un addr;
  int sock;
  sock = socket(AF_UNIX, SOCK_STREAM, 0);
  init_socaddr_client(&addr,soquette);
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


int* runClient(char *fonction, int argc, arg *argv,char* soquette){
  mySocket = createSocket(soquette);
  if(mySocket==-1) {
    perror("socket problem\n");
    exit(EXIT_FAILURE);
  }

  if((appel_externe(fonction , argc, argv)) != 0)
	  return NULL;
 
  return resultat;
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

int* getInt(char* buffer){
	int j,taille,estNegatif;
	int c = 0;
	estNegatif=1;
	char* nbArgTmp;
	int* resultat = malloc(sizeof(int)); 
	if(buffer[c]==1 || buffer[c]==3){//l'argument est un entier
		if(buffer[c]==3){
			estNegatif=-1;
		}
		else{
			estNegatif=1;
		}
		taille=buffer[++c];
		nbArgTmp=malloc(taille*(sizeof(char))+1);
		for(j=0;j<taille;j++){
			nbArgTmp[j]=buffer[++c];
		}
		nbArgTmp[j]='\0';
		*resultat=atoi(nbArgTmp)*estNegatif;
		free(nbArgTmp);
		return resultat;
	}
	else return NULL;
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
    {printReceiveMsg(buffer+1);
  	resultat=getInt(buffer+1);
	}
  return val;
}


