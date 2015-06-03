#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <ctype.h>
#include "serialize.h"
#include "client.h"

int isInt(char* testing){
  if(*testing=='-'){// le nombre est negatif
	  testing++;
  }
  while (*testing)
     {
        if (!isdigit(*testing))
           return 0;
        else
           ++testing;
     }
  return 1;
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

int runClientAfterParse(int argc, char *argv[]){
  int i=0 , j=0, k=0, size, nbrArg[512];
  char *name, *tmpArg; 
  
  if(argc==1){ //Si aucun nom de fonction
    printf("Pas de fonction a appeler\n");
    return 0;
  }
  else {  //Si on donne un nom de fonction
    size = numberArg(argc);
    arg a[size];        
    if(size==1){  //Si la fonction ne prend pas d'arguments   
      a[0].type = 0;
      a[0].arg = NULL;
      size = 0;
    }
    else {  //Si la fonction a des arguments
      for(i=2;i<argc;i++){
	tmpArg = argv[i];
	if(isInt(tmpArg)){ //Si l'argument est un nombre
	printf("largument %d est un entier\n",i-1);
	  nbrArg[k] = atoi(tmpArg);
	  if(nbrArg[k]< 0){
	  	printf("largument %d est un entier negatif %d \n",i-1,nbrArg[k] * (-1));
		  nbrArg[k]=nbrArg[k] * (-1);
		  a[j].type = 3;
	  }
	  else
	  	a[j].type = 1;
	  a[j].arg = &nbrArg[k];
	  k++;
	  j++;
	}
	else { //Si l'argument est un string
	printf("largument %d nest pas un entier\n",i-1);

	  a[j].type = 2;
	  a[j].arg = tmpArg;
	  j++;
	}
      }
    }
    name = nameFunc(argc, argv);
    runClient(name, size, a);
  } 
  return 0;
}
