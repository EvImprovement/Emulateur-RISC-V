addi t0, zero, 0
addi t1, zero, 3
bge t0,t1,12
addi t0,t0,1
jal ra, -8
addi t2, zero, 9


# EXPECTED
# sp: 16384
# t0: 3
# t1: 3
# t2: 9
# ra: 20
