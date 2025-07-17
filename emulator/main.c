#include "emulator.h"

int main(int argc, char **argv)
{
    /* ./riscv-emulator <HEX INPUT> <EMULATION OUTPUT> */
    if(argc != 3) {
        printf("Usage: ./riscv-emulator <HEX INPUT> <EMULATION OUTPUT>\n");
        printf("error: invalid command\n");
        return 1;
    }

    FILE *fp_in = fopen(argv[1], "r");
    if(fp_in == NULL) {
        perror("error: cannot read input file");
        return 1;
    }

    FILE *fp_out = fopen(argv[2], "w");
    if(fp_out == NULL) {
        perror("error: cannot open output file");
        return 1;
    }

    emulate(fp_in, fp_out);
    fclose(fp_in);
    fclose(fp_out);
    return 0;
}
