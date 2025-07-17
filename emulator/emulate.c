#include "emulator.h"
#include <unistd.h>
#include <time.h>


void execute_instruction(struct machine *mach, uint32_t insn);

void emulate(FILE *fp_in, FILE *fp_out)
{
    /* Création et initialisation de la machine émulée */
    struct machine *mach = malloc(sizeof *mach);
    machine_init(mach);

    /* Chargement des instructions assemblées dans la mémoire */
    int insn_count = 0;
    uint32_t insn;
    while(fscanf(fp_in, "%x", &insn) > 0) {
        machine_suw(mach, 4 * insn_count, insn);
        insn_count++;
    }


     /************************************* Boucle de lecture - décodage - exécution des instructions ********************************************/
    while (1) {
    uint32_t instruction = machine_luw(mach, mach->PC); // Lis l'instruction

    if (instruction == 0) { // Vérifier si l'instruction vaut 0
        fprintf(stderr, "Fin de l'exécution : instruction 0 détectée.\n");
        break; // Sortir de la boucle
    } 
    execute_instruction(mach,instruction); 
    mach->regs[0] = 0;  // pour s'assurer que on ne modifie pas x0=0, 
}
    /* Affiche de l'état final dans fp_out */
    for(int i=0;i<32;i++){
        fprintf(fp_out,"x%d: %ld\n",i,mach->regs[i]);
    }
    free(mach);
}
/*********************************************************************** Codage des types d'instruction **********************************************************/
void B_type(uint32_t insn, int *rs1, int *rs2, int *imm)
{
    *rs1 = (insn >> 15) & 0x1f;
    *rs2 = (insn >> 20) & 0x1f;
    *imm = (((int32_t)insn >> 31) << 12)
           | (((insn >> 7) & 1) << 11)
           | (((insn >> 25) & 0x3f) << 5)
           | (((insn >> 8) & 0xf) << 1);
    printf(":: B type (rs1=%d rs2=%d imm=%d)\n", *rs1, *rs2, *imm);
}

void J_type(uint32_t insn, int *rd, int *imm)
{
    *rd = (insn >> 7) & 0x1f;
    *imm = (((int32_t)insn >> 31) << 20)
           | (((insn >> 12) & 0xff) << 12)
           | (((insn >> 20) & 1) << 11)
           | (((insn >> 21) & 0x3ff) << 1);
    printf(":: J type (rd=%d imm=%d)\n", *rd, *imm);
}

void R_type(uint32_t insn, int *rs1, int *rs2, int *rd)
{
    *rs1 = (insn >> 15) & 0x1f;
    *rs2 = (insn >> 20) & 0x1f;
    *rd = (insn >> 7) & 0x1f;
    printf(":: R type (rs1=%d rs2=%d rd=%d)\n", *rs1, *rs2, *rd);
}
void I_type(uint32_t insn, int *rs1, int *rd, int *imm)
{
    *rs1 = (insn >> 15) & 0x1f;
    *rd = (insn >> 7) & 0x1f;
    *imm = (((int32_t)insn >> 20));

    printf(":: I type (rs1=%d rd=%d imm=%d)\n", *rs1, *rd, *imm);
}
void S_type(uint32_t insn, int *rs1, int *rs2, int *imm)
{
    *rs1 = (insn >> 15) & 0x1f;
    *rs2 = (insn >> 20) & 0x1f;
    *imm = (((int32_t)insn >> 25) << 5)
           | (((insn >> 7) & 0x1f));
    printf(":: S type (rs1=%d rs2=%d imm=%d)\n", *rs1, *rs2, *imm);
}

/*************************************************************************Fonction do_x pour l"émulateur ***************************************************************/
void do_addi(struct machine *mach, uint32_t instruction) {
    int rs1, rd, imm;
    I_type(instruction, &rs1, &rd, &imm);

    printf("ADDI: rs1=x%d (%ld), imm=%d -> rd=x%d\n", rs1, mach->regs[rs1], imm, rd);
    mach->regs[rd] = mach->regs[rs1] + imm;
    mach->PC += 4;
    printf("Résultat: rd=x%d = %ld, PC=%d\n", rd, mach->regs[rd], mach->PC);
}

void do_add(struct machine *mach, uint32_t instruction) {
    int rs1, rs2, rd;
    R_type(instruction, &rs1, &rs2, &rd);

    printf("ADD: rs1=x%d (%ld), rs2=x%d (%ld) -> rd=x%d\n", rs1, mach->regs[rs1], rs2, mach->regs[rs2], rd);
    mach->regs[rd] = mach->regs[rs1] + mach->regs[rs2];
    mach->PC += 4;
    printf("Résultat: rd=x%d = %ld, PC=%d\n", rd, mach->regs[rd], mach->PC);
}

void do_sub(struct machine *mach, uint32_t instruction) {
    int rs1, rs2, rd;
    R_type(instruction, &rs1, &rs2, &rd);

    printf("SUB: rs1=x%d (%ld), rs2=x%d (%ld) -> rd=x%d\n", rs1, mach->regs[rs1], rs2, mach->regs[rs2], rd);
    mach->regs[rd] = mach->regs[rs1] - mach->regs[rs2];
    mach->PC += 4;
    printf("Résultat: rd=x%d = %ld, PC=%d\n", rd, mach->regs[rd], mach->PC);
}

void do_ld(struct machine *mach, uint32_t instruction) {
    int rs1, rd, imm;
    I_type(instruction, &rs1, &rd, &imm);

    // Calcul de l'adresse mémoire avec alignement
    uint32_t addr = mach->regs[rs1] + imm;
    uint32_t adrr_alignée = (addr / 8) * 8;  // Dans le cas où addr%8 !=0, on s'aligne sur le multiple de 8 le plus proche
    printf("LD: rs1=x%d (%ld), imm=%d -> rd=x%d, addr=%u, adrr_alignée=%u\n",rs1, mach->regs[rs1], imm, rd, addr, adrr_alignée);

    // Chargement depuis la mémoire
    mach->regs[rd] = machine_ld(mach, adrr_alignée);
    mach->PC += 4;
    printf("Résultat: rd=x%d = %ld, PC=%d\n", rd, mach->regs[rd], mach->PC);
}


void do_sd(struct machine *mach, uint32_t instruction) {
    int rs1, rs2, imm;
    S_type(instruction, &rs1, &rs2, &imm);

    // Calcul de l'adresse mémoire avec alignement
    uint32_t addr = mach->regs[rs1] + imm;
    uint32_t adrr_alignée = (addr / 8) * 8;  // Même chose que pour ld

    printf("SD: rs1=x%d (%ld), rs2=x%d (%ld), imm=%d, addr=%u, adrr_alignée=%u\n",
           rs1, mach->regs[rs1], rs2, mach->regs[rs2], imm, addr, adrr_alignée);

    // Stockage dans la mémoire
    machine_sd(mach, adrr_alignée, mach->regs[rs2]);
    mach->PC += 4;
    printf("Memoire écrite à l'adresse : aligned_addr=%u, valeur=%ld, PC=%d\n", adrr_alignée, mach->regs[rs2], mach->PC);
}

void do_beq(struct machine *mach, uint32_t instruction) {
    int rs1, rs2, imm;
    B_type(instruction, &rs1, &rs2, &imm);

    printf("BEQ: rs1=x%d (%ld), rs2=x%d (%ld), imm=%d\n", rs1, mach->regs[rs1], rs2, mach->regs[rs2], imm);
    if (mach->regs[rs1] == mach->regs[rs2]) {
        mach->PC += imm;
        printf("Branche prise: PC=%d\n", mach->PC);
    } else {
        mach->PC += 4;
        printf("Branche ignorée: PC=%d\n", mach->PC);
    }
}

void do_blt(struct machine *mach, uint32_t instruction) {
    int rs1, rs2, imm;
    B_type(instruction, &rs1, &rs2, &imm);

    printf("BLT: rs1=x%d (%ld), rs2=x%d (%ld), imm=%d\n", rs1, mach->regs[rs1], rs2, mach->regs[rs2], imm);
    if (mach->regs[rs1] < mach->regs[rs2]) {
        mach->PC += imm;
        printf("Branche prise: PC=%d\n", mach->PC);
    } else {
        mach->PC += 4;
        printf("Branche ignorée: PC=%d\n", mach->PC);
    }
}

void do_bne(struct machine *mach, uint32_t instruction) {
    int rs1, rs2, imm;
    B_type(instruction, &rs1, &rs2, &imm);

    printf("BNE: rs1=x%d (%ld), rs2=x%d (%ld), imm=%d\n", rs1, mach->regs[rs1], rs2, mach->regs[rs2], imm);
    if (mach->regs[rs1] != mach->regs[rs2]) {
        mach->PC += imm;
        printf("Branche prise: PC=%d\n", mach->PC);
    } else {
        mach->PC += 4;
        printf("Branche ignorée: PC=%d\n", mach->PC);
    }
}

void do_bge(struct machine *mach, uint32_t instruction) {
    int rs1, rs2, imm;
    B_type(instruction, &rs1, &rs2, &imm);

    printf("BGE: rs1=x%d (%ld), rs2=x%d (%ld), imm=%d\n", rs1, mach->regs[rs1], rs2, mach->regs[rs2], imm);
    if (mach->regs[rs1] >= mach->regs[rs2]) {
        mach->PC += imm;
        printf("Branch prise: PC=%d\n", mach->PC);
    } else {
        mach->PC += 4;
        printf("Branche ignorée: PC=%d\n", mach->PC);
    }
}

void do_jal(struct machine *mach, uint32_t instruction) {
    int rd, imm;
    J_type(instruction, &rd, &imm);

    printf("JAL: rd=x%d, imm=%d\n", rd, imm);
    if (rd != 0) {
        mach->regs[rd] = mach->PC + 4;
        printf("Adresse de la prochaine instruction mise dans : rd=x%d = %ld\n", rd, mach->regs[rd]);
    }
    mach->PC += imm;
    printf("Saut à PC=%d\n", mach->PC);
}

/*************************************************************** Partie 5 Blob **************************************************************************/
void U_type(uint32_t instruction, int *rd, int *imm) {
    *rd = (instruction >> 7) & 0x1F; 
    *imm = (instruction >> 12) & 0xFFFFF; 
}

void do_auipc(struct machine *mach, uint32_t instruction) {
    int rd, imm;
    U_type(instruction, &rd, &imm);

    printf("AUIPC: rd=x%d, imm=%d\n", rd, imm);
    mach->regs[rd] = mach->PC + (imm << 12); 
    mach->PC += 4;
    printf("Résultat: rd=x%d = %ld, PC=%d\n", rd, mach->regs[rd], mach->PC);
}

void do_srli(struct machine *mach, uint32_t instruction) {
    int rs1, rd, imm;
    I_type(instruction, &rs1, &rd, &imm); 

    mach->regs[rd] = mach->regs[rs1] >> (imm & 0x1F);  
    mach->PC += 4;
    printf("SRLI: rs1=x%d (%ld), imm=%d -> rd=x%d\n", rs1, mach->regs[rs1], imm, rd);
    printf("Résultat: rd=x%d = %ld, PC=%d\n", rd, mach->regs[rd], mach->PC);
}

void do_lw(struct machine *mach, uint32_t instruction) {
    int rs1, rd, imm;
    I_type(instruction, &rs1, &rd, &imm);  
    uint32_t addr = mach->regs[rs1] + imm;
    uint32_t adrr_alignée = (addr * 8)/8;
    printf("LW: rs1=x%d (%ld), imm=%d -> rd=x%d, addr_alignée=%u\n",rs1, mach->regs[rs1], imm, rd, adrr_alignée);
    mach->regs[rd] = (int32_t)machine_luw(mach, adrr_alignée); 
    mach->PC += 4;
    printf("Résultat: rd=x%d = %ld, PC=%d\n", rd, mach->regs[rd], mach->PC);
}
void do_ecall(struct machine *mach) {
    int char_to_print = mach->regs[10];  // Le caractère à afficher dans a0 (x10)
    fprintf(stderr,"%c", (char)char_to_print);
    mach->PC += 4;
}
void do_and(struct machine *mach, uint32_t instruction) {
    int rs1, rs2, rd;
    R_type(instruction, &rs1, &rs2, &rd); 
    mach->regs[rd] = mach->regs[rs1] & mach->regs[rs2];
    mach->PC += 4;
    printf("AND: rs1=x%d (%ld), rs2=x%d (%ld) -> rd=x%d\n",rs1, mach->regs[rs1], rs2, mach->regs[rs2], rd);
    printf("Résultat: rd=x%d = %ld, PC=%d\n", rd, mach->regs[rd], mach->PC);
}
void do_jalr(struct machine *mach, uint32_t instruction) {
    int rs1, rd, imm;
    I_type(instruction, &rs1, &rd, &imm); 
    uint64_t adrr_saut = (mach->regs[rs1] + imm);  
    if (rd != 0) {
        mach->regs[rd] = mach->PC + 4;
    }
    mach->PC = adrr_saut;
    printf("JALR: rs1=x%d (%ld), imm=%d -> target=%ld, rd=x%d (link) = %ld, PC=%d\n", rs1, mach->regs[rs1], imm, adrr_saut, rd, mach->regs[rd], mach->PC);
}
/********************************************************************* Déchiffrement et éxécution des instructions ************************************/
void execute_instruction(struct machine *mach, uint32_t instruction) {
    uint32_t opcode = instruction & 0x7F;         // Extraction de l'opcode (bits [6:0])
    uint32_t funct3 = (instruction >> 12) & 0x7; // Extraction de funct3 (bits [14:12])
    uint32_t funct7 = (instruction >> 25) & 0x7F; // Extraction de funct7 (bits [31:25])
    printf("Instruction : 0x%x, opcode : %x, funct3 : %03x, funct7 : %03x \n", instruction, opcode, funct3, funct7);

    switch (opcode) {
        case 0x33: // Instructions de type R
            if (funct3 == 0x0 && funct7 == 0x00) do_add(mach, instruction);
            else if (funct3 == 0x0 && funct7 == 0x20) do_sub(mach, instruction);
            else if (funct3 == 0x7 && funct7 == 0x0) do_and(mach,instruction);
            break;

        case 0x13: // Instructions de type I
            if (funct3 == 0x0) do_addi(mach, instruction);
            else if(funct3 ==0x5) do_srli(mach,instruction);
            break;

        case 0x03: // Instructions de type I 
            if (funct3 == 0x3) do_ld(mach, instruction);
            else if(funct3==0x2) do_lw(mach,instruction);
            break;

        case 0x23: // Instructions de type S 
            if (funct3 == 0x3) do_sd(mach, instruction);
            break;

        case 0x63: // Instructions de type B
            if (funct3 == 0x0) do_beq(mach, instruction);
            else if (funct3 == 0x1) do_bne(mach, instruction);
            else if (funct3 == 0x4) do_blt(mach, instruction);
            else if (funct3 == 0x5) do_bge(mach, instruction);
            break;

        case 0x6F: // Instruction jal 
            do_jal(mach, instruction);
            break;

        case 0x17: 
            do_auipc(mach, instruction);
            break;
        case 0x73:
        do_ecall(mach);
        break;
        case 0x67:
        do_jalr(mach,instruction);
        break;
        default:
            fprintf(stderr, "Instruction non supportée : 0x%x, opcode : %x, funct3 : %03x, funct7 : %03x \n", instruction, opcode, funct3, funct7);
            break;
    }
}

