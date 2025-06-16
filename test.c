#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read(char *filename);

unsigned char memory[154] = {0};

// argc: will be the count of input arguments to your program
// argv: will be a pointer to all the input arguments
int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Ohh nooo la politiza");
        return 0;
    }
    read(argv[1]);
}

void read(char *filename) {
    FILE* program;
    program = fopen(filename, "r");
    if (program == NULL) {
        printf("Houve um problema na leitura do aquivo.");
        return;
    }

    char str[100];
    char *tokens[5]; // posicaoNaMemoria;dadoOuInstrucao;palavra
    char error[100]; // para exibir mensagem de erro
    int line = 0;
    int memoryAddress = 0;
    int word = 0;

    while (fgets(str, sizeof(str), program) != NULL) { 
        strcpy(error, str);
        int wordPosition = 0;
        char *token = strtok(str, "; ,\n");

        while (token) {
            tokens[wordPosition++] = token;
            token = strtok(NULL, "; ,\n"); 
        }        

        memoryAddress = (int)strtol(tokens[0], NULL, 16);
        // o array tokens possui cada pedaço da instrucao armazenado agora
        // 0;i;ld r0, 96
        // tokens[0] = posicao na memoria
        // tokens[1] = se é dado ou instrucao
        // para dado        tokens[2] = dado
        // para instrucao   tokens[2] = opcode
        if (strcmp(tokens[1], "d") == 0) { // yep, funciona perfeitamente pra dados
            int data = (int)strtol(tokens[2], NULL, 16);

            memory[memoryAddress] = data >> 8;
            memory[++memoryAddress] = data & 0xff;
            //
            //
            //
        } else if (strcmp(tokens[1], "i") == 0) {
            if (strcmp(tokens[2], "hlt") == 0) { 
                memory[memoryAddress] = 0;
            } else if (strcmp(tokens[2], "nop") == 0) {
                memory[memoryAddress] = 1;



            } else if (strcmp(tokens[2], "ldr") == 0) {
                word = 2 << 2;
                word = (word | atoi(tokens[3] + 1)) << 2;
                word = (word | atoi(tokens[4] + 1)) << 7;
                memory[memoryAddress] = word >> 8;
                memory[++memoryAddress] = word & 0xff;

            } else if (strcmp(tokens[2], "str") == 0) {
                word = 3 << 2;
            } else if (strcmp(tokens[2], "add") == 0) {
                word = 4 << 2;
            } else if (strcmp(tokens[2], "sub") == 0) {
                word = 5 << 2;
            } else if (strcmp(tokens[2], "mul") == 0) {
                word = 6 << 2;
            } else if (strcmp(tokens[2], "div") == 0) {
                word = 7 << 2;
            } else if (strcmp(tokens[2], "cmp") == 0) {
                word = 8 << 2;
            } else if (strcmp(tokens[2], "movrr") == 0) {
                word = 9 << 2;
            } else if (strcmp(tokens[2], "and") == 0) {
                word = 10 << 2;
            } else if (strcmp(tokens[2], "or") == 0) {
                word = 11 << 2;
            } else if (strcmp(tokens[2], "xor") == 0) {
                word = 12 << 2;
            




                
            } else if (strcmp(tokens[2], "nop") == 0) {
                
            } else if (strcmp(tokens[2], "div") == 0) {
                
            } else if (strcmp(tokens[2], "div") == 0) {
                
            } else if (strcmp(tokens[2], "div") == 0) {
                
            } else if (strcmp(tokens[2], "div") == 0) {
                
            } else if (strcmp(tokens[2], "div") == 0) {
                
            } else if (strcmp(tokens[2], "div") == 0) {
                
            } else if (strcmp(tokens[2], "div") == 0) {
                
            } else if (strcmp(tokens[2], "div") == 0) {
                
            } else if (strcmp(tokens[2], "div") == 0) {
                
            } else if (strcmp(tokens[2], "div") == 0) {
                
            } else if (strcmp(tokens[2], "div") == 0) {
                
            } else if (strcmp(tokens[2], "div") == 0) {
                
            } else if (strcmp(tokens[2], "div") == 0) {
                
            } else {
                //
            }
        //
        //
        //
        //
        } else {
            //"Erro: tipo inválido \"%s\". Apenas \"i\" (instrução) ou \"d\" (dado) são permitidos.\n", tokens[1])
            printf("Syntax error on line: %i - %s", line, error);
            printf("Invalid type \"%s\". Only \"i\" (instruction) or \"d\" (data) are allowed.\n", tokens[1]);
            return;
        }

        line++;
    }
    fclose(program);
}

int getOpcode(char *mnemonic) {
    if (strcmp(mnemonic, "hlt") == 0) return 0;
    if (strcmp(mnemonic, "nop") == 0) return 1;

    if (strcmp(mnemonic, "ldr") == 0) return 2;
    if (strcmp(mnemonic, "str") == 0) return 3;
    if (strcmp(mnemonic, "add") == 0) return 4;
    if (strcmp(mnemonic, "sub") == 0) return 5;
    if (strcmp(mnemonic, "mul") == 0) return 6;
    if (strcmp(mnemonic, "div") == 0) return 7;
    if (strcmp(mnemonic, "cmp") == 0) return 8;
    if (strcmp(mnemonic, "movrr") == 0) return 9;
    if (strcmp(mnemonic, "and") == 0) return 10;
    if (strcmp(mnemonic, "or") == 0) return 11;
    if (strcmp(mnemonic, "xor") == 0) return 12;

    if (strcmp(mnemonic, "not") == 0) return 13;

    if (strcmp(mnemonic, "je") == 0) return 14;
    if (strcmp(mnemonic, "jne") == 0) return 15;
    if (strcmp(mnemonic, "jl") == 0) return 16;
    if (strcmp(mnemonic, "jle") == 0) return 17;
    if (strcmp(mnemonic, "jg") == 0) return 18;
    if (strcmp(mnemonic, "jge") == 0) return 19;
    if (strcmp(mnemonic, "jmp") == 0) return 20;

    if (strcmp(mnemonic, "ld") == 0) return 21;
    if (strcmp(mnemonic, "st") == 0) return 22;

    if (strcmp(mnemonic, "movi") == 0) return 23;
    if (strcmp(mnemonic, "addi") == 0) return 24;
    if (strcmp(mnemonic, "subi") == 0) return 25;
    if (strcmp(mnemonic, "muli") == 0) return 26;
    if (strcmp(mnemonic, "divi") == 0) return 27;
    if (strcmp(mnemonic, "lsh") == 0) return 28;
    if (strcmp(mnemonic, "rsh") == 0) return 29;
}