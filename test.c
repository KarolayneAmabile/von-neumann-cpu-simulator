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

    while (fgets(str, sizeof(str), program) != NULL) { 
        strcpy(error, str);
        int wordPosition = 0;
        char *token = strtok(str, "; ,\n");

        while (token) {
            tokens[wordPosition++] = token;
            token = strtok(NULL, "; ,\n"); 
        }        
        // o array tokens possui cada pedaço da instrucao armazenado agora
        // 0;i;ld r0, 96
        // tokens[0] = posicao na memoria
        // tokens[1] = se é dado ou instrucao
        // para dado        tokens[2] = dado
        // para instrucao   tokens[2] = opcode
        if (strcmp(tokens[1], "d") == 0) { // yep, funciona perfeitamente pra dados
            int memoryAddress = (int)strtol(tokens[0], NULL, 16);
            int data = (int)strtol(tokens[2], NULL, 16);

            memory[memoryAddress] = data >> 8;
            memory[++memoryAddress] = data & 0xff;
        } else if (strcmp(tokens[1], "i") == 0) {
            // do
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