#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "serialize.h"
#include "deserialize.h"
#include "serverFunctions.h"

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
	for(int i=0;i <nbArg;i++){
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
			write(sock,(void*)(MAUVAIS_ARGUMENTS),1);
			close(sock);
			return NULL;
		}		
	}
	return tabArg;
}


void erreur(char err,int sock){
	switch(err){
		case FONCTION_INCONNUE:
		perror("mauvaise fonction");
		write(sock,(void*)FONCTION_INCONNUE,1);
		close(sock);
		break;
		
		case MAUVAIS_ARGUMENTS:
		perror("mauvaise arguments");
		write(sock,(void*)MAUVAIS_ARGUMENTS,1);
		close(sock);
		break;
		
		default:
		break;
	}
}

char* getNomFonction(char* buffer,int taille,int c){
	char* nomFonction;
	int i;
	nomFonction=malloc(taille+1*sizeof(char));//taille+1 car le dernier caractere pour stoquer le caractere '\0'
	for(i=0;i<taille;i++){// on mets les caracteres de la fonction a utiliser dans nomFonction
		c++;
		nomFonction[i]=buffer[c];
	}
	nomFonction[i]='\0';
	return nomFonction;
}



