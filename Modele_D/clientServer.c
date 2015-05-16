#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "serveur.h"
#include "parsing.h"	
#include "serialize.h"
#include "printMsg.h"
#include "client.h"
#include "errno.h"
#include "printMsg.h"
#include "serialize.h"
#include "deserialize.h"
#include "serverFunctions.h"
#include "Polonaise.h"
#include "stack.h"


#define flag IPC_CREAT | S_IRUSR | S_IWUSR| IPC_EXCL 
#define BUFFER_SIZE 256
#define NBCLIENTSERV 4
#define BUFFSIZE 256

typedef struct info{
	pid_t pid;
	char fonctions [20][5];
	char soquette[20];
}info;

int* runClientRequest(char* input,char* soquette){
  char *  token = strtok (input, " ");
  int nb_spaces = 1;
  int argc = 1;
  char ** res=NULL;
  res  = realloc (res, sizeof (char*) * nb_spaces);
  res[0] = "./client";
  
  
  while (token) {
    argc++;
    res = realloc (res, sizeof (char*) * ++nb_spaces);
    if (res == NULL)
      exit (-1); 
    res[nb_spaces-1] = token;
    token = strtok (NULL, " ");
  }

  res = realloc (res, sizeof (char*) * (nb_spaces+1));
  res[nb_spaces] = 0;

  /*
  for (i = 0; i < (nb_spaces+1); ++i)
    printf ("res[%d] = %so\n", i, res[i]);
  */
  int* resultat =runClientAfterParse(argc, res,
  soquette);
  free (res);
  return resultat;
 
}

//Fonction pour calculer une operation binaire on utilisera cette fonction quand on demande a notre client de faire un calcul qu'il sait faire
int calcul(char* op,int a, int b){
	if(*op=='+'){
		return a+b;
	}
	else if(*op=='-')
		return b-a;
	else if(*op=='*')
		return a*b;
	else return b/a;
}

// cette fonction sert a verifier si le calcule qu'on demande a faire a notre client est realisable par le client en question
// cette fonction n'est pas implementée pour le moment et renvoie toujours vrai
char* operateurToString(char op){
	switch(op){
		case '+' :
		return "plus";
		break;
		
		case '-' :
		return "moins";
		break;
		
		case '*' :
		return "multiplie";
		break;
		
		case '/' :
		return "divise";
		break;
		
		default:
		return NULL;	
	}
}
int fonctionLocale(char operateur,int pos,info* p){
	p+=pos;
	int i;
	char* comparaison= operateurToString(operateur);
	if (comparaison==NULL)
		return -1;
	for(i=0;i<5;i++ ){
		if((strcmp(p->fonctions[i],comparaison)==0))
			return 1;
	}
	return 0;
}

int chercheServeur(char operateur,info* p){
	int i,j;
	char* comparaison= operateurToString(operateur);
	if (comparaison==NULL)
		return -1;
	for( i =0;i < NBCLIENTSERV;i++ ){
		for( j=0; j < 5; j++){
			if((strcmp(p->fonctions[j],comparaison)==0))
				return i;
		}
		p++;
	}
	return -1;
}
char* toStringRequete(char op,int a,int b){
	char* nomFonction = operateurToString(op);
	char buf[256];
	char* total = (char*)malloc(512);
	sprintf(buf," %d %d",b,a);
	strcpy(total,nomFonction);
	strcat(total,buf);
//	printf("LOOOK %s\n",total);
	return total;
}



// /!\/!\/!\/!\/!\ attention si on deplace cette foction hors de ce fichier alors copier aussi le #define flag :)
void CreateOrConnectSegment(int *shmid,int key,int* premier){
    if ((*shmid = shmget(key,sizeof(info)*NBCLIENTSERV,flag)) < 0) {//on crée un nouveau segment mémoire
		if(errno==EEXIST){
			if(premier!=NULL)
				*premier=0;// car on n'est pas celui qui a crée le segment mémoire
			if((*shmid = shmget(key, 0, 0)) < 0){// si le segment memoire existe déja on s"y connecte 
				perror("shmget exist");
				        exit(1);
			}
		}
		else{// erreur shmget a echoue
			printf("errno is %i",errno);
			perror("shmget");
			        exit(1);
		}
    }
	else if(premier != NULL){// dans ce cas on est celui qui a cree le segment memoire donc on met premier a 1 pour le savoir
		*premier =1;
	}
}

void putInfo(info* s,int maPosition,char** argv,int argc){
	int i;
	s+=maPosition;
    s->pid=getpid();
	strcpy(s->soquette,argv[1]);
	for(i=2; i < argc;i++){
		strcpy(s->fonctions[i-2],argv[i]);
	}
}

int launchServeur(int sock){
	pid_t pid;
	int client_sock;
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
	return 1;
}

int launchClient(int maPosition,int* position,info* shm){
	char* requete;
	char buff[BUFFER_SIZE];
	pid_t p;
	int* resultatServeurDistant;
	int serveurAppele;
    const char e[2] = " ";		
	printf("voici mes infos je suis le prog N° %d mon pid est %d et ma fonc est %s\n",maPosition,(shm+(*position))->pid,(shm+(*position))->fonctions[0]);
	while(1){
		printf("entrer la calcul que vous souhaitez effectuer\n");
		requete=fgets(buff,BUFFER_SIZE-1,stdin);
		if(requete==NULL)
			continue;
		if((p=fork()) < 0){
			exit(EXIT_FAILURE);
		}
		else if(p==0){// le fils va maintenant traiter le calcul demandé
		    char *token ;
			char* saveptr;
			STACK s;// pile d'entier, indispensable pour calculer une expression en forme polonaise
			s.top=-1;
			char* polonaise = toPolonaise(requete);
			token=strtok_r(polonaise,e,&saveptr);
			while(token !=NULL){// on parse l'expression en forme polonaise cela nous donnera a chaque iteration un nombre ou un operateur
				if((isdigit(*token)) || ( *token=='-' && isdigit(*(token+1)))){// on a tire un nombre
					pushStack(&s,atoi(token));
					//printf("je push %d\n",atoi(token));
				}
				else{// on a tire un operateur
					if(fonctionLocale(token[0],maPosition,shm)){// la fonction peut etre calcule par le serveur
						//printf("je sais calculer ceci\n");
						pushStack(&s,calcul(token,popStack(&s),popStack(&s)));
					}
					else{// le serveur a besoin de demander le resultat a un autre serveur
						//printf("je ne sais pas calculer ceci\n");
						if((serveurAppele=chercheServeur(token[0],shm)) < 0){
							printf("ni moi ni mes amis client/serveur ne savont faire cette operation compliquée");
							clearStack(&s);
							break;
						}
						printf("je ne sais pas faire ca je vais demander au servuer N %d a la soquette %s\n",serveurAppele,(shm+serveurAppele)->soquette);
						char* request = toStringRequete(token[0],popStack(&s),popStack(&s));
				        resultatServeurDistant=runClientRequest(request,(shm+serveurAppele)->soquette);
						if(resultatServeurDistant==NULL){// le serveur distant n'a pu executer la requete
							printf("echec serseur distant");
							break;
						}
						else{// le servuer distant a executé la requete avec succes
							pushStack(&s,*resultatServeurDistant);
						}
						free(request);
					}
				}
				token=strtok_r(NULL,e,&saveptr);
			}
			printf("resultat final %d et polonaise %s\n",popStack(&s),polonaise);	
			clearStack(&s);		
		}
	}
}


int main(int argc,char** argv)
{
    int shmid;// identifiant du segment de mémoire partagé
	int shmid2;//identifiant du segment mémoire de l'entier qui stockera notre position dans le tableau de structure
    key_t key,key2;// cle du segment partage
	pid_t pid;
    info *shm;// deux pointeurs qui pointerons sur le debut du segment memoire partagée stoquant le tableau de struct info
	int * position;// pointeur sur le segment de mémoire partagé,contient un entier qui indique quelle est la position dans la tableau du prochain client serveur se connectant
	int premier; // est a 0 si l'on programme que l'on execute est celui qui a crée le segment mémoire, est sctrictement positif sinon
	int maPosition;// position du serveur dans le tableau de structure
    struct sockaddr_un addr;
    int sock;
    
	
	/* l'argument premier correspond au nom de la soquette pour le joindre et les restants aux la fonctions que pourra efectuer ce serveur 
	 par ex ./a.out soquette plus fera que le programme saura faire la fonction plus et que la soquette pour le joindre se nomme soquette
	*/
	
	if(argc< 3 || argc > 7 ){
		exit(EXIT_FAILURE);
		}	
   /*
    * Initialisation des cles
    */
    key=5678;
    key2=8765;
  
    /*
     * Creation du segment mémoire pour les fonctions partagées
	*/
	
	CreateOrConnectSegment(&shmid,key,&premier);
	
   /*
    * Creation du segment mémoire pour la position du serveur dans le tableau
    */
	
	CreateOrConnectSegment(&shmid2,key2,NULL);

    /*
     * on attache le segment a notre espace mémoire
     */
    if ((shm = (info*)shmat(shmid, NULL, 0)) == (info*)-1) {
        perror("shmat");
        exit(1);
    }
	
    if ((position = (int*)shmat(shmid2, NULL, 0)) == (int*)-1) {
        perror("shmat");
        exit(1);
    }
 	if(premier !=0){
 		*position=0;
		maPosition=0;
 	}
	else{
	//	printf("old position est %d\n",*position);
		*position=*position+1;
		maPosition=*position;
	}
   
	putInfo(shm,maPosition, argv, argc);
	
	/*  ca commence a forker ici on enclenche le client/serveur
	*
	*/
	if((pid=fork()) < 0){
		exit(EXIT_FAILURE);
	}
	if (pid==0){//fils qui va jouer le role du serveur
	    init_socaddr(&addr,argv[1]);//argv[1] est le nom que l'on va donner a notre serveurSocket
	    sock = createServerSocket(argv[1]); 
		launchServeur(sock);
	}
	else{//pere qui va jouer le role du client
		launchClient( maPosition,position,shm);
				
			}
		}
