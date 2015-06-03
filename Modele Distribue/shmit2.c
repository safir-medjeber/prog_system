#include <sys/types.h>
#include <sys/ipc.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include "Polonaise.h"
#include "stack.h"
#define flag IPC_CREAT | S_IRUSR | S_IWUSR| IPC_EXCL 
#define BUFFER_SIZE 256
#define NBCLIENTSERV 4
//structure que l'on va utiliser pour stoquer les infomations d'un client serveur en particulier le pid la socket et les fonctions
// on se limite a 5 fonctions limités a 20 caractere(vous pouvez autorisez plus de 20 carac si vous voulez) car c'est plus facile de gerer la mémoire partagé quand on connait sa taille maximale
typedef struct info{
	pid_t pid;
	char fonctions [20][5];
	char soquette[20];
}info;

// /!\/!\/!\/!\/!\/!\/!\ CE QUI VA SUIVRE EST DU COPIER COLLER DE FONCTIONS DU MODELE NORMAL JE LES ENLEVERAI
// ***** ET JE METTRAI LES INCLUDES NECESSAIRES PLUS TARD POUR LINSTANT JE LES LAISSE ICI POUR TESTER PLUS VITE
// ***** SAUTEZ DIRECTEMENT A LA LIGNE 83

void init_socaddr2(struct sockaddr_un* addr,char* sock){
  memset(addr,0, sizeof(*addr));
  addr->sun_family = AF_UNIX;
  strncpy(addr->sun_path, sock, sizeof(addr->sun_path)-1);
}


int createSocket(char* sock2){
  struct sockaddr_un addr;
  int sock;
  sock = socket(AF_UNIX, SOCK_STREAM, 0);
  init_socaddr2(&addr,sock2);
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
void init_socaddr(struct sockaddr_un* addr,char* sock){
  memset(addr,'0', sizeof(*addr));
  addr->sun_family = AF_UNIX;

  if(unlink(sock)<0 && errno!=ENOENT){
    perror("unlink error dude");
    exit(-1);
  } 
  strncpy(addr->sun_path, sock,sizeof(addr->sun_path)-1 );
}
int createServerSocket(char* a){
  struct sockaddr_un addr;
  int sock;
  init_socaddr(&addr,a);
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
		return "multiplier";
		break;
		
		case '/' :
		return "diviser";
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
		for(int j=0; j < 5; j++){
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
	sprintf(buf,"%d %d",a,b);
	strcpy(total,nomFonction);
	strcat(total,buf);
	return total;
}

int* runClientRequest(char* requete,char* soquette){
	
}

int main(int argc,char** argv)
{
	char fonctions[20][5];// on stockera ici les fonctions que le programme saura faire
    int shmid;// identifiant du segment de mémoire partagé
	int shmid2;//identifiant du segment mémoire de l'entier qui stockera notre position dans le tableau de structure
    key_t key,key2;// cle du segment partage
	pid_t pid;
    info *shm, *s;
	int * position;
	int i;//pour les boucles for
	int premier; // est a 0 si l'on programme que l'on execute est celui qui a crée le segment mémoire, est sctrictement positif sinon
	int maPosition;// position du serveur dans le tableau de structure
    struct sockaddr_un addr;
    int sock,client_sock;
	if(argc< 3 || argc > 7 ){// l'argument premier correspond au nom de la soquette pour le joindre et les restants aux la fonctions que pourra efectuer ce serveur 
	// par ex ./a.out soquette plus fera que le programme saura faire la fonction plus et que la soquette pour le joindre se nomme soquette
		exit(EXIT_FAILURE);
	}


   key=5678;
   key2=8765;
    /*
     * Creation du segment mémoire pour les fonctions partagées
     */
    if ((shmid = shmget(key,sizeof(info)*NBCLIENTSERV,flag)) < 0) {//on crée un nouveau segment mémoire
		if(errno==EEXIST){
			premier=0;// car on n'est pas celui qui a crée le segment mémoire
			if((shmid = shmget(key, 0, 0)) < 0){// si le segment memoire existe déja on s"y connecte 
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
	else{// dans ce cas on est celui qui a cree le segment memoire donc on met premier a 1 pour le savoir
		premier =1;
	}
   /*
    * Creation du segment mémoire pour la position du serveur dans le tableau
    */
    if ((shmid2 = shmget(key2,sizeof(int),flag)) < 0) {//on crée un nouveau segment mémoire
		if(errno==EEXIST){if((shmid2 = shmget(key2, 0, 0)) < 0){// si le segment memoire existe déja on s"y connecte 
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
		printf("old position est %d\n",*position);
		*position=*position+1;
		maPosition=*position;
	}
   

    /*
     * on mets les info (nom de socket,pid,fonctions) sur les structures
     */
    s = shm;
	s+=maPosition;
    s->pid=getpid();
	strcpy(s->soquette,argv[1]);
	for(i=2; i < argc;i++){
		strcpy(s->fonctions[i-2],argv[i]);
	}
	
	/*  ca commence a forker ici on enclenche le client/serveur
	*
	*/
	if((pid=fork()) < 0){
		exit(EXIT_FAILURE);
	}
	if (pid==0){//fils qui va jouer le role du serveur
	    init_socaddr(&addr,argv[1]);//argv[1] est le nom que l'on va donner a notre serveurSocket
	    sock = createServerSocket(argv[1]); 
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
			  //ici gerer la requete du client pas encore implemente pour le moment
			 // write(client_sock,"rep",3);
	       	exit(EXIT_SUCCESS);
	      }

	    }
	}
	else{//pere qui va jouer le role du client
		char* requete;
		char buff[BUFFER_SIZE];
		pid_t p;
		int* resultatServeurDistant;
		int mySock,serveurAppele;
		printf("voici mes infos je suis le prog N° %d mon pid est %d et ma fonc est %s\n",maPosition,(shm+(*position))->pid,(shm+(*position))->fonctions[0]);
		while(1){
			printf("je suis le papa j'attends les requetes\n");
			requete=fgets(buff,BUFFER_SIZE-1,stdin);// calcul demander par le client tout doit etre separe par un espace par ex ( 20 + 30 ) / ( 3 + 2 ) * 4
			// sinon la transformation en forme polonaise echouera
			if(requete==NULL)
				continue;
			if((p=fork()) < 0){
				exit(EXIT_FAILURE);
			}
			else if(p==0){// le fils va maintenant traiter le calcul demandé
			    const char e[2] = " ";		
			    char *token ;
				STACK s;// pile d'entier, indispensable pour calculer une expression en forme polonaise
				s.top=-1;
				char* polonaise = toPolonaise(requete);
				printf("transformation reussi %s debut du calcul\n",polonaise);
				token=strtok(polonaise,e);
				while(token !=NULL){// on parse l'expression en forme polonaise cela nous donnera a chaque iteration un nombre ou un operateur
					
					if((isdigit(*token)) || ( *token=='-' && isdigit(*(token+1)))){// on a tire un nombre
						pushStack(&s,atoi(token));
						printf("je push %d\n",atoi(token));
					}
					else{// on a tire un operateur
						if(fonctionLocale(token[0],maPosition,shm)){// la fonction peut etre calcule par le serveur
							printf("je sais calculer ceci\n");
							pushStack(&s,calcul(token,popStack(&s),popStack(&s)));
						}
						else{// le serveur a besoin de demander le resultat a un autre serveur
							printf("je ne sais pas calculer ceci\n");
							if((serveurAppele=chercheServeur(token[0],shm)) < 0){
								printf("ni moi ni mes amis client/serveur ne savont faire cette operation compliquée");
								clearStack(&s);
								break;
							}
							//pushStack(&s,calculServeurDistant(serveurAppele,token[0],popStack(&s),popStack(&s)));
							printf("je ne sais pas faire ca je vais demander au servuer N %d",serveurAppele);
							char* requete = toStringRequete(token[0],popStack(&s),popStack(&s));
					        resultatServeurDistant=runClientRequest(requete,(shm+serveurAppele)->soquette);
							if(resultatServeurDistant==NULL){// le serveur distant n'a pu executer la requete
								printf("echec serseur distant");
								break;
							}
							else{// le servuer distant a executé la requete avec succes
								
							}
							free requete;
							//clearStack(&s);
						}
										}
					token=strtok(NULL,e);
				}
				// 
				//mySock=createSocket();
				//write(mySock,&resultat,sizeof(int));
				//read(mySock,buff,BUFFER_SIZE);
				printf("%d\n",popStack(&s));
			}
		}
		
	}
   
}