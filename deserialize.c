#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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



