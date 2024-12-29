#include "stack.h"
#include <assert.h>
#include <stdio.h>

// remember when we return, local variables are deallocated so unless we return by value we have undefined behaviour. Using pointers to local variables (which are eventually deallocated) results in undefined behaviour.
stack init_stack(void) {
  stack s = {
  .top = 0,
  .size = 0, 
  };
  return s;
}

int isEmpty(stack *s) {
  return s->size == 0;
}

int isFull(stack *s) {
  return s->size == MAX;
}

void push(stack *s, int x) {
  assert(!isFull(s));

  s->data[++s->top] = x;
  s->size++;
}

int pop(stack *s) {
  assert(!isEmpty(s));

  s->size--;
  return s->data[s->top--];
}

int peek(stack* s) {
  assert(!isEmpty(s));
  return s->data[s->top];
}









