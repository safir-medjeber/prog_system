#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#include "printMsg.h"
#include "serialize.h"
#define ADRESSE "127.0.0.1"
#define PORT 61234


int* setArg(int a){
	int *x = malloc(sizeof(int));
	*x=a;
	//printf("la valeur du premer arg est: %d",*x);
	return x;
}
char* setArg2(char* a){
	char* tmp = malloc(strlen(a)*sizeof(char));
	memcpy(tmp,a,strlen(a));
	//printf("la valeur du second arg est: %s",tmp);
	return tmp;
}
int veriFonction(char* fonction){
	if(strcmp(fonction,"plus")==0){
		return 1;
	}
	else{
		return -1;
	}
}
// Gestionnaire de connection pour un client
void *connection_handler(void *socket_desc){
  int lus, i,nbArg,taille,c,j;
  int sock = *(int*)socket_desc;
  char buffer[1024];
  char* nomFonction;
  char* nbArgTmp;
  char *msg = "votre msg a ete recu par le serveur\n";
  while ((lus=read(sock,buffer,256))>0) {
    printMsg(buffer);
    if(buffer[0] !=2){// si on a pas transmis les infos sur la fonction a utiliser
    	perror("erreur le nom de la function n'est pas transmis");
    }
	else{//ok on a donne le nom de la fonction a utiliser
		c=0;
		c++;
		taille=buffer[c];
		nomFonction=malloc(taille+1*sizeof(char));//taille+1 car le dernier caractere pour stoquer le caractere '\0'
		for(i=0;i<taille;i++){// on mets les caracteres de la fonction a utiliser dans nomFonction
			c++;
			nomFonction[i]=buffer[c];
		}
		nomFonction[i]='\0';
		printf("la fonction est:%s \n",nomFonction);
		veriFonction(nomFonction);
		c++;
		// on deserialise le nombre d'argument
		if(buffer[c]!=1){
			perror("nombre d'arguments non fourni");
		}
		taille=buffer[++c];
		nbArgTmp=malloc(taille*sizeof(char)+1);
		for(i=0;i< taille;i++){
			c++;
			nbArgTmp[i]=buffer[c];
		}
		nbArg=atoi(nbArgTmp);
		free(nbArgTmp);
		printf("jai %d arguments\n",nbArg);
		//TODO ici mettre la condition sur le nombre d'arguements que l'on veut
		arg tabArg[nbArg];// tableau ou l'on va stoquer tous les arguments sous forme de struct arg
		for(int i=0;i <nbArg;i++){
			if(buffer[++c]==1){//l'argument est un entier
				tabArg[i].type=1;
				taille=buffer[++c];
				nbArgTmp=malloc(taille*(sizeof(char)));
				for(j=0;j<taille;j++){
					nbArgTmp[j]=buffer[++c];
				}
				tabArg[i].arg=setArg(atoi(nbArgTmp));
				free(nbArgTmp);
			}
			else if (buffer[c]==2){//l'argument est un char*
				tabArg[i].type=2;
				taille=buffer[++c];
				nbArgTmp=malloc((taille+1)*(sizeof(char)));
				for(j=0;j<taille;j++){
					nbArgTmp[j]=buffer[++c];
				}
				nbArgTmp[j]='\0';
				tabArg[i].arg=setArg2(nbArgTmp);
				free(nbArgTmp);
			}
			else{
				perror("type d'argument innatendu");
				printf("\n%d\n",buffer[c]);
				break;
			}		
		}
		printf(" 1er arg:%s \n 2eme arg:%d \n",tabArg[0].arg,*((int*)(tabArg[1].arg)));
	}
    write(sock , msg , strlen(msg));
    //perror("write");
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

