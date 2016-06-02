OBJS := start.o mem.o main.o dev/dev.o lib/lib.o

CFLAGS = -fno-builtin -I$(shell pwd)/include
export CFLAGS

gboot.bin : $(OBJS)
	arm-linux-ld -Tgboot.lds -o gboot.elf $^
	arm-linux-objcopy -O binary -S gboot.elf $@
	
%.o : %.S
	arm-linux-gcc -g -c $<

%.o : %.c
	arm-linux-gcc -g $(CFLAGS) -c $<

.PHONY : lib/lib.o
lib/lib.o:
	cd lib; make; cd ..

.PHONY : dev/dev.o
dev/dev.o:
	cd dev; make; cd ..

clean:
	rm -f *.o *.elf *.bin
	make clean -C lib
	make clean -C dev