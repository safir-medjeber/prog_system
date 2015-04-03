#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "serialize.h"



char* boucle(){
  while(1){
  }
  return NULL;
}

char* plus(arg* arg,int nb){
	int i;
	char* a;
	int res = 0;
	for(i=0; i < nb;i++){
		if(arg[i].type!=1){
			a = malloc(sizeof(char));
			*a = MAUVAIS_ARGUMENTS;
			return a;
		}
		else{
			res+=*((int*)(arg[i].arg));
		}
	}
	char*b = serializeInt(res);
	a = malloc((strlen(b)+1)*sizeof(char));// 
	a[0]=APPEL_OK;
	memcpy(a+1,b,strlen(b));
	printf("je vais afficher ce que vaut a\n");
	for(i=0;i < (int)strlen(b)+1;i++){
		printf("la valeur de a de %d : %d\n",i,a[i]);
	}
	return a;
}
char* moins(arg* arg,int nb){
	int i;
	int res = 0;
	char* a;
	for(i=0; i < nb;i++){
		if(arg[i].type!=1){
			a = malloc(sizeof(char));
			*a = MAUVAIS_ARGUMENTS;
			return a;
		}
		else{
			res-=*((int*)(arg[i].arg));
		}
	}
	char*b = serializeInt(res);
	a = malloc((strlen(b)+1)*sizeof(char));// 
	a[0]=APPEL_OK;
	memcpy(a+1,b,strlen(b));
	printf("je vais afficher ce que vaut a\n");
	for(i=0;i < (int)strlen(b)+1;i++){
		printf("la valeur de a de %d : %d\n",i,a[i]);
	}
	return a;
}
char* multiplie(arg* arg,int nb){
	int i;
	int res = 1;
	char* a;
	for(i=0; i < nb;i++){
		if(arg[i].type!=1){
			a = malloc(sizeof(char));
			*a = MAUVAIS_ARGUMENTS;
			return a;
		}
		else{
			res=res* *((int*)(arg[i].arg));
		}
	}
	char*b = serializeInt(res);
	a = malloc((strlen(b)+1)*sizeof(char));// 
	a[0]=APPEL_OK;
	memcpy(a+1,b,strlen(b));
	printf("je vais afficher ce que vaut a\n");
	for(i=0;i < (int)strlen(b)+1;i++){
		printf("la valeur de a de %d : %d\n",i,a[i]);
	}
	return a;
}
char* divise(arg* arg,int nb){
	//TODO verifier que le nombre d'ARGUMENTS et leurs type est bon
	int i;
	int res = 1;
	char* a;
	for(i=0; i < nb;i++){
		if(arg[i].type!=1){
			a = malloc(sizeof(char));
			*a = MAUVAIS_ARGUMENTS;
			return a;
		}
		else{
			res/=*((int*)(arg[i].arg));
		}
	}
	char*b = serializeInt(res);
	a = malloc((strlen(b)+1)*sizeof(char));// 
	a[0]=APPEL_OK;
	memcpy(a+1,b,strlen(b));
	printf("je vais afficher ce que vaut a\n");
	for(i=0;i < (int)strlen(b)+1;i++){
		printf("la valeur de a de %d : %d\n",i,a[i]);
	}
	return a;
}
char* concat(arg* argu,int nb){
  argu=NULL;
  nb=0;
	//TODO verifier que le nombre d'ARGUMENTS et leurs type est bon	
	return NULL;
}

