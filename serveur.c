#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
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
char* plus(arg* arg,int nb){
	//TODO verifier que le nombre d'argument et leurs type sont bons
	int i;
	int res = 0;
	for(i=0; i < nb;i++){
		if(arg[i].type!=1){
			//TODO renvoyer mauvais argument
		}
		else{
			res+=*((int*)(arg[i].arg));
		}
	}
	char*b = serializeInt(res);
	char* a = malloc((strlen(b)+1)*sizeof(char));// 
	a[0]=APPEL_OK;
	memcpy(a+1,b,strlen(b));
	printf("je vais afficher ce que vaut a\n");
	for(i=0;i < strlen(b)+1;i++){
		printf("la valeur de a de %d : %d\n",i,a[i]);
	}
	return a;
}
char* moins(arg* arg,int nb){
	//TODO verifier que le nombre d'argument et leurs type est bon
	int i;
	int res = 0;
	for(i=0; i < nb;i++){
		if(arg[i].type!=1){
			//TODO renvoyer mauvais argument
			return NULL;
		}
		else{
			res-=*((int*)(arg[i].arg));
		}
	}
	char*b = serializeInt(res);
	char* a = malloc((strlen(b)+1)*sizeof(char));// 
	a[0]=APPEL_OK;
	memcpy(a+1,b,strlen(b));
	printf("je vais afficher ce que vaut a\n");
	for(i=0;i < strlen(b)+1;i++){
		printf("la valeur de a de %d : %d\n",i,a[i]);
	}
	return a;
}
char* multiplie(arg* arg,int nb){
	//TODO verifier que le nombre d'argument et leurs type est bon
	int i;
	int res = 1;
	for(i=0; i < nb;i++){
		if(arg[i].type!=1){
			//TODO renvoyer mauvais argument
			return NULL;
		}
		else{
			res=res* *((int*)(arg[i].arg));
		}
	}
	char*b = serializeInt(res);
	char* a = malloc((strlen(b)+1)*sizeof(char));// 
	a[0]=APPEL_OK;
	memcpy(a+1,b,strlen(b));
	printf("je vais afficher ce que vaut a\n");
	for(i=0;i < strlen(b)+1;i++){
		printf("la valeur de a de %d : %d\n",i,a[i]);
	}
	return a;
}
char* divise(arg* arg,int nb){
	//TODO verifier que le nombre d'argument et leurs type est bon
	int i;
	int res = 1;
	for(i=0; i < nb;i++){
		if(arg[i].type!=1){
			//TODO renvoyer mauvais argument
			return NULL;
		}
		else{
			res/=*((int*)(arg[i].arg));
		}
	}
	char*b = serializeInt(res);
	char* a = malloc((strlen(b)+1)*sizeof(char));// 
	a[0]=APPEL_OK;
	memcpy(a+1,b,strlen(b));
	printf("je vais afficher ce que vaut a\n");
	for(i=0;i < strlen(b)+1;i++){
		printf("la valeur de a de %d : %d\n",i,a[i]);
	}
	return a;
}
char* concat(arg* argu,int nb){
	//TODO verifier que le nombre d'argument et leurs type est bon	
	return NULL;
}
int veriFonction(char* fonction){
	if(strcmp(fonction,"plus")==0){
		return 1;
	}
	else if(strcmp(fonction,"moins")==0){
		return 2;
	}
	else if(strcmp(fonction,"multiplie")==0){
		return 3;
	}
	if(strcmp(fonction,"divise")==0){
		return 4;
	}
	if(strcmp(fonction,"concat")==0){
		return 5;
	}
	else{
		return -1;
	}
}
char* apply_function(int fonc,arg* argu,int nbArg){
	switch(fonc){
		case 1:
			return plus(argu,nbArg);
			break;
		case 2:
			return moins(argu,nbArg);
			break;
		case 3:
			return multiplie(argu,nbArg);
			break;
		case 4:
			return divise(argu,nbArg);
			break;
		case 5:
			return concat(argu,nbArg);
			break;
		default:
			return NULL;	
		//TODO renvoyer erreur
			break;
	}
}
// Gestionnaire de connection pour un client
void *connection_handler(void *socket_desc){
  int lus, i,nbArg,taille,c,j,fonction;
  int sock = *(int*)socket_desc;
  char buffer[1024];// la ou on va stoquer les messages transmis par le socket
  char* nomFonction;// la ou on va stoquer le nom de la fonction demander
  char* nbArgTmp;
  char* res; // la ou on va stoquer le resultat a retourner au client
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
		fonction=veriFonction(nomFonction);// fonction represente un numero associe a la fonction que l'on va utiliser dans un switch pour faire
		// le bon appel de fonction plus bas
		printf("la fonction est:%s \n",nomFonction);
		if( fonction< 0){
			perror("mauvaise fonction");
			write(sock,serializeInt(FONCTION_INCONNUE),3);
			return NULL;
		}
		c++;
		// on deserialise le nombre d'argument
		if(buffer[c]!=1){
			perror("nombre d'arguments non fourni");
			write(sock,serializeInt(MAUVAIS_ARGUMENTS),3);
			return NULL;
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
				write(sock,serializeInt(MAUVAIS_ARGUMENTS),3);
				return NULL;
			}		
		}
		// tout va bien la serialisation s'est bien deroulee on appelle la fonction demande
		res=apply_function(fonction,tabArg,nbArg);
		//TODO faire les free
		printf("voici la reponse\n");
		printMsg(res);
		write(sock,res,strlen(res));
		free(res);
		printf("\n 1er arg:%d \n 2eme arg:%d \n",*((int*)(tabArg[0].arg)),*((int*)(tabArg[1].arg)));
		break;
		
	}
    write(sock , msg , strlen(msg));
    //perror("write");
    close(sock);
    printf("socket fermee\n");
  }
  
  printf("thread terminee\n");

  return NULL;
}




void init_socaddr(struct sockaddr_un* addr){
  memset(addr,'0', sizeof(*addr));
  addr->sun_family = AF_UNIX;
  
  //  								  /_!_\	REMARQUE IMPORTANTE 	/_!_\
  
  // Il faudrait peut etre changer le nom de la soquette pour chaque connextion car chaque connexion est unique 
  // NB : penser a tester plusieurs connexions pour verifier que le nom unique de la socket ne pose paas probleme
  strncpy(addr->sun_path, getenv("HOME"), sizeof(addr->sun_path)-1);
  strncat(addr->sun_path, "/", sizeof(addr->sun_path)-1);
  strncat(addr->sun_path, ".soquette", sizeof(addr->sun_path)-1);
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



int main(int argc,char *argv[]) {
  struct sockaddr_un addr;
  struct sockaddr_un c;
  int sock,client_sock;
  unsigned int l;
  pthread_t th;

  init_socaddr(&addr);
  sock = createServerSocket(); 

   if (sock==-1) {
    perror("socket problem\n");
    exit(EXIT_FAILURE);
  }
 
  while ((client_sock=accept(sock,NULL,NULL))) {
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

