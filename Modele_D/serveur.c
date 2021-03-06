#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>

#include "serveur.h"
#include "errno.h"
#include "printMsg.h"
#include "serialize.h"
#include "deserialize.h"
#include "serverFunctions.h"

#define SOQUETTE "/tmp/.soquette"
int mySocket;

void timeOut () {
  printf("temps ecoule 2 sec \n");
  char* res ="3";
  write(mySocket,res,strlen(res));
  close(mySocket);
  exit(0);
}

void runAlarm(int time){
  struct sigaction act ;
  sigemptyset (& act.sa_mask ) ;
  act.sa_flags = SA_SIGINFO ;
  act.sa_sigaction = timeOut ;
  if(sigaction(SIGALRM, &act, NULL) == (int)SIG_ERR) {
    perror ("sigaction");
  }
  alarm(time);
}

// Gestionnaire de connection pour un client
int connection_handler(int sock){
  
  int lus, nbArg,c,fonction;
  char buffer[1024];// la ou on va stoquer les messages transmis par le socket
  char* nomFonction;// la ou on va stoquer le nom de la fonction demander
  char* res; // la ou on va stoquer le resultat a retourner au client
  mySocket=sock;
  c=0;
  
  while ((lus=read(sock,buffer,256))>0) {


    runAlarm(2);
    printf("je lis: \n");
    printMsg(buffer);
      nomFonction=deserialize(buffer,&c,2);
      printf("la fonction est:%s \n",nomFonction);
      // on deserialise le nombre d'argument
      nbArg=getNbArg(buffer,&c);
      printf("jai %d arguments\n",nbArg);
      fonction=veriFonction(nomFonction,nbArg,sock);
      if( fonction < 0){
		  return -1;
      }
      //TODO ici mettre la condition sur le nombre d'arguements que l'on veut
      arg* tabArg=getArg(buffer,nbArg,c,sock);// tableau ou l'on va stoquer tous les arguments sous forme de struct arg
      if(tabArg==NULL){
	return -1;
      }
      // tout va bien la serialisation s'est bien deroulee on appelle la fonction demande
      res=apply_function(fonction,tabArg,nbArg);
      write(sock,res,strlen(res));
      free(nomFonction);
      free(res);
      free(tabArg);
      break;	// mais qu'est ce que ce break fout la ?	
    close(sock);
    printf("socket fermee\n");
  }
  printf("Processus termine\n");
  return 0;
}




void init_socaddr(struct sockaddr_un* addr,char* sock){
  memset(addr,'0', sizeof(*addr));
  addr->sun_family = AF_UNIX;

  if(unlink(sock)<0 && errno!=ENOENT){
    perror("unlink error dude");
    exit(-1);
  } 
  strncpy(addr->sun_path, sock,sizeof(addr->sun_path)-1 );
}



int createServerSocket(char* soquette){
  struct sockaddr_un addr;
  int sock;
  init_socaddr(&addr,soquette);
  sock = socket(AF_UNIX, SOCK_STREAM, 0);
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
/*
int runServer(){
 struct sockaddr_un addr;
  int sock,client_sock, pid;

  init_socaddr(&addr);
  sock = createServerSocket(); 
  if (sock==-1) {
    perror("socket problem\n");
    exit(EXIT_FAILURE);
  }
 
  while ((client_sock=accept(sock,NULL,NULL))) {
    if (client_sock < 0) {
      perror("accept failed");
      return 1;
    }
 
    printf("connection du client acceptee\n");
    pid=fork();
    if(pid==0){
      exit(connection_handler(client_sock));
    }

  }
  perror("boucle" );
  return 0;
}

int main() {
  runServer();
}
*/
