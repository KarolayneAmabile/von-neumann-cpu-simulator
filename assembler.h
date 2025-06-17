#ifndef ASSEMBLER_H
#define ASSEMBLER_H

void loadProgram(char *filename, unsigned char *memory);
int getOpcode(char *mnemonic);

#endif 