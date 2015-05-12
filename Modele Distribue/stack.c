

/*
 * C program to implement stack. Stack is a LIFO data structure.
 * Stack operations: PUSH(insert operation), POP(Delete operation)
 * and Display stack.
 */
#include <stdio.h>
#include "stack.h"

/*  Function to add an element to the stack */
void pushStack (STACK *s,int num)
{
    if (s->top == (MAXSIZE - 1))
    {
        printf ("Stack is Full\n");
        return;
    }
    else
    {
        s->top = s->top + 1;
        s->stk[s->top] = num;
    }
    return;
}
/*  Function to delete an element from the stack */
int popStack (STACK *s)
{
    int num;
    if (s->top == - 1)
    {
        printf ("Stack is Empty\n");
        return (s->top);
    }
    else
    {
        num = s->stk[s->top];
       // printf ("poped element is = %d\n", s->stk[s->top]);
        s->top = s->top - 1;
    }
    return(num);
}
/*  Function to display the status of the stack */
void display (STACK *s)
{
    int i;
    if (s->top == -1)
    {
        printf ("Stack is empty\n");
        return;
    }
    else
    {
        printf ("\n The status of the stack is \n");
        for (i = s->top; i >= 0; i--)
        {
            printf ("%d\n", s->stk[i]);
        }
    }
    printf ("\n");
}

void clearStack(STACK *s){
	while (s->top > -1){
		popStack(s);
	}
}

