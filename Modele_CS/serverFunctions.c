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
    if(arg[i].type!=1 && arg[i].type!=3){
      a = malloc(sizeof(char));
      printf("le type d'arg est:%d",arg[i].type);
      perror("plus");
      *a = MAUVAIS_ARGUMENTS;
      return a;
    }
    else{
      res+=*((int*)(arg[i].arg));
    }
  }
  char*b = serializeInt(res,res >=0? 1 : 3);

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
    if(arg[i].type!=1 && arg[i].type!=3){
      a = malloc(sizeof(char));
      *a = MAUVAIS_ARGUMENTS;
      return a;
    }
    else{
      if(i==0){
	res=*((int*)(arg[i].arg));
      }
      else
	res=res-*((int*)(arg[i].arg));
    }
  }
  char*b = serializeInt(res,res >=0? 1 : 3);

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
    if(arg[i].type!=1 && arg[i].type!=3){
      a = malloc(sizeof(char));
      *a = MAUVAIS_ARGUMENTS;
      return a;
    }
    else{
      res=res* *((int*)(arg[i].arg));
    }
  }
  char*b = serializeInt(res,res >=0? 1 : 3);

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
    if(arg[i].type!=1 && arg[i].type!=3){
      a = malloc(sizeof(char));
      *a = MAUVAIS_ARGUMENTS;
      return a;
    }
    else{
      if(i==0){
	res=*((int*)(arg[i].arg));
      }
      else
	res/= *((int*)(arg[i].arg));
    }
  }
  char*b = serializeInt(res,res >=0? 1 : 3);
  a = malloc((strlen(b)+1)*sizeof(char));// 
  a[0]=APPEL_OK;
  memcpy(a+1,b,strlen(b));
  printf("je vais afficher ce que vaut a\n");
  for(i=0;i < (int)strlen(b)+1;i++){
    printf("la valeur de a de %d : %d\n",i,a[i]);
  }
  return a;
}


char* concat(arg* arg,int nb){
  int i;
  char *res = malloc(sizeof(char)*128);
  char* a=NULL;


  for(i=0; i < nb;i++){
    if(arg[i].type!=2){
      a = malloc(sizeof(char));
      printf("Le type d'arg est: %d",arg[i].type);
      perror("concat");
      *a = MAUVAIS_ARGUMENTS;
      return a;
    }
    else{
      strcat(res, arg[i].arg) ;
    }
  }
  a = malloc((strlen(res)+1)*sizeof(char));
  char * w = serializeString(res);
  a[0]=APPEL_OK;  
  memcpy(a+sizeof(char),w,strlen(w));
  free(w);
  free(res);
  return a;
}

