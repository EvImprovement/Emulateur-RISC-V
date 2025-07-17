addi t0, zero, 20
sd t0, 100(zero)
ld t1, 100(zero)

# EXPECTED
# sp: 16384
# t0: 20
# t1: 20

