CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)

CCFLAGS = -g -Wall -ffreestanding -nostdlib

CC = aarch64-elf-gcc
LD = aarch64-elf-ld
OBJCOPY = aarch64-elf-objcopy

#CC = aarch64-linux-gnu-gcc
#LD = aarch64-linux-gnu-ld
#OBJCOPY = aarch64-linux-gnu-objcopy

all: clean kernel8.img

boot.o: boot.S
	$(CC) $(CCFLAGS) -c boot.S -o boot.o

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

kernel8.img: boot.o $(OFILES)
	$(LD) -nostdlib boot.o $(OFILES) -T link.ld -o kernel8.elf
	$(OBJCOPY) -O binary kernel8.elf kernel8.img

clean:
	echo "OFILES=" $(OFILES)
	rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true

run:
	qemu-system-aarch64  -cpu cortex-a57 -machine virt \
	-serial stdio \
	-vga none -device ramfb \
	-net none -pidfile qemu.pid -semihosting \
	-kernel kernel8.elf

debug:
	qemu-system-aarch64 -s -S -cpu cortex-a57 -machine virt \
	-serial stdio \
	-vga none -device ramfb \
	-net none -pidfile qemu.pid -semihosting \
	-kernel kernel8.elf

	# -monitor stdio \
