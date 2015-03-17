#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



struct arg{
  int type;
  void *arg;
};
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
char * serializeInt(int entier){
	printf("nouvel appel\n");
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
	printf("regarde %c\n",buff1[i]);
  }
  memcpy(serial, buff2, lng+2);
  return serial;
}

char * serializeArg(struct arg argv){
  int i, lng1, lng2, lng,type;
  char *serialType,  *tmp, *serial,*nb;
  int * convert;
  char buff[512];
  serialType = serializeInt(argv.type);
  type=serialType[2]-'0';// permet de savoir quel est le type du pointeur de argv
  printf("type vaut %d\n",type);
  if(type==1){// si le pointeur de argv est un pointeur sur int
	  lng1 = strlen(serialType);
	  convert = (int *)argv.arg;
	  printf("\n i am \t %d\n",*convert);
	   lng2 = taille_int(*convert);
	  buff[0] = 0x00;
	  nb=serializeInt(*convert); 
	for(i=0; i<lng2+1; i++){
	    buff[i+1]=nb[1+i];
		printf("conver de i vaut: \t %d\n",buff[i+1]);
	  }
	  //buff[1] = lng2;
 	  
	  lng = lng1+lng2+2;
	  tmp = malloc(sizeof(char)*(lng));
	  memcpy(tmp, serialType, lng1);
  
	  for(i=0; i<(lng2+2); i++){
	    tmp[i+lng1]=buff[i];
	  }
	  //printf("tmp %d\n", tmp[4]);
	  serial = malloc(sizeof(char)*(lng));
	  memcpy(serial, tmp, lng);
	  return serial;
  }
  else{//si le pointeur de argv est un pointeur sur char
	  lng1 = strlen(serialType);
 
	  convert = (int *)argv.arg;
	  printf("\n i am \t %d\n",*convert);
	   lng2 = taille_int(*convert);


	  buff[0] = 0x00;
  
	  for(i=0; i<lng2; i++){
	    buff[i+2]=convert[i];
		printf("conver de i vaut: \t %c\n",buff[i+2]);
	  }
	  buff[1] = lng2;
 
	  lng = lng1+lng2+2;
	  tmp = malloc(sizeof(char)*(lng));
	  memcpy(tmp, serialType, lng1);
  
	  for(i=0; i<(lng2+2); i++){
	    tmp[i+lng1]=buff[i];
	  }
	  printf("tmp %d\n", tmp[4]);
  
	  serial = malloc(sizeof(char)*(lng));
	  memcpy(serial, tmp, lng);

	  return serial;
  }
  
}



int main(){
	serializeInt(345);
	int n = 1284;
	  struct arg ar;
	  ar.type = 1;
	  ar.arg = &n;
	  char * a = serializeArg(ar);
	  printf("arg : %d%d %c %d%d %s\n", a[0],a[1],a[2],a[3],a[5],a);
}