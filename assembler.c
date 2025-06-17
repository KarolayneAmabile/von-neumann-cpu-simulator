#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read(char *filename);
int getOpcode(char *mnemonic);
void printMem();

unsigned char memory[154] = {0};

// argc: will be the count of input arguments to your program
// argv: will be a pointer to all the input arguments
int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Ohh nooo la politiza");
        return 0;
    }
    read(argv[1]);
    printMem();
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
        if (strcmp(tokens[1], "d") == 0) { // data
            int data = (int)strtol(tokens[2], NULL, 16);
            memory[memoryAddress] = data >> 8;
            memory[++memoryAddress] = data & 0xff;
            
            
            
        } else if (strcmp(tokens[1], "i") == 0) { // instrucions
            word = getOpcode(tokens[2]);
            if (word == 0) { 
                memory[memoryAddress] = 0;
            } else if (word == 1) {
                memory[memoryAddress] = 1;
            } else if (word >= 2 && word <= 12) {
                // 0000 0000 000X XXXX
                word = word << 2;
                // 0000 0000 0XXX XX00
                int rX = atoi(tokens[3] + 1);
                word = (word | rX) << 2;
                // 0000 0000 0XXX XXYY
                // 0000 000X XXXX YY00
                int rY = atoi(tokens[4] + 1);
                word = (word | rY) << 7;
                // 0000 000X XXXX YYZZ
                // XXXX XYYZ Z000 0000

                memory[memoryAddress] = word >> 8;
                memory[++memoryAddress] = word & 0xff;

            } else if (word == 13) {
                word = word << 2;
                int reg = atoi(tokens[3] + 1);
                word = (word | reg) << 1;
                memory[memoryAddress] = word;

            } else if (word >= 14 && word <= 20) {
                word = word << 19;

                int address = (int)strtol(tokens[3], NULL, 16);
                word = word | address;

                memory[memoryAddress] = word >> 16;
                memory[++memoryAddress] = (word & 0xff00) >> 8;
                memory[++memoryAddress] = (word & 0xff);
                // c0 0 20
                // c 0 22
            } else if (word >= 21 && word <= 29) {
                word = word << 2;

                int reg = atoi(tokens[3] + 1);
                word = (word | reg) << 17;                  

                int immOrAdd = (int)strtol(tokens[4], NULL, 16);  
                word = word | immOrAdd;                           

                memory[memoryAddress] = word >> 16;
                memory[++memoryAddress] = (word & 0xff00) >> 8;
                memory[++memoryAddress] = (word & 0xff);

            } else {
                printf("Syntax error on line: %i - %s", line, error);
                printf("Unknown instruction. Check the language documentation for valid instructions.\n");
                return;
            }
        } else {
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
    return -1;
}

void printMem() {
    for (int i = 0; i < 20; i++) {
        printf("%X\n", memory[i]);
    }
}
// A8 00 1E
// AA 00 20
// 20 80
// C0 00 14
// B0 00 22