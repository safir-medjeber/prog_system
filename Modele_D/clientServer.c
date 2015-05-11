#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#include "parsing.h"	
#include "serialize.h"
#include "printMsg.h"
#include "client.h"

#include "serveur.h"
#include "errno.h"
#include "printMsg.h"
#include "serialize.h"
#include "deserialize.h"
#include "serverFunctions.h"
#define BUFFSIZE 256



void runClientRequest(char* input){
  char *  token = strtok (input, " ");
  int nb_spaces = 1, i;
  int argc = 1;
  char ** res=NULL;
  res  = realloc (res, sizeof (char*) * nb_spaces);
  res[0] = "./client";
  
  
  while (token) {
    argc++;
    res = realloc (res, sizeof (char*) * ++nb_spaces);
    if (res == NULL)
      exit (-1); 
    res[nb_spaces-1] = token;
    token = strtok (NULL, " ");
  }

  res = realloc (res, sizeof (char*) * (nb_spaces+1));
  res[nb_spaces] = 0;

  /*
  for (i = 0; i < (nb_spaces+1); ++i)
    printf ("res[%d] = %so\n", i, res[i]);
  */
  runClientAfterParse(argc, res);
  free (res);
  
 
}


int main(int argc,char *argv[]) {
 
  char buff[BUFFSIZE];

  int  pid=fork();

  if(pid==0){
    exit(runServer());
  }
  else{
    while(1){
      int lu = read(STDIN_FILENO, buff, BUFFSIZE);
      buff[lu-1]='\0'; // supprime le  \n
      runClientRequest(buff);
    }
  }
 
 
  return 0;
}
