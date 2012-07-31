all:
	cd kernel && make all

run: all
	mount -o loop floppy /mnt/os
	cp kernel/kernel /mnt/os/boot
	umount /mnt/os
	qemu -fda floppy -k en-us

debug: all
	mount /mnt/os
	cp kernel/kernel /mnt/os/boot
	umount /mnt/os
	qemu -s -S -fda floppy -k en-us

dep:
	cd kernel && make dep

clean:
	cd kernel && make clean



