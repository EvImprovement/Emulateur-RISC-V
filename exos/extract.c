#include <stdio.h>
#include <stdint.h>

// TP 2/3 de IN330 -- exercices préparatoires
// LG et SM pour IN330, 2024

uint32_t extract_field(uint32_t w, int p, int l)
{
	// TODO : Extraire de w la série de bits à la position p de longueur l
	w=w>>p;
	uint32_t b=(1<<l)-1;
	w=w&b;
	return w;

}

// Extension de signe d'un entier de n-bits (n > 1), i, vers 32 bits
int32_t sign_extend(uint32_t i, int n){
	i=i<<(32-n);
	return ((int32_t)i)>>(32-n);
}

int main(void)
{
	uint32_t w = 0x00b60633;
	printf("w[6..0] = %u\n", extract_field(w, 0, 7));
	printf("w[19..15] = %u\n", extract_field(w, 15, 5));
	printf("%08x %08x \n",sign_extend(-15,12),extract_field(-15,0,12));


}
