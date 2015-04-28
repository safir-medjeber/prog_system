#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include "Polonaise.h"


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
            if(token[0]=='+'||token[0]=='-'||token[0]=='*'||token[0]=='/'||token[0]=='^'){
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
