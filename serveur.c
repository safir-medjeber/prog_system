#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

 

#define ADRESSE "127.0.0.1"
#define PORT 61234


void init_socaddr(struct sockaddr_in* addr){
  bzero(addr, sizeof(*addr));
  addr->sin_family = AF_INET;
  addr->sin_port = htons(PORT);
  addr->sin_addr.s_addr = inet_addr(ADRESSE);
}



int main(int argc,char *argv[]) {
  struct sockaddr_in addr;
  struct sockaddr_in c;
  int sock, i, d, lus;
  unsigned int l;
  char buffer[256];

  init_socaddr(&addr);
  sock =  socket(PF_INET, SOCK_STREAM, 0);

  if (sock==-1) {
    fprintf(stderr,"sock problem\n");
    exit(EXIT_FAILURE);
  }

 
  if (bind(sock,(struct sockaddr *)&addr,sizeof(addr))==-1) {
    perror("bind problem ");
    close(sock);
    exit(EXIT_FAILURE);
  }
 
  if (listen(sock,0)==-1) {
    perror("listen problem ");
    close(sock);
    exit(EXIT_FAILURE);
  }

  
  do {
    if ((d=accept(sock,(struct sockaddr *)&c,&l))==-1) {
      perror("accept problem");
      close(sock);
      exit(EXIT_FAILURE);
    }
    
    shutdown(sock,SHUT_WR);
    while ((lus=read(d,buffer,256))>0) {
      for (i=0; i<lus; i++)
	printf("%c",buffer[i]);
      printf("\n");
    }
    close(d);
  }

  while(lus==0);
  close(sock);
}
