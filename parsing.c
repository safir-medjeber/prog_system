#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/socket.h>
#include "serialize.h"
#include "client.h"

int isInt(char* testing){
  int i, size, cond=1;
  size = strlen(testing);
  for(i=0; i<size; i++){
    if(testing[i]<='0' || testing[i]>='9'){
      cond = 0;
      return cond;
    }
  }
  return cond;
}

char* nameFunc(int argc, char* argv[]){
  if(argc==1)
    return NULL;
  else 
    return argv[1];
}

int numberArg(int argc){
  if(argc==2)
    return 1;
  else 
    return argc-2;
}

int parseArg(int argc, char *argv[]){
  int i=0 , j=0, k=0, size, nbrArg[512];
  char *name, *tmpArg; 
  
  if(argc==1){//Si aucun nom de fonction
    printf("Pas de fonction a appeler\n");
    return 0;
  }
  else {  //Si on donne un nom de fonction
    size = numberArg(argc);
    arg a[size];        
    if(size==1){  //Si la fonction ne prend pas d'arguments   
      a[0].type = 0;
      a[0].arg = NULL;
    }
    else {  //Si la fonction a des arguments
      for(i=2;i<argc;i++){
	tmpArg = argv[i];
	if(isInt(tmpArg)){ //Si l'argument est un nombre
	  nbrArg[k] = atoi(tmpArg);
	  a[j].type = 1;
	  a[j].arg = &nbrArg[k];
	  k++;
	  j++;
	}
	else { //Si l'argument est un string
	  a[j].type = 2;
	  a[j].arg = tmpArg;
	  j++;
	}
      }
      name = nameFunc(argc, argv);
      runClient(name, size, a);
    }
  } 
  return 0;
}
