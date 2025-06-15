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

    while (fgets(str, sizeof(str), program) != NULL) { // line tem o conteudo de uma linha
        // 0;i;ld r0, 96
        int wordPosition = 0;
        char *token = strtok(str, "; ,\n");
        // when we pass all the parameters of the string, this function
        // will return only the *first* token of the string inpu
        // after that, we *need to pass 'NULL'* insted of the input
        // string to obtain the following toknes. it will return NULL
        // when there is no token left
        
        while (token) {
            tokens[wordPosition] = token;
            printf("%s", tokens[wordPosition]);
            wordPosition++;
            token = strtok(NULL, "; ,\n"); 
        }        
        printf("\n");
    }
    fclose(program);
}

// char* strtok(char *str, const char *delimiter)
// str: Original String that is to be processed
// delimiter: The separator using which we tokenize the string.