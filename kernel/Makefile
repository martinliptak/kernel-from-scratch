all: kernel

CFLAGS = -g -Iinclude
ASMFLAGS = -g
include Dependencies

dep:
	echo -n > Dependencies
	for f in *.c lib/*.c; do gcc -Iinclude -MM $$f -MQ "$${f%.c}.o">> Dependencies; done
	echo -n "OBJS = " >> Dependencies
	for f in *.asm; do echo -n "$${f%.asm}.o " >> Dependencies; done
	for f in *.c lib/*.c; do echo -n "$${f%.c}.o " >> Dependencies; done
	echo >> Dependencies

clean:
	-rm *.o
	-rm lib/*.o
	-rm kernel

kernel: $(OBJS) linker.ld
	ld $(OBJS) -o kernel -Tlinker.ld

.SUFFIXES: .asm
.asm.o:
	nasm $(ASMFLAGS) -f elf $< -o $@

