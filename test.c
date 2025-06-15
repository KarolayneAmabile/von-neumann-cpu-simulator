#include <stdio.h>
#include <stdlib.h>

void read(char *filename);

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
    int line = 0;

    while (fgets(str, sizeof(str), program) != NULL) {
        printf("Line: %d: %s", line, str);
        line++;
    }
    fclose(program);
}
