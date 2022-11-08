CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)

QEMU_CFILES = $(wildcard qemu/*.c)
QEMU_OFILES = $(QEMU_CFILE:%.c=%.o)

CFLAGS = -Wall -O2 -ffreestanding -nostdlib -mcpu=cortex-a53+nosimd

CC = clang
LD = ld.lld
OBJCOPY = llvm-objcopy

all: 	clean kernel8.img

boot.o: boot.S
	$(CC) --target=aarch64-elf -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) --target=aarch64-elf $(CFLAGS) -c $< -o $@


kernel8.img: boot.o $(OFILES) $(QEMU_OFILES)
	$(LD) -m aarch64elf -nostdlib boot.o $(OFILES) $(QEMU_OFILES) -T link.ld -o kernel8.elf
	$(OBJCOPY) -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true


# qemu-system-arm -kernel ~/qemu_vms/<your-kernel-qemu> -cpu arm1176 -m 256 -M versatilepb -serial stdio -append "root=/dev/sda2 rootfstype=ext4 rw" -hda ~/qemu_vms/<your-jessie-image.img> -redir tcp:5022::22 -no-reboot
# https://stackoverflow.com/questions/67045438/emulating-raspberry-pi-4-with-qemu/71185370#71185370

rpi:	kernel8.img
	qemu-system-aarch64 \
	-M raspi3b -cpu cortex-a72 \
	-kernel kernel8.img \
	-m 1G \
	-serial null -serial stdio  -d in_asm 

run:	kernel8.img
	qemu-system-aarch64 \
		-M virt,highmem=off,accel=hvf \
		-smp 8 -m 1G -cpu cortex-a72 -kernel kernel8.img \
		-pidfile qemu.pid \
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
