#ifndef STACK_H_INCLUDED

#define STACK_H_INCLUDED



#define MAXSIZE 400
 
struct stack
{
    int stk[MAXSIZE];
    int top;
};
typedef struct stack STACK;
 
void pushStack(STACK *s,int a);
int  popStack(STACK *s);
void display(STACK *s);
void clearStack(STACK *s);
 
#endif