CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib 
AARCH64-CC = aarch64-elf-gcc
AARCH64-LD = aarch64-elf-ld


all: clean kernel8.img 

boot.o: boot.S
	$(AARCH64-CC) $(GCCFLAGS) -c boot.S -o boot.o

%.o: %.c
	$(AARCH64-CC) $(GCCFLAGS) -c $< -o $@

kernel8.img: boot.o $(OFILES)
	$(AARCH64-LD) -nostdlib boot.o $(OFILES) -T link.ld -o kernel8.elf
	aarch64-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true


run:
	qemu-system-aarch64 -machine virt -cpu cortex-a57 -vga none -net none -pidfile qemu.pid  -nographic -semihosting -kernel kernel8.elf

