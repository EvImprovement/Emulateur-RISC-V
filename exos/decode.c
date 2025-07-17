#include <stdio.h>
#include <stdint.h>

// TP 2/3 de IN330 -- exercices préparatoires
// LG et SM pour IN330, 2024

// Rappel de quelques fonctions d'encodage... (attention à l'ordre des
// arguments, fiez-vous aux noms plutôt qu'à l'ordre)

uint32_t encode_B_type(int opcode, int funct3, int rs1, int rs2, int imm)
{
    int imm_4_1  = (imm & 0b0000000011110) >> 1;
    int imm_10_5 = (imm & 0b0011111100000) >> 5;
    int imm_11   = (imm & 0b0100000000000) >> 11;
    int imm_12   = (imm & 0b1000000000000) >> 12;
    return opcode | (imm_11 << 7) | (imm_4_1 << 8) | (funct3 << 12)
           | (rs1 << 15) | (rs2 << 20) | (imm_10_5 << 25)
           | (imm_12 << 31);
}

uint32_t encode_J_type(int opcode, int rd, int imm)
{
    int imm_10_1  = (imm & 0b000000000011111111110) >> 1;
    int imm_11    = (imm & 0b000000000100000000000) >> 11;
    int imm_19_12 = (imm & 0b011111111000000000000) >> 12;
    int imm_20    = (imm & 0b100000000000000000000) >> 20;
    return opcode | (rd << 7) | (imm_19_12 << 12) | (imm_11 << 20)
           | (imm_10_1 << 21) | (imm_20 << 31);
}

uint32_t encode_I_type(int opcode, int funct3, int rd, int rs1, int imm)
{
    return opcode | (rd << 7) | (funct3 << 12) | (rs1 << 15)
           | ((imm & 0xfff) << 20);
}

uint32_t encode_R_type(
    int opcode, int funct3, int funct7, int rd, int rs1, int rs2)
{
    return opcode | (rd << 7) | (funct3 << 12) | (rs1 << 15) | (rs2 << 20)
           | (funct7 << 25);
}

uint32_t encode_S_type(int opcode, int funct3, int rs1, int rs2, int imm)
{
    int imm_4_0  = (imm & 0b000000011111);
    int imm_11_5 = (imm & 0b111111100000) >> 5;
    return opcode | (imm_4_0 << 7) | (funct3 << 12) | (rs1 << 15) | (rs2 << 20)
           | (imm_11_5 << 25);
}

// Fonctions de décodage : doivent faire l'inverse de encode_*_type()

void B_type(uint32_t insn, int *rs1, int *rs2, int *imm)
{
    *rs1 = (insn >> 15) & 0x1f;
    *rs2 = (insn >> 20) & 0x1f;
    *imm = (((int32_t)insn >> 31) << 12)//récupère 11
           | (((insn >> 7) & 1) << 11)//récupère 12
           | (((insn >> 25) & 0x3f) << 5)//récupère de 10 à 5
           | (((insn >> 8) & 0xf) << 1);//récupère de 4 à 1
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

// TODO: Faire de même pour R_type(), I_type() et S_type()
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
    *imm = (((int32_t)insn >> 25) << 5)//récupère de 11 à 5
           | (((insn >> 7) & 0x1f));//récupère de 4 à 0
    printf(":: S type (rs1=%d rs2=%d imm=%d)\n", *rs1, *rs2, *imm);
}
int main(void)
{
    int rs1, rs2, rd, imm;

    // On ignore opcode/funct3/funct7 durant le test car on les testera avant
    // pour décider quelle fonction *_type() appeler.

    B_type(encode_B_type(0, 0, 12, 17, 48), &rs1, &rs2, &imm);
    // Doit produire rs1=12, rs2=17, imm=48

    B_type(encode_B_type(0, 0, 5, 22, -24), &rs1, &rs2, &imm);
    // Doit produire rs1=5, rs2=22, imm=-24

    J_type(encode_J_type(0, 31, 4), &rd, &imm);
    // Doit produire rd=31, imm=4

    J_type(encode_J_type(0, 1, -1024), &rd, &imm);
    // Doit produire rd=1, imm=-1024

    // TODO: Faire de même pour R_type(), I_type() et S_type()
    R_type(encode_R_type(0,0,0,25,30,2),&rs1, &rs2, &rd);

    I_type(encode_I_type(0,0,14,12,-253),&rs1, &rd, &imm);

    S_type(encode_S_type(0,0,22,11,-783),&rs1, &rs2, &imm);
}   
