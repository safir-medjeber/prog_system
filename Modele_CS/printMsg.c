#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int extractInt(char * s){
  int i, size, value;
  char * tmp;
  size = s[1];
  tmp = malloc(sizeof(char)*size);
  for(i=0; i< size; i++)
    tmp[i]=s[i+2];
  value=atoi(tmp);
  free(tmp);
  return value; 
}


char* printVar(char* ptr){
  int tmp, i;
  printf(" (%d ",*ptr);
  ptr++;
  printf("%d)",*ptr);
  tmp = *ptr;
  for(i=0; i< tmp; i++){
    ptr++; 
    printf("'%c'",*ptr);
  }
  ptr++;
  return ptr;
}


void  printMsg(char * send){
  char* ptr;
  int i, argc;
  ptr = printVar(send);
  printf("\n");
  argc = extractInt(ptr);
  ptr = printVar(ptr);
  printf("\n");
  for(i=0; i< argc ; i++){
    ptr = printVar(ptr);
    printf("\n");
  } 
}


void  printReceiveMsg(char * ptr){
  int tmp, i;
  ptr++;
  tmp = *ptr;
  printf("Resultat : ");
  for(i=0; i< tmp; i++){
    ptr++; 
    printf("%c",*ptr);
  }
  printf("\n");
}


