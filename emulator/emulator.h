#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>


/* Émule le code hexadécimal de `fp_in` et produit le résultat final dans
  `fp_out` */
void emulate(FILE *fp_in, FILE *fp_out);

#define MACHINE_MEMSIZE 16384

/* Donnée pour la machine en cours d'exécution */
struct machine {
    uint32_t PC;
    int64_t regs[32];
    int64_t memory[MACHINE_MEMSIZE / 8];
};

/* Initialise l'état de la machine (mémoire vide, pointeur de pile...). */
void machine_init(struct machine *mach);

/* Lit une valeur non signée de 32 bits dans la mémoire à l'adresse `addr`. */
uint32_t machine_luw(struct machine *mach, uint32_t addr);

/* Écrit une valeur non signée de 32 bits à l'adresse `addr`. */
void machine_suw(struct machine *mach, uint32_t addr, uint32_t value);

/* Lit une valeur signée de 64 bits à l'adresse `addr`. */
int64_t machine_ld(struct machine *mach, uint32_t addr);

/* Écrit une valeur signée de 64 bits à l'adresse `addr`. */
void machine_sd(struct machine *mach, uint32_t addr, int64_t value);
