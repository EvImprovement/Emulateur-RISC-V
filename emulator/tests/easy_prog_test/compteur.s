addi a1, zero, 0       # a1 = 0 (compteur)
addi a2, zero, 50      # a2 = 50 (limite)
addi a1, a1, 1     # a1 = a1 + 1
bne a1, a2, -4

# EXPECTED
# sp: 16384
# a1: 50
# a2: 50
