#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "serialize.h"


char* printVar(char* ptr){
  int tmp;
  printf(" (%d ",*ptr);
  ptr++;
  printf("%d)",*ptr);
  tmp = *ptr;
  for(int i=0; i< tmp; i++){
    ptr++; 
    printf("'%c'",*ptr);
  }
  ptr++;
  return ptr;
}

void  printMsg(char * send){
  char* ptr;  
  ptr = printVar(send);
  printf("\n");
  ptr = printVar(ptr);
  printf("\n");
  ptr = printVar(ptr);
  printf("\n");
  ptr = printVar(ptr);
  printf("\n");
  ptr = printVar(ptr);
  printf("\n");
} 


char * prepareMsgBeforeSend(char* fonction, char* argc, char* structArg){
  char *sizeMsg; 
  int size;
  char* send;
  size= strlen(fonction)+ strlen(argc) + strlen(structArg);
  sizeMsg = serializeInt(size);
  send = malloc(sizeof(char)*size+2);
  sprintf(send, "%s%s%s%s", sizeMsg, fonction, argc, structArg);
  return send;
}


char * serializeInt(int entier){
  int i, lng;
  char buff1[512];
  char buff2[512]; 
  char *serial;
  
  sprintf(buff1, "%d", entier); // Conversion de l'entier
  lng=strlen(buff1);
  serial=malloc(sizeof(char)*(lng+2));

  buff2[0]=0x01;
  buff2[1]=lng;
  
  for(i=0; i<lng; i++){
    buff2[i+2]=buff1[i];
  }
  buff2[i+3]='\0';
  memcpy(serial, buff2, lng+3);
  return serial;
}

char * serializeString(char *s){
  int i, lng;
  char *serial;
  char buff[512];
  lng=strlen(s);
  serial=malloc(sizeof(char)*(lng+2));
  buff[0]=0x02;
  buff[1]=lng;

  for(i=0; i<lng; i++){
    buff[i+2]=s[i];
  }
  buff[i+3]='\0';
  memcpy(serial, buff, lng+3);
  return serial;
}




char * serializeArg(arg argv){
  int type, size;
  char *champ1, *champ2, *structure;
  int convertInt;
  char buff[512];
  
  champ1 = serializeInt(argv.type);
  type= champ1[2]- '0';

  if(type==1){// si pointeur sur int
    convertInt = *((int *) argv.arg);
    champ2 = serializeInt(convertInt);
  }
  else{
    if(type==2){// si pointeur sur int
      champ2 = serializeString(((char *) argv.arg));
    }
  }

  size = strlen(champ1)+ strlen(champ2);
  structure = malloc(sizeof(char)*size);
  sprintf(structure, "%s%s", champ1, champ2);
  
  return structure;

}



int testSerialize(){
  arg a;
  int var =4356;
  char *var2= "coucou";
  a.type =1;
  a.arg=&var;

  char *ptr;
  char* fonction = serializeString("VoitureGamosGovaCaisse");
  char* argc= serializeInt(133444233);
  char* structArg =  serializeArg(a);
  char* send= prepareMsgBeforeSend(fonction, argc , structArg);
  printMsg(send);
  printf("\n");
   
  return 0;
}



