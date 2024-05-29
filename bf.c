#include <string.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <stdlib.h>

#define STACK_SIZE 10000 
#define MEM_SIZE   3000 
#define BF_C_SIZE  8
#define LINE_SIZE  256 
#define EOF_CH     '\n'  

const char * BF_C = "+-<>[],."; 

typedef struct{
    int memory[MEM_SIZE];
    char code[STACK_SIZE]; 
    int jump_i[STACK_SIZE]; 
    int jumps[STACK_SIZE]; 
    int intr_ptr; //instruction pointer
    int data_ptr; //data pointer
    int program_size; 
} Program; 

typedef struct{
   int stack[STACK_SIZE]; 
   int ptr; 
} IntStack; 

bool is_bf(char l){
    for (int i = 0; i < BF_C_SIZE; i++){
        if (BF_C[i] == l) return true; 
    }
    return false; 
}

void find_all_jumps(Program *program){
    IntStack stk = {0}; 
    for (int i = 0; i < program->program_size; i++){
        char c = program->code[i]; 
        if (c == '['){
            stk.stack[stk.ptr++] = i; 
        }else if(c ==']'){
            int prev_i = stk.stack[--stk.ptr]; 
            program->jumps[prev_i] = i; 
            program->jump_i[i] = prev_i;
        }
    }
}   

int init_program(Program *program, const char *file_name){
    FILE *file;
    char ch;

    file = fopen(file_name, "r"); 
    if (file == NULL){
        printf("Error opening file: %s\n", file_name); 
        return 1; 
    }

    while ((ch = fgetc(file)) != EOF) {
        if (!is_bf(ch)) continue; 
        if (program->program_size >= STACK_SIZE){
            printf("Program too large.\n"); 
            return 1; 
        }
        program->code[program->program_size++] = ch; 
    }

    

    fclose(file); 
    return 0;
}

void run_bf(Program * program){
    while(true){
        if (program->intr_ptr >= program->program_size) break; 
        char c = program->code[program->intr_ptr]; 
        if (c == '>'){
            program->data_ptr += 1; 
        }else if (c == '<'){
            program->data_ptr -= 1; 
        }else if (c == '+'){
            program->memory[program->data_ptr] += 1; 
        }else if (c == '-'){
            program->memory[program->data_ptr] -= 1;
        }else if (c == '.'){
            char out = (char) program->memory[program->data_ptr];
            printf("%c", out); 
        }else if(c == ','){
            char inpt; 
            scanf("%c", &inpt); 
            getchar(); 
            if (inpt == EOF_CH){
                program->intr_ptr += 1;
                continue; 
            }
            program->memory[program->data_ptr] = (int) inpt; 
        }else if(c == '['){
            int jmp = program->memory[program->data_ptr]; 
            if (jmp == 0){
                program->intr_ptr = program->jumps[program->intr_ptr]; 
            }
        }else if (c == ']'){
            int jmp = program->memory[program->data_ptr]; 
            if (jmp != 0){
                program->intr_ptr = program->jump_i[program->intr_ptr]; 
            }
        }
        program->intr_ptr += 1;
    }
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: %s <bf file ending with .bf>\n", argv[0]);
        return 1;
    }

    Program program = {0}; 
    const char* program_name = argv[1]; 
    if (init_program(&program, program_name) != 0) return 1; 
    find_all_jumps(&program);  
    run_bf(&program);

    return 0; 
}
