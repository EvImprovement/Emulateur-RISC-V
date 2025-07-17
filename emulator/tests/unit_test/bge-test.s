addi t0, zero, 20
addi t1, zero, 10
bge t0, t1, 8
addi t2, zero, 5
addi t3, zero, 50

# EXPECTED
# sp: 16384
# t0: 20
# t1: 10
# t2: 0
# t3: 50
