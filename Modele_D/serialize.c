#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "serialize.h"



char * prepareMsgBeforeSend(char* fonction, char* argc, char* structArg){
  int size;
  char* send;
  size= strlen(fonction)+ strlen(argc) + strlen(structArg);
  send = malloc(sizeof(char)*size);
  sprintf(send, "%s%s%s",fonction, argc, structArg);
  return send;
}



char * serializeInt(int entier,int type){
  int i, lng;
  char buff1[512];
  char buff2[512]; 
  char *serial;
  
  sprintf(buff1, "%d", entier); // Conversion de l'entier
  lng=strlen(buff1);
  serial=malloc(sizeof(char)*(lng+3));
  memset(serial,0,lng+3);
  buff2[0]=type;
  buff2[1]=lng;
 
  for(i=0; i<lng; i++){
    buff2[i+2]=buff1[i];
  }
  memcpy(serial, buff2, lng+2);
  return serial;
}

char * serializeString(const char *s){
  int i, lng;
  char *serial;
  char buff[512];

  lng=strlen(s);
  serial=malloc(sizeof(char)*(lng+3));
  memset(serial,0,lng+3);
  buff[0]=0x02;
  buff[1]=lng;

  for(i=0; i<lng; i++){
    buff[i+2]=s[i];
  }
  memcpy(serial, buff, lng+2);
  return serial;
}


char * serializeArg(arg argv){
  int type, convertInt;
  char *champ="error";
  
  type= argv.type;
  if(type==0){// void
    champ = "";
  }
  if(type==1 || type==3){// si pointeur sur int
    convertInt = *((int *) argv.arg);
    champ = serializeInt(convertInt,type);
  }
  if(type==2){// si pointeur sur char
    champ = serializeString(((char *) argv.arg));
  }

  return champ;
}

char * serializeTabArg(unsigned short argc, arg* argv){
  int i, size=0;
  char *tmpArg, *tabStruct;
  for(i=0; i<argc; i++){
    tmpArg = serializeArg(*argv);
    size += strlen(tmpArg);
  }
  tabStruct = malloc(sizeof(char)*size); 
	
  for(i=0; i<argc; i++){
    tmpArg = serializeArg(argv[i]);
    strcat(tabStruct,tmpArg);
  }
 
  return tabStruct;
}