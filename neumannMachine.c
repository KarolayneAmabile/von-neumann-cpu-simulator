#include <stdio.h> 
#include <conio.h> 
#include <stdlib.h>
#include <string.h>

#include "assembler.h"

#define SIZE 154

/*atencao: comentarios que possuem o marcador <> indicam que aquele trecho, linha
ou funcao esta presente no codigo meramente para auxiliar no controle ou realizar
a impressao no formato certo em tela e pode ser ignorado */


// REGISTRADORES INTERNOS A CPU:
unsigned char memory[SIZE] = {0};
unsigned int mbr = 0; // Memory Buffer Register
unsigned short int mar = 0; // Memory Address Register
unsigned char ir = 0; // Instruction Register
unsigned char ro0 = 0, ro1 = 0; // Register Operand 0 and 1
unsigned short int imm = 0; // Immediate
unsigned short int pc = 0; // Program Counter
unsigned char e = 0, l = 0, g = 0; // ‘equal to’, ‘lower than’ and ‘greater than’
unsigned short int reg[4] = {0};// General purpose registers

// <> variaveis de controle do restante do codigo, que nao pertecem a cpu
int running = 0;
int cicles = 0;
char programLines[200][20] = {0};
int currentLine = 0;

// FUNCOES QUE EXECUTAM O CICLO DA MAQUINA DE VON NEUMANN:
void fetch();
void decode();
void execute();

//<> funcoes para impressao
void getSourceFile(char *filename);
int getInstruction();
void displayCPUStatus();

int main(int argc, char **argv) {
    // <> o nome do programa pode ser passado diretamente pela linha de comando,
    // <> ou em tempo de execucao
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

    // funcao que traduz o codigo fonte para binario e o carrega na memoria principal
    loadProgram(filename, memory);

    // <> funcao auxiliar, que obtem as strings que compoe a instrucao para imprimir na tela
    getSourceFile(filename);

    // loop que executa o ciclo da maquina de von Neumann
    do {
        currentLine = getInstruction(); //<>
        fetch();
        decode();
        execute();

        displayCPUStatus(); // <>
        cicles++; // <>

        if (running == 1) {
            char r = '\0';
            printf("\n\nPROGRAM FINISHED.\nDo you want to run the same program again? (y/n)");
            scanf("%c", &r);
            if (r == 'y' || r == 'Y') {
                running = 0, pc = 0, cicles = 0;
            } else {
                return 0;
            }
        }
        getch();
        system("cls");
        
    } while (running == 0);
    return 0;
}

void fetch() { 
    mar = pc; // atualiza o mar para a instrucao atual
    mbr = memory[mar];
    ir = mbr >> 3; // extrai o opcode

    // para instrucoes de 1 byte
    if (ir == 0 || ir == 1 || ir == 13) { 
        return; 
    }

    // para instrucoes de 2 bytes
    mbr = (mbr << 8) | memory[++mar];
    if (ir >= 2 && ir <= 12) { 
        return; 
    }

    // para instrucoes de 3 bytes
    mbr = (mbr << 8) | memory[++mar];

}

void decode() {
    // as instrucoes sao agrupadas de acordo com a quantidade de bytes que possuem
    // e os registradores que alteram; como instrucoes similares estao agrupadas de
    // maneira continua, fica ainda mais facil

    // a extracao de bytes eh realizada atraves do deslocamento de bytes a esquerda
    // ou a direita ou por meio do uso de "mascaras" com operacoes bitwise or/and

    // a instrucao hlt nao participa, pois ela necessariamente nao deve alterar o
    // conteudo de nenhum registrador
   if (ir == 1) {
        pc++;
   } else if (ir >= 2 && ir <= 12) {
        ro0 = (mbr & 0x600) >> 9;
        ro1 = (mbr & 0x180) >> 7;
   } else if (ir == 13) {
        ro0 = (mbr & 0x6);
   } else if (ir >= 14 && ir <= 20) {
        mar = (mbr & 0xffff);
   } else if (ir == 21 || ir == 22) {
        ro0 = (mbr & 0x60000) >> 17;
        mar = (mbr & 0xffff);
   } else {
        ro0 = (mbr & 0x60000) >> 17;
        imm = (mbr & 0xffff);
   }
}

void execute() {
    // executa as instrucoes a partir dos registradores modificados na etapa de decodificacao
    // a instrucao nop nao esta abaixo, pois sua unica funcao e incrementar o registrador pc

    if (ir == 0) { // hlt
        running = 1;
    } else if (ir == 2) { // ldr
        reg[ro0] = reg[ro1];
        pc += 2;
    } else if (ir == 3) { // str
        reg[ro1] = reg[ro0];
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
        e = (reg[ro0] = reg[ro1]) ? 1 : 0;
        l = (reg[ro0] < reg[ro1]) ? 1 : 0;
        g = (reg[ro0] > reg[ro1]) ? 1 : 0;
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
            return;
        }
        pc +=3;
    } else if (ir == 15) { // jump if not equal to
        if (e == 0) {
            pc = mar;
            return;
        }
        pc +=3;
    } else if (ir == 16) { // jump if lower than
        if (l == 1) {
            pc = mar;
            return;
        }
        pc +=3;
    } else if (ir == 17) { // jump if lower than or equal to
        if (e == 1 || l == 1) {
            pc = mar;
            return;
        }
        pc +=3;
    } else if (ir == 18) { // jump if greater than
        if (g == 1) {
            pc = mar;
            return;
        }
        pc +=3;
    } else if (ir == 19) { // jump if greater than or equal to
        if (e == 1 || g == 1) {
            pc = mar;
            return;
        }
        pc +=3;
    } else if (ir == 20) { // jump
        pc = mar;
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
        mbr = imm;
        reg[ro0] = mbr;
        pc += 3;
    } else if (ir == 24) { // add immediate
        mbr = imm;
        reg[ro0] += mbr;
        pc += 3;
    } else if (ir == 25) { // subtract immediate
        mbr = imm;
        reg[ro0] -= mbr;
        pc += 3;
    } else if (ir == 26) { // multiply immediate
        mbr = imm;
        reg[ro0] *= mbr;
        pc += 3;
    } else if (ir == 27) { // divide immediate
        mbr = imm;
        reg[ro0] /= mbr;
        pc += 3;
    } else if (ir == 28) { // left shift
        mbr = imm;
        reg[ro0] <<= mbr;
        pc += 3;
    } else if (ir == 29) { // right shift
        mbr = imm;
        reg[ro0] >>= mbr;
        pc += 3;
    }
}

// <> possui a funcao de copiar o codigo fonte para o array programLines,
// a fim de serem impressas a cada ciclo para melhor vizualizacao
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

// <> possui a funcao de extrair o indice da instrucao da linha qual
// o registrador pc esta apontando
int getInstruction() {
    for (int i = 0; i < SIZE; i++) {
        char tempLine[30];
        strcpy(tempLine, programLines[i]);

        char *line = strtok(tempLine, ";");

        if (line != NULL && pc == (int)strtol(line, NULL, 16)) {
            return i;
        }
    }
}

// <> imprime o estado da cpu, com todos os seus registradores e a memoria RAM
void displayCPUStatus() {
    char *screen[] = {
    "|_______________________________________________________________________________________________________________|",
    "|_________________________________________________CPU SIMULATOR_________________________________________________|",
    "|___________________________________________________REGISTERS___________________________________________________|",
    "|__________________________________________________MAIN MEMORY__________________________________________________|",
    "|                   Press any key to start the next machine cycle or press CTRL + C to exit.                    |"
    };
    printf("%s\n", screen[1]);

    printf("| Running line: %*s|\n", -96, programLines[currentLine]);

    
    printf("%s\n", screen[2]);

    printf("|%*s R0:%*X | R1:%*X | R2:%*X | R3:%*X |%*s\n", 36, "|", 4, reg[0], 4, reg[1], 4, reg[2], 4, reg[3], 36, "|"); 

    printf("|%*s MBR:%*X | MAR:%*X | IMM:%*X | PC:%*X |%*s\n", 28, "|", 7, mbr, 7, mar, 7, imm, 7, pc, 29, "|");
    printf("|%*s IR:%*X | RO0:%*X | RO1:%*X |%*s\n", 40, "|", 4, ir, 4, ro0, 4, ro1, 40, "|");
    printf("|%*s E:%*X | L:%*X | G:%*X |%*s\n", 41, "|", 5, e, 5, l, 5, g, 41, "|");

    printf("%s\n", screen[3]);
    int qtd = 11;
    for (int i = 0; i < SIZE; i++) {
        printf("|%*X:%*X ", 3, i, 4, memory[i]);
        if (i == qtd - 1) {
            printf("  |\n");
            qtd += 11;
        }
    }
    printf("%s\n%s\n%s\n", screen[0], screen[4], screen[0]);
}