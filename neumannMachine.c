#include <stdio.h> 
#include <conio.h> 
#include <stdlib.h>
#include <string.h>

#include "assembler.h"

#define SIZE 154

unsigned char memory[SIZE] = {0};
unsigned int mbr = 0; // Memory Buffer Register
unsigned short int mar = 0; // Memory Address Register
unsigned char ir = 0; // Instruction Register
unsigned char ro0 = 0, ro1 = 0; // Register Operand 0 and 1
unsigned short int imm = 0; // Immediate
unsigned short int pc = 0; // Program Counter
unsigned char e = 0, l = 0, g = 0; // ‘equal to’, ‘lower than’ and ‘greater than’

unsigned short int reg[4] = {0};// General purpose registers

int running = 0;
int cicles = 0;
char programLines[200][20] = {0};

void fetch();
void decode();
void execute();

void getSourceFile(char *filename);
void displayCPUStatus();

int main(int argc, char **argv) {
    
    char filename[300];
    if (argc == 1) {
        printf("Please type the name of the .txt file: ");
        fgets(filename, 300, stdin);
    } else {
        strcpy(filename, argv[1]);
    }

    if (filename[0] == '\n') {
        printf("Error: No input file provided. Please enter a valid filename next time.\n");
        printf("Press Enter to exit...\n");
        getchar();
        exit(1);

    }
    filename[strcspn(filename, "\n")] = 0;
    loadProgram(filename, memory);
    getSourceFile(filename);

    do {
        fetch();
        decode();
        execute();

        displayCPUStatus();
        cicles++;

        if (running == 1) {
            char r = '\0';
            printf("\n\nPROGRAM FINISHED.\nDo you want to run the same program again? (y/n)");
            scanf("%c", &r);
            if (r == 'y' || r == 'Y') {
                running = 0, pc = 0, cicles = 0;
            }
        }
        getchar();
        system("cls");
        
    } while (running == 0);
}

void fetch() { 
    // instrucao de 1 byte
    mar = pc;
    mbr = memory[mar];
    //printf("%X\n", mbr);
    ir = mbr >> 3;
    if (ir == 0 || ir == 1 || ir == 13) {
        return;
    }

    // instrucao de 2 bytes
    mbr = (mbr << 8) | memory[++mar];
    //printf("%X\n", mbr);
    if (ir >= 2 && ir <= 12) { 
        return;
    }

    // instrucao de 3 bytes
    mbr = (mbr << 8) | memory[++mar];
    //printf("%X\n", mbr);

}

void decode() {
   if (ir == 1) {
        pc++;
   } else if (ir >= 2 && ir <= 12) {
        ro0 = (mbr & 0x600) >> 9;
        ro1 = (mbr & 0x180) >> 7;
   } else if (ir == 13) {
        ro0 = (mbr & 0x6);
   } else if (ir >= 14 && ir <= 20) {
        pc = (mbr & 0xffff);
   } else if (ir == 21 || ir == 22) {
        ro0 = (mbr & 0x60000) >> 17;
        mar = (mbr & 0xffff);

        //printf("ro0 %X\n", ro0);
        //printf("mar %X\n", mar);
   } else {
        ro0 = (mbr & 0x60000) >> 17;
        imm = (mbr & 0xffff);

   }
}

void execute() {
    if (ir == 0) { // hlt
        running = 1;
    } else if (ir == 2) { // ldr
        reg[ro0] = reg[ro1];
        pc += 2;
    } else if (ir == 3) { // str
        reg[ro1] == reg[ro0];
        pc += 2;
    } else if (ir == 4) { // add
        reg[ro0] += reg[ro1];
        pc += 2;
    } else if (ir == 5) { // sub
        reg[ro0] -= reg[ro1];
        pc += 2;
    } else if (ir == 6) { // mul
        reg[ro0] *= reg[ro1];
        pc += 2;
    } else if (ir == 7) { // div
        reg[ro0] /= reg[ro1];
        pc += 2;
    } else if (ir == 8) { // cmp
        e = reg[ro0] = reg[ro1] ? 1 : 0;
        l = reg[ro0] < reg[ro1] ? 1 : 0;
        g = reg[ro0] > reg[ro1] ? 1 : 0;
        pc += 2;
    } else if (ir == 9) { // movr
        reg[ro0] = reg[ro1];
        pc += 2;
    } else if (ir == 10) { //and
        reg[ro0] = reg[ro0] & reg[ro1];
        pc += 2;
    } else if (ir == 11) { // or
        reg[ro0] = reg[ro0] | reg[ro1];
        pc += 2;
    } else if (ir == 12) { //xor
        reg[ro0] = reg[ro0] ^ reg[ro1];
        pc += 2;
    } else if (ir == 13) { // not
        reg[ro0] = !reg[ro0];
        pc++;
    } else if (ir == 14) { // jump if equal to
        if (e == 1) {
            pc = mar;
        }
        pc += 3;
    } else if (ir == 15) { // jump if not equal to
        if (e == 0) {
            pc = mar;
        }
        pc += 3;
    } else if (ir == 16) { // jump if lower than
        if (l == 0) {
            pc = mar;
        }
        pc += 3;
    } else if (ir == 17) { // jump if lower than or equal to
        if (e == 1 || l == 1) {
            pc = mar;
        }
        pc += 3;
    } else if (ir == 18) { // jump if greater than
        if (g == 1) {
            pc = mar;
        }
        pc += 3;
    } else if (ir == 19) { // jump if greater than or equal to
        if (e == 1 || g == 1) {
            pc = mar;
        }
        pc += 3;
    } else if (ir == 20) { // jump
        pc = mar;
        pc += 3;
    } else if (ir == 21) { // load
        mbr = memory[mar++];
        mbr = (mbr << 8) | memory[mar];
        reg[ro0] = mbr;
        pc += 3;

    } else if (ir == 22) { // store
        mbr = reg[ro0];
        memory[mar] = mbr >> 8;
        memory[++mar] = mbr & 0xff;
        pc += 3;
    } else if (ir == 23) { // move immediate
        reg[ro0] = imm;
        pc += 3;
    } else if (ir == 24) { // add immediate
        reg[ro0] += imm;
        pc += 3;
    } else if (ir == 25) { // subtract immediate
        reg[ro0] -= imm;
        pc += 3;
    } else if (ir == 26) { // multiply immediate
        reg[ro0] *= imm;
        pc += 3;
    } else if (ir == 27) { // divide immediate
        reg[ro0] /= imm;
        pc += 3;
    } else if (ir == 28) { // left shift
        reg[ro0] << imm;
        pc += 3;
    } else if (ir == 29) { // right shift
        reg[ro0] >> imm;
        pc += 3;
    }
}

void getSourceFile(char *filename) {
    FILE *program = fopen(filename, "r");
    if (program == NULL) {
        exit(1);
    }

    int size = 0;
    while (fgets(programLines[size], 30, program)) {
        programLines[size][strcspn(programLines[size], "\n")] = 0;
        size++;

    }
    fclose(program);
}
void displayCPUStatus() {
    char *screen[] = {
    "|_______________________________________________________________________________________________________________|",
    "|_________________________________________________CPU SIMULATOR_________________________________________________|",
    "|___________________________________________________REGISTERS___________________________________________________|",
    "|__________________________________________________MAIN MEMORY__________________________________________________|",
    "|                   Press any key to start the next machine cycle or press CTRL + C to exit.                    |"
    };
    printf("%s\n", screen[1]);

    printf("| Running line: %*s|\n", -96, programLines[cicles]);

    
    printf("%s\n", screen[2]);

    printf("|%*s R0:%*X | R1:%*X | R2:%*X | R3:%*X |%*s\n", 36, "|", 4, reg[0], 4, reg[1], 4, reg[2], 4, reg[3], 36, "|"); 

    printf("|%*s MBR:%*X | MAR:%*X | IMM:%*X | PC:%*X |%*s\n", 28, "|", 7, mbr, 7, mar, 7, imm, 7, pc, 29, "|");
    printf("|%*s IR:%*X | RO0:%*X | RO1:%*X |%*s\n", 40, "|", 4, ir, 4, ro0, 4, ro1, 40, "|");
    printf("|%*s E:%*X | L:%*X | G:%*X |%*s\n", 41, "|", 5, e, 5, l, 5, g, 41, "|");

    printf("|%*s\n%s\n", 112, "|", screen[3]);
    int qtd = 11;
    for (int i = 0; i < SIZE; i++) {
        printf("|%*i:%*X ", 3, i, 4, memory[i]);
        if (i == qtd - 1) {
            printf("  |\n");
            qtd += 11;
        }
    }
    printf("%s\n%s\n%s\n", screen[0], screen[4], screen[0]);
}