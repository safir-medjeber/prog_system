#include <stdio.h>
#include <stdlib.h>

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

char* intToChar(int x){
	int tailleX= taille_int(x);
	char* res=malloc(tailleX+2*sizeof(char));
	res[0]=1;
	res[1]=tailleX;
	int i;
	for(i=tailleX+1; i >= 2;i--){
		res[i]='0'+x%10;
		x=x/10;
	}
	return res;
	
}
int main(){
	int n;
	printf("donner un nombre");
	scanf ("%d",&n);
	char *a =intToChar(n);
	char* tmp=a;
	int taille = tmp[1];
	int i;
	for(i=0;i<taille;i++){
		printf("%c",a[i+2]);
	}
	free(a);	
}