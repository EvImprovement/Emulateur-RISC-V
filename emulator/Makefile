# INDIQUEZ ICI : Votre nom et celui de votre binôme (sans espaces)
AUTEURS := GrabeEvan_EymeryLéa

SRC := $(wildcard *.c)
HDR := $(wildcard *.h)

riscv-emulator: $(SRC) $(HDR)
	gcc $(SRC) -o $@ -Wall -Wextra -O0 -g

test: test.py riscv-emulator
	@ python3  $<

blob:
	./riscv-emulator blob.hex blob.state >/dev/null

clean:
	@ rm -f riscv-emulator
	@ rm -rf __pycache__ .pytest_cache .hypothesis ._* *~
	@ find tests \( -name '*.o' -o -name '*.bin' -o -name '*.hex' -o -name '*.state' \) -delete
	@ find ../exos -type f -executable -delete

tar: clean
	@ rm -f ../*.tgz
	@ (cd ..; tar -cvzf "$(AUTEURS).tgz" *)
	@ echo "==="; echo "Created: ../$(AUTEURS).tgz"

.PHONY: test clean tar
