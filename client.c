#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define ADRESSE "127.0.0.1"
#define PORT 61234

struct arg{
  int variable1;
  int variable2;
};





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


//int appel_externe(const char *fonction, unsigned short argc, struct arg *argv);




void init_socaddr(struct sockaddr_in* addr){
  memset(addr,'0', sizeof(*addr));
  addr->sin_family = AF_INET;
  addr->sin_port = htons(PORT);
  addr->sin_addr.s_addr = inet_addr(ADRESSE);
}


int createSocket(){
  struct sockaddr_in addr;
  int sock;
  init_socaddr(&addr);
  sock = socket(PF_INET, SOCK_STREAM, 0);

  if(sock == -1){
    perror("Erreur create client");
    return -1;
  }
  
  if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
    perror("Client connect");
    close(sock);
    return -1;
  }
  return sock;
}


int runClient(){
  int socket;
  char buffer[256];
  socket = createSocket();
  
  if (socket==-1) {
    perror("socket problem\n");
    exit(EXIT_FAILURE);
  }
 
  char *n = "salut toi";
  printf("j'envoi le mot : %s\n", n);
  write(socket,n,strlen(n));
  perror("write ");
  read(socket,buffer,256);
  printf("%s", buffer);
  perror("read");
  close(socket);
  exit(EXIT_SUCCESS);

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


int testSerialize(){
  char * c = serializeInt(123);
  printf("int: %d%d %s\n",c[0],c[1],c);

  char * b = serializeString("argent money oseille maille pognon fric flouze pépète");
  printf("string: %d%d ", b[0],b[1]);
  int var = b[1];
  for (int i=0 ; i<var; i++)
    printf("%c", b[i+2]);
  printf("\n");

  return 0;
}

int main(int argc,char *argv[]) {

  //runClient();
  testSerialize();
  

}
