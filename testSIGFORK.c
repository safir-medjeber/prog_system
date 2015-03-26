#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>


void timeOut ( int signo ) {
  printf("temps ecoulé 5 sec \n");
  //alarm (2) ; /* re−se t alarm */
  exit(0);
}

int main(){

  int pid = 1;


  pid = fork();


  if(pid==0){
    printf("coucou\n");
    if (signal( SIGALRM , timeOut ) == SIG_ERR )
      perror( " cant catch SIGALRM " ) ;
    alarm(2);
    sleep(10);
    exit(0);
  }
  wait(NULL);
  
  printf("attente du fil \n");
}
