#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <ctype.h>
#include "Polonaise.h"

/* algo 
ant qu'il y a des tokens en entrée {

    Examiner le token courant sur le fichier d'entrée
    Si c'est un opérande, le placer sur le fichier de sortie
    Si c'est une parenthèse ouvrante, la mettre sur la pile
    Si c'est un opérateur, alors
        Si la pile est vide, pousser l'opérateur sur la pile
        Si le sommet de la pile est une parenthèse ouvrante, pousser l'opérateur sur la pile
        Si l'opérateur est prioritaire sur celui au sommet de la pile, pousser l'opérateur sur la pile
        Sinon, enlever l'opérateur de la pile et le mettre sur le fichier de sortie. Replacer ensuite l'opérateur courant sur la pile
    Si c'est une parenthèse fermante, enlever les opérateurs de la pile et les placer sur le fichier de sortie jusqu'à ce que l'on rencontre la parenthèse ouvrante, que l'on élimine.

}

Enlever tous les opérateurs restants et les placer sur le fichier de sortie. 

*/

char* toPolonaise(char* expr)
{
    //char expr[400];

 //   scanf("%lu\n",&test_case);
	char stack[400];
	unsigned long top=0;
	int len;
	       // scanf("%s\n",expr);
		//sprintf(expr,"6 * ( 4 + 5 ) - 92 / ( 2 + 3 )");
        len=strlen(expr);
		char* rep = (char*)malloc(len*sizeof(char));
		printf("tu as demande a transformer %s\n",expr);
		char* reponse=rep;
	    const char s[2] = " ";		
	    char *token ;
	    token = strtok(expr, s);
        while(token != NULL)
        {
			if( (isdigit(token[0])) || ( token[0]=='-' && isdigit(token[1])) ){
                printf("%s ",token);
				while(*token!='\0'){
					*rep=*token;
					token++;
					rep++;
				}
				*rep = ' ';
				rep++;
			}
            else if(token[0]=='+'||token[0]=='-'||token[0]=='*'||token[0]=='/'||token[0]=='^'){
            	if(top==0 || stack[top]=='('){
            		push(token[0],&top,stack);;
            	}
				else if((token[0]=='*' || token[0]=='/') &&(stack[top]=='-' || stack[top]=='+')){
					push(token[0],&top,stack);;
				}
				else{
					printf("%c ",stack[top]);
					*rep=stack[top];
					rep++;
					*rep = ' ';
					rep++;
					stack[top]=token[0];
				}
            }

            else if(token[0]==')')
            {
                while(stack[top]!='('){
					*rep=stack[top];
					rep++;
                	printf("%c ",stack[top--]);
					*rep = ' ';
					rep++;
                }
				top--;
            }

            else if(token[0]=='(')
               push(token[0],&top,stack);

            else{
                printf("%s ",token);
				while(*token!='\0'){
					*rep=*token;
					token++;
					rep++;
				}
				*rep = ' ';
				rep++;
            	
            }
	        token = strtok(NULL, s);
        }
		while(top>0){
			*rep=stack[top];
			rep++;
			*rep= ' ';
			rep++;
			printf("%c ",stack[top--]);
		}
		*rep='\0';
		printf("\n%s\n",rep);
    return reponse;
}


void push(char x,unsigned long* top,char* stack)
{
    if(*top==400)
        return;
    else
    {
        stack[++(*top)]=x;
    }

}
