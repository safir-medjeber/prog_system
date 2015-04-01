#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "serialize.h"
#include "deserialize.h"
#include "serverFunctions.h"

int veriFonction(char* fonction,int nbArg,int sock){
	if(strcmp(fonction,"plus")==0){
		if(nbArg > 1 && nbArg < 5)
		return 1;
		else{
		  	erreur(MAUVAIS_ARGUMENTS,sock);
			return -1;
		}
	}
	else if(strcmp(fonction,"moins")==0){
		if(nbArg > 1 && nbArg < 5)
		return 2;
		else{
		  	erreur(MAUVAIS_ARGUMENTS,sock);
			return -1;
		}
	}
	else if(strcmp(fonction,"multiplie")==0){
		if(nbArg > 1 && nbArg < 5)
		return 3;
		else{
		  	erreur(MAUVAIS_ARGUMENTS,sock);
			return -1;
		}
	}
	if(strcmp(fonction,"divise")==0){
		if(nbArg > 1 && nbArg < 5)
		return 4;
		else{
		  	erreur(MAUVAIS_ARGUMENTS,sock);
			return -1;
		}
	}
	if(strcmp(fonction,"concat")==0){
		if(nbArg > 1 && nbArg < 5)
		return 5;
		else{
		  	erreur(MAUVAIS_ARGUMENTS,sock);
			return -1;
		}
	}

	if(strcmp(fonction,"boucle")==0){
		if(nbArg == 0)
		return 6;
		else{
		  	erreur(MAUVAIS_ARGUMENTS,sock);
			return -1;
		}
		return 6;
	}
	
	else{
	  	erreur(FONCTION_INCONNUE,sock);
		return -1;
	}
}

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
		case 6:
			return boucle();
			break;
		default:
			return NULL;	
		//TODO renvoyer erreur
			break;
	}
}
arg* getArg(char* buffer,int nbArg,int c,int sock){
	int i,j,taille;
	char* nbArgTmp;
	arg* tabArg=malloc(nbArg*sizeof(arg));
	for(i=0;i <nbArg;i++){
		if(buffer[++c]==1){//l'argument est un entier
			tabArg[i].type=1;
			taille=buffer[++c];
			nbArgTmp=malloc(taille*(sizeof(char))+1);
			for(j=0;j<taille;j++){
				nbArgTmp[j]=buffer[++c];
				printf("arg numero%d %c \n",i,nbArgTmp[j]);
			}
			nbArgTmp[j]='\0';
			printf("argument est %d et sa taille est %d  et son string %s\n",atoi(nbArgTmp),taille,nbArgTmp);
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
			char a= MAUVAIS_ARGUMENTS;
			write(sock,&a,1);
			close(sock);
			return NULL;
		}		
	}
	printf("fini le traitement ici\n");
	return tabArg;
}


void erreur(char err,int sock){
	switch(err){
		char a;
		case FONCTION_INCONNUE:
		printf("avant d'envoyer la sauce\n");
		a =FONCTION_INCONNUE;
		write(sock,&a,1);
		printf("sauce envoyee\n");
		close(sock);
		break;
		
		case MAUVAIS_ARGUMENTS:
		perror("mauvaise arguments");
		a=MAUVAIS_ARGUMENTS;
		write(sock,&a,1);
		close(sock);
		break;
		
		default:
		break;
	}
}
int getNbArg(char*buffer,int* c){
	(*c)++;
	if(buffer[*c]!=1){
		printf("lerreur est %d %d %d",buffer[*c],buffer[*c+1],buffer[*c+2]);
		return -1;
	}
	else{
		int res;
		char * tmp;
		tmp= deserialize(buffer,c,1);
		res= atoi(tmp);
		free(tmp);
		return res;
	}
}
char* deserialize(char* buffer,int* c,int type){
	char* nomFonction;
	int i,taille;
    if(type==2  && buffer[*c] !=2  ){// si on a pas transmis les infos sur la fonction a utiliser
		return NULL;
    }
	*c= *c+ 1;
	taille=buffer[*c];
	printf("la taille est %d \n",taille);
	nomFonction=malloc(taille+1*sizeof(char));//taille+1 car le dernier caractere pour stoquer le caractere '\0'
	for(i=0;i<taille;i++){// on mets les caracteres de la fonction a utiliser dans nomFonction
		*c=*c+1;
		nomFonction[i]=buffer[*c];
	}
	nomFonction[i]='\0';
	return nomFonction;
}



