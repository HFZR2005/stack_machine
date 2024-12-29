#ifndef STACK_H
#define STACK_H 

#define MAX 100

typedef struct stack {
  int data[MAX];
  int top;
  int size;
} stack;

stack init_stack(void);
int isEmpty(stack *s);
int isFull(stack *s);
void push(stack *s, int x);
int pop(stack *s);
int peek(stack *s);


#endif
