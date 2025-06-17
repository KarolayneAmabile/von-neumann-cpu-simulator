#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loadProgram(char *filename, unsigned char *memory);
int getOpcode(char *mnemonic);


void loadProgram(char *filename, unsigned char *memory) {
    FILE* program;
    program = fopen(filename, "r");
    if (program == NULL) {
        printf("Error: Could not open file \"%s\" for reading.\n", filename);
        printf("Please make sure the provided file is a .txt file. Press Enter to exit...\n");
        getchar();
        exit(1);
    }

    char str[100];
    char *tokens[5];
    char error[100];
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

        if (strcmp(tokens[1], "d") == 0) { 
            int data = (int)strtol(tokens[2], NULL, 16);
            memory[memoryAddress] = data >> 8;
            memory[++memoryAddress] = data & 0xff;
        } else if (strcmp(tokens[1], "i") == 0) { 
            word = getOpcode(tokens[2]);
            if (word == 0) { 
                memory[memoryAddress] = 0;
            } else if (word == 1) {
                memory[memoryAddress] = 1;
            } else if (word >= 2 && word <= 12) {
                word = word << 2;

                int rX = atoi(tokens[3] + 1);
                word = (word | rX) << 2;

                int rY = atoi(tokens[4] + 1);
                word = (word | rY) << 7;

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
                printf("Unknown instruction \"%s\". Check the language documentation for valid instructions.\n", tokens[2]);
                printf("Press Enter to exit...\n");
                getchar();
                exit(1);
            }
        } else {
            printf("Syntax error on line: %i - %s", line, error);
            printf("Invalid type \"%s\". Only \"i\" (instruction) or \"d\" (data) are allowed.\n", tokens[1]);
            printf("Press Enter to exit...\n");
            getchar();
            exit(1);
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

