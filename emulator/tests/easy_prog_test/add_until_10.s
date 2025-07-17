addi a4, zero, 0       # a4 = 0 (somme)
addi a5, zero, 1       # a5 = 1 (compteur)
addi a6, zero, 10      # a6 = 10 (limite de la somme)
add a4, a4, a5     # a4 = a4 + a5
addi a5, a5, 1     # a5 = a5 + 1
bne a5, a6, -8

# EXPECTED
# sp : 16384
# a4: 45
# a5: 10
# a6: 10