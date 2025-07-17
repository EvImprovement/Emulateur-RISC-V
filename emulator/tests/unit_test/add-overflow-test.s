addi a1, zero, 2047  # Valeur maximale pour 32 bits signés
addi a2, zero, 1
add a0, a1, a2

# EXPECTED
# sp: 16384
# a0: 2048
# a1: 2047
# a2: 1
