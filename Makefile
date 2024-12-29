DEBUG_FLAGS = -fsanitize=address -fsanitize=undefined -g -fno-omit-frame-pointer -Wall -pedantic


all: 
	gcc -c stack_machine.c 
	gcc -c stack.c
	gcc -o s stack_machine.o stack.o

sane: 
	gcc ${DEBUG_FLAGS} -c stack_machine.c 
	gcc ${DEBUG_FLAGS} -c stack.c 
	gcc ${DEBUG_FLAGS} -o s stack_machine.o stack.o

tests: 
	gcc -c stack.c
	gcc -c test_stack.c
	gcc -o test_stack stack.o test_stack.o

clean:
	rm -f *.o
