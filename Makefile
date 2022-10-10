CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)

QEMU_CFILES = $(wildcard qemu/*.c)
QEMU_OFILES = $(QEMU_CFILE:%.c=%.o)

CCFLAGS = -g -Wall -ffreestanding -nostdlib

CC = aarch64-elf-gcc
LD = aarch64-elf-ld
OBJCOPY = aarch64-elf-objcopy

#CC = aarch64-linux-gnu-gcc
#LD = aarch64-linux-gnu-ld
#OBJCOPY = aarch64-linux-gnu-objcopy

all: 	clean kernel8.img

boot.o: boot.S
	$(CC) $(CCFLAGS) -c boot.S -o boot.o

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

kernel8.img: boot.o $(OFILES) $(QEMU_OFILES)
	$(LD) -nostdlib boot.o $(OFILES) $(QEMU_OFILES) -T link.ld -o kernel8.elf
	$(OBJCOPY) -O binary kernel8.elf kernel8.img

clean:
	echo "OFILES	= " $(OFILES)
	echo "QEMU	= " $(QEMU_CFILES)
	echo "QEMU	= " $(QEMU_OFILES)
	rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true


# qemu-system-arm -kernel ~/qemu_vms/<your-kernel-qemu> -cpu arm1176 -m 256 -M versatilepb -serial stdio -append "root=/dev/sda2 rootfstype=ext4 rw" -hda ~/qemu_vms/<your-jessie-image.img> -redir tcp:5022::22 -no-reboot
# https://stackoverflow.com/questions/67045438/emulating-raspberry-pi-4-with-qemu/71185370#71185370
run:
	qemu-system-aarch64 -M virt,highmem=off -smp 8 -m 2G -cpu cortex-a72 -kernel kernel8.elf \
	-serial stdio \
	-device VGA,id=vga1 -device secondary-vga,id=vga2 -object iothread,id=io1 

#	qemu-system-aarch64  -cpu cortex-a57 -machine virt \
#	-serial stdio \#
#	-vga none -device ramfb \
#	-net none -pidfile qemu.pid -semihosting \
#	-kernel kernel8.elf

debug:
	qemu-system-aarch64 -s -S -cpu cortex-a57 -machine virt \
	-serial stdio \
	-vga none -device ramfb \
	-net none -pidfile qemu.pid -semihosting \
	-kernel kernel8.elf

	# -monitor stdio \
