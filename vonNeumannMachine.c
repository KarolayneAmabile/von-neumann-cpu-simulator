#include <stdio.h> 
#include <conio.h> 
#include <stdlib.h>

#include "assembler.h"

unsigned char memory[154] = {0};
unsigned int mbr = 0; // Memory Buffer Register
unsigned short int mar = 0; // Memory Address Register
unsigned char ir = 0; // Instruction Register
unsigned char ro0 = 0, ro1 = 0; // Register Operand 0 and 1
unsigned short int imm = 0; // Immediate
unsigned short int pc = 0; // Program Counter
unsigned char e = 0, l = 0, g = 0; // ‘equal to’, ‘lower than’ and ‘greater than’

unsigned short int reg[4]; // General purpose registers
// r0 : 00 | r1 : 01 | r2 : 10 | r3 : 11

int running = 0;

void printPCStatus();

void fetch();
void decode();
void execute();

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Error: No input file specified. Please provide the filename as a command-line argument.\n");
        printf("Press Enter to exit...\n");
        getchar();
        exit(1);
        return 1;
    }
    loadProgram(argv[1], memory);

    do {
        fetch();
        decode();
        execute();
        printPCStatus();

        if (running == 1) {
            char r = '\0';
            printf("\n\nPROGRAMA FINALIZADO.\nDeseja executar o mesmo programa novamente? (y/n)");
            scanf("%c", &r);
            if (r == 'y' || r == 'Y') {
                running = 0;
                pc = 0;
                    printf("\n\nPressione uma tecla para iniciar o proximo ciclo da maquina "
                    "ou aperte CTRL + C para finalizar a execucao do programa.");
            }
        }
        getchar();
        system("cls");
        
    } while (running == 0);
}

void printPCStatus() {
    /*==============================[ CPU ]==============================
| R0:   0 | R1:   0 | R2:   0 | R3:   0 |
| MBR:  0 | MAR:  0 | IMM:  0 | PC:   0 |
| IR:   0 | RO0:  0 | RO1:  0 |
| E:    0 | L:    0 | G:    0 |
==================================================================*/
    printf("CPU:\n");
    printf("R0: %X\t R1: %X\t R2: %X\t R3: %X\n", reg[0], reg[1], reg[2], reg[3]);
    printf("MBR: %X\t MAR: %X\t IMM: %X\t PC: %X\n", mbr, mar, imm, pc);
    printf("IR: %X\t RO0: %X\t RO1: %X\n", ir, ro0, ro1);
    printf("E: %X\t L: %X\t G: %X\n\n", e, l, g);

    /*printf("Memory: \n");
    for (int i = 0, j = 13; i < 154; i++) {
        printf("%i: %X    ", i, memory[i]);
        if (i <= 13) {
            if (i < 10) {
                printf("  ");
            } else {
                printf(" ");
            }
            if (i == j) {
                printf("\n");
                j += 14;
            }
        } else if (i <= 99) {
            printf(" ");
            if (i == j) {
                printf("\n");
                j += 14;
            }
        } else {
            if (i == j) {
                printf("\n");
                j += 14;
            }
        }
    }*/

    printf("\n\nPressione uma tecla para iniciar o proximo ciclo da maquina "
            "ou aperte CTRL + C para finalizar a execucao do programa.");
}   

void fetch() { 
    // instrucao de 1 byte
    mar = pc;
    mbr = memory[mar++];
    //printf("%X\n", mbr);
    ir = mbr >> 3;
    if (ir == 0 || ir == 1 || ir == 13) {
        return;
    }

    // instrucao de 2 bytes
    mbr = (mbr << 8) | memory[mar++];
    //printf("%X\n", mbr);
    if (ir >= 2 && ir <= 12) { 
        return;
    }

    // instrucao de 3 bytes
    mbr = (mbr << 8) | memory[mar];
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