addi t0, zero, 0
addi t1, zero, 3
bge t0, t1, (.+ 12)
addi t0,t0,1
jal ra, (.+  -8)
addi t2, zero, 9
