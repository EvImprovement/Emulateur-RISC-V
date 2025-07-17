#include "emulator.h"

void machine_init(struct machine *mach)
{
    /* Initialisation de la mémoire et des registres */
    memset(mach, 0, sizeof *mach);
    mach->PC = 0;
    mach->regs[2]=MACHINE_MEMSIZE;
    // TODO: Initialiser mach->PC à l'adresse de chargement du programme
    // TODO: Initialiser mach->regs[2] (i.e. le pointeur de pile, sp) à sa
    // valeur initiale, la taille de la mémoire.
}

static void check_address(uint32_t addr, int align)
{
    if(addr >= MACHINE_MEMSIZE) {
        fprintf(stderr, "error: memory read out of bounds at %08x\n", addr);
        exit(1);
    }
    if((addr % align) != 0) {
        fprintf(stderr, "error: unaligned memory access at %08x\n", addr);
        exit(1);
    }
}

uint32_t machine_luw(struct machine *mach, uint32_t addr)
{
    check_address(addr, 4);

    int64_t v = mach->memory[addr / 8];
    if(addr & 4)
        return (v & 0xffffffff00000000l) >> 32;
    else
        return (v & 0x00000000ffffffffl);
}

void machine_suw(struct machine *mach, uint32_t addr, uint32_t value)
{
    check_address(addr, 4);

    int64_t v = mach->memory[addr / 8];
    if(addr & 4)
        v = (v & 0x00000000ffffffffl) | (int64_t)value << 32;
    else
        v = (v & 0xffffffff00000000l) | value;
    mach->memory[addr / 8] = v;
}

int64_t machine_ld(struct machine *mach, uint32_t addr)
{
    check_address(addr, 8);
    return mach->memory[addr / 8];
}

void machine_sd(struct machine *mach, uint32_t addr, int64_t value)
{

    check_address(addr, 8);
    mach->memory[addr / 8] = value;
}
