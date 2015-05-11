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
int fonctionLocale(char* operateur){
	return 1;
}

int main(int argc,char** argv)
{
	char fonctions[20][5];// on stockera ici les fonctions que le programme saura faire
    int shmid;// identifiant du segment de mémoire partagé
    key_t key;// cle du segment partage
	pid_t pid;
    info *shm, *s;
    struct sockaddr_un addr;
    int sock,client_sock;
	if(argc< 3 ){// l'argument 1 correspond a la fonction que pourra efectuer ce serveur et le second au nom de la soquette pour le joindre
	// par ex ./a.out plus soquette fera que le programme saura faire la fonction plus et que la soquette pour le joindre se nomme soquette
		exit(EXIT_FAILURE);
	}


   key=5678;
    /*
     * Creation du segment mémoire
     */
    if ((shmid = shmget(key,sizeof(info)*2,flag)) < 0) {//on crée un nouveau segment mémoire
		if(errno==EEXIST){if((shmid = shmget(key, 0, 0)) < 0){// si le segment memoire existe déja on s"y connecte 
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
    * Notez que si le segment memoire existe deja et qu'on si connecte je n'ai pas trouve de moyen de savoir quel Nieme processus on est
	* par exemple il pourrait tres bien y avoir 3 client connecte ou un seul, c'est important de savoir cela car cela permet de connaitre sa place dans le 
	* tableau de structure info qu'est notre memoire partage
	* peut etre faut-il fait un broadcast comme en projet de PR6 chaque personne qui se connecte dit HLO a tout le monde et ces derniers repondent?
    */

    /*
     * on attache le segment a notre espace mémoire
     */
    if ((shm = (info*)shmat(shmid, NULL, 0)) == (info*)-1) {
        perror("shmat");
        exit(1);
    }

    /*
     * on mets les info que l'on souhaite sur les structures
     */
    s = shm;

    s->pid=getpid();
	strcpy(s->soquette,argv[1]);
	strcpy(s->fonctions[0],argv[2]);
	
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
			  write(client_sock,"rep",3);
	       	exit(EXIT_SUCCESS);
	      }

	    }
	}
	else{//pere qui va jouer le role du client
		char* requete;
		char buff[BUFFER_SIZE];
		pid_t p;
		int mySock;
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
					if(*token =='+' || *token=='-' || *token =='/' || *token=='*'){
						if(fonctionLocale(token)){// la fonction peut etre calcule par le serveur
							printf("je sais calculer ceci\n");
							pushStack(&s,calcul(token,popStack(&s),popStack(&s)));
						}
						else{// le serveur a besoin de demander le resultat a un autre serveur
							printf("je ne sais pas calculer ceci\n");
							// todo demander le calcul au bon serveur
						}
					}
					else{
						pushStack(&s,atoi(token));
						printf("je push %d\n",atoi(token));
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