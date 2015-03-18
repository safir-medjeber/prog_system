#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "serialize.h"
  
int taille_int (int n) {
  if (n < 0) return taille_int(n*-1);
  if (n < 10) return 1;
  if (n < 100) return 2;
  if (n < 1000) return 3;
  if (n < 10000) return 4;
  if (n < 100000) return 5;
  if (n < 1000000) return 6;
  if (n < 10000000) return 7;
  if (n < 100000000) return 8;
  if (n < 1000000000) return 9;
  /*      2147483647 is 2^31-1 - add more ifs as needed
	  and adjust this final return as well. */
  return 10;
}




void printVar(char ** ptr){
  int tmp;
  printf(" (%d ",**ptr);
  (*ptr)++;
  printf("%d)",**ptr);
  tmp = **ptr;
  for(int i=0; i< tmp; i++){
    (*ptr)++; 
    printf("'%c'",**ptr);
  }
  (*ptr)++;
}


void  printMsg(char * send){
  int tmp;
  char** ptr= &send;  
  printVar(ptr);
  printVar(ptr);
  printVar(ptr);
  printVar(ptr);
 
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
  memcpy(serial, buff2, lng+2);
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
  memcpy(serial, buff, lng+2);
  return serial;
}

char * serializeArg(arg argv){
  int i, lng1, lng2, lng, type;
  char *serialType, *convertString, *tmp, *serial;
  int *convertInt;
  char buff[512];
  
  serialType = serializeInt(argv.type);
  lng1 = strlen(serialType);
  type=serialType[2]-'0';// le type du pointeur de argv
  
  buff[0] = 0x00;
  
  if(type==1){// si pointeur sur int
    convertInt = (int *)argv.arg;
    lng2 = taille_int(*convertInt);
    tmp=serializeInt(*convertInt); 
 
    for(i=0; i<lng2+1; i++){
      buff[i+1]=tmp[1+i];
      }
  }

  else {// si pointeur sur char
    convertString = (char *)argv.arg;
    lng2 = strlen(convertString);
    tmp = serializeString(convertString);
 
    for(i=0; i<lng2+1; i++){
      buff[i+1]=tmp[1+i];
    }
  }

  lng = lng1+lng2+2;
  serial = malloc(sizeof(char)*(lng));
  memcpy(serial, serialType, lng1);
  
  for(i=0; i<(lng2+2); i++){
    serial[i+lng1]=buff[i];
  }
  
  return serial;
}

int testSerialize(){
  char* c = serializeInt(123);
  char* b = serializeString("ok");
  char* send= prepareMsgBeforeSend(b, c ,b);
  printMsg(send);
   
  return 0;
}



