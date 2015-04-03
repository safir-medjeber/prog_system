#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>

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


// Gestionnaire de connection pour un client
int connection_handler(int sock){
  
  int lus, nbArg,taille,c,fonction;
  char buffer[1024];// la ou on va stoquer les messages transmis par le socket
  char* nomFonction;// la ou on va stoquer le nom de la fonction demander
  char* nbArgTmp;
  char* res; // la ou on va stoquer le resultat a retourner au client
  //char *msg = "votre msg a ete recu par le serveur\n";
  mySocket=sock;

 
  while ((lus=read(sock,buffer,256))>0) {

    if (signal( SIGALRM , timeOut) == SIG_ERR )
      perror( " cant catch SIGALRM " ) ;
     alarm(2);
     printf("alarme lancee\n");
   
    
  
    printf("je lis: \n");
    printMsg(buffer);
    if(buffer[0] !=2){// si on a pas transmis les infos sur la fonction a utiliser
      perror("erreur le nom de la function n'est pas transmis");
    }
    else{//ok on a donne le nom de la fonction a utiliser
      c=1;
      taille=buffer[c];
      nomFonction=getNomFonction(buffer,taille,c);
      c+=taille;
      fonction=veriFonction(nomFonction);
      /* fonction represente un numero associe a la fonction que l'on va utiliser dans un switch pour faire
	 le bon appel de fonction plus bas*/
      printf("la fonction est:%s \n",nomFonction);
      if( fonction< 0){
	erreur(FONCTION_INCONNUE,sock);
	return -1;
      }
      c++;
      // on deserialise le nombre d'argument
      if(buffer[c]!=1){
	erreur(MAUVAIS_ARGUMENTS,sock);
	return -1;
      }
      taille=buffer[++c];
      nbArgTmp=getNomFonction(buffer,taille,c);
      c+=taille;
      nbArg=atoi(nbArgTmp);
      printf("jai %d arguments\n",nbArg);
      //TODO ici mettre la condition sur le nombre d'arguements que l'on veut
      arg* tabArg=getArg(buffer,nbArg,c,sock);// tableau ou l'on va stoquer tous les arguments sous forme de struct arg
      if(tabArg==NULL){
	return -1;
      }
      printf("\n 1er arg:%d \n 2eme arg:%d \n",*((int*)(tabArg[0].arg)),*((int*)(tabArg[1].arg)));
      // tout va bien la serialisation s'est bien deroulee on appelle la fonction demande
      res=apply_function(fonction,tabArg,nbArg);
      //TODO faire les free
      write(sock,res,strlen(res));
      free(nomFonction);
      free(res);
      free(tabArg);
      free(nbArgTmp);
      break;
		
    }
    //write(sock , msg , strlen(msg));
    //perror("write");
    close(sock);
    printf("socket fermee\n");
  }
  
  printf("Processus termine\n");

  return 0;
}




void init_socaddr(struct sockaddr_un* addr){
  memset(addr,'0', sizeof(*addr));
  addr->sun_family = AF_UNIX;

  if(unlink(SOQUETTE)<0 && errno!=ENOENT){
    perror("unlink error dude");
    exit(-1);
  } 
  strncpy(addr->sun_path, SOQUETTE,sizeof(addr->sun_path)-1 );
}



int createServerSocket(){
  struct sockaddr_un addr;
  int sock;
  init_socaddr(&addr);
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



int main() {
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

