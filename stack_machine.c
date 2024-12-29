#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "stack.h"

#define MAX_INSTRUCTIONS 100

typedef enum instr_type {
  PUSH,
  POP,
  ADD,
  ADDI,
  MUL,
  JUMP,
  SWAP,
  BEQZ,
  LOAD,
  STORE,
  DUP,
  NUM_INSTR_TYPES
}instr_type;


bool takes_argument [NUM_INSTR_TYPES] = {
  [PUSH] = true,
  [POP] = false,
  [ADD] = false,
  [ADDI] = true, 
  [MUL] = false,
  [JUMP] = true,
  [SWAP] = false,
  [BEQZ] = true,
  [LOAD] = false,
  [STORE] = false,
  [DUP] = false,
};


char* get_instr_name [NUM_INSTR_TYPES] = {
  [PUSH] = "PUSH",
  [POP] = "POP",
  [ADD] = "ADD",
  [ADDI] = "ADDI",
  [MUL] = "MUL",
  [JUMP] = "JUMP",
  [SWAP] = "SWAP",
  [BEQZ] = "BEQZ",
  [LOAD] = "LOAD",
  [STORE] = "STORE",
  [DUP] = "DUP",
};

typedef struct instr {
  enum instr_type type;
  int arg;
} instr;

typedef struct CPU {
  stack* s;
  int pc;
  int instr_count;
  int store;
  instr* instrs;
} CPU;

CPU init_cpu(void){
  CPU cpu;
  stack* s = (stack*)malloc(sizeof(stack));
  memset(s->data, 0, sizeof(s->data));
  s->top = -1;
  s->size = 0;
  cpu.s = s;
  cpu.pc = 0;  
  cpu.instrs = (instr*)malloc(MAX_INSTRUCTIONS * sizeof(instr));
  cpu.instr_count = 0;
  return cpu;
}

instr_type get_instr_type(char* instr){
  if(strcmp(instr, "PUSH") == 0){
    return PUSH;
  } else if(strcmp(instr, "POP") == 0){
    return POP;
  } else if(strcmp(instr, "ADD") == 0){
    return ADD;
  } else if(strcmp(instr, "SWAP") == 0){
    return SWAP;
  } else if (strcmp(instr, "BEQZ") == 0){
    return BEQZ;
  } else if (strcmp(instr, "LOAD") == 0){
    return LOAD;
  } else if (strcmp(instr, "STORE") == 0){
    return STORE;
  } else if (strcmp(instr, "DUP") == 0){
    return DUP;
  } else if (strcmp(instr, "ADDI") == 0){
    return ADDI; 
  } else if (strcmp(instr, "MUL") == 0){
    return MUL;
  } else if (strcmp(instr, "JUMP") == 0){
    return JUMP;
  } else {
    printf("Error: Unknown instruction %s\n", instr);
    exit(1);
  }
}

void execute_instr(CPU* cpu, instr instruction){
  stack* s = cpu->s;
  int num1;
  int num2; 
  switch(instruction.type){
    case PUSH:
      push(s, instruction.arg);
      cpu->pc++;
      break; 
    case POP:
      pop(s); 
      cpu->pc++;
      break;
    case ADD:
      num1 = pop(s);
      num2 = pop(s);
      push(s, num1 + num2);
      cpu->pc++;
      break;
    case JUMP:
      assert(instruction.arg < cpu->instr_count);
      cpu->pc = instruction.arg;
      break;
    case SWAP:
      num1 = pop(s);
      num2 = pop(s);
      push(s, num1);
      push(s, num2);
      cpu->pc++;
      break; 
    case BEQZ:
      assert(instruction.arg < cpu->instr_count);
      if (peek(cpu->s) == 0){
        cpu->pc = instruction.arg;
      } else {
        cpu->pc++;
      }
      break;
    case LOAD:
      push(cpu->s, cpu->store);
      cpu->pc++;
      break;
    case STORE:
      num1 = pop(cpu->s);
      cpu->store = num1;
      cpu->pc++;
      break;
    case DUP:
      num1 = peek(cpu->s);
      push(cpu->s, num1);
      cpu->pc++;
      break;
    case ADDI:
      num1 = pop(cpu->s) + instruction.arg;
      push(cpu->s, num1);
      cpu->pc++;
      break;
    case MUL:
      num1 = pop(cpu->s);
      num2 = pop(cpu->s);
      push(cpu->s, num1*num2);
      cpu->pc++;
      break;
    default:
      break;
  }

}

void execute_instrs(CPU* cpu){
  printf("INSTR COUNT: %d\n", cpu->instr_count);
  while(cpu->pc < cpu->instr_count){
    execute_instr(cpu, cpu->instrs[cpu->pc]);
  }
  printf("RESULT: %d \n", peek(cpu->s));
}

int main(int argc, char** argv){
  
  if(argc != 2){
    printf("Usage: %s <input file>\n", argv[0]);
    return 1;
  }
  
  char* input_file = argv[1];
  FILE* file = fopen(input_file, "r");
  
  if(file == NULL){
    printf("Error: Unable to open file %s\n", input_file);
    return 1;
  }

  // Read the file
  char buffer[50]; 

  
  CPU cpu = init_cpu();
  int i = 0;
  instr* instructions = cpu.instrs;

  while(fgets(buffer, 32, file) != NULL){
    char* token = strtok(buffer, " ");
    
    if (token[strlen(token) - 1] == '\n'){
      token[strlen(token) - 1] = '\0';
    }
    
    instructions[i].type = get_instr_type(token);
    token = strtok(NULL, " ");
    
    if (takes_argument[instructions[i].type]){
      instructions[i].arg = atoi(token);
    } else {
      instructions[i].arg = 0;
    }
    
    i ++;   
    cpu.instr_count ++;
  }
  
  fclose(file);

  for(int j = 0; j < i; j++){
    instr instr = instructions[j];
    if (takes_argument[instr.type]){
      printf("%s %d\n", get_instr_name[instr.type], instr.arg);
    } else {
      printf("%s\n", get_instr_name[instr.type]);
    }
  }

  execute_instrs(&cpu);
}
