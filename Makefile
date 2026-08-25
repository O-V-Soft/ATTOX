AS          := nasm
CC          := gcc
LD          := ld
OBJCOPY     := objcopy
RM          := rm -f

ASFLAGS_BIN := -f bin
ASFLAGS_ELF := -f elf32
CFLAGS      := -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore \
               -ffunction-sections -mgeneral-regs-only -mno-red-zone -I./include -c \
               -fno-pic -fno-asynchronous-unwind-tables -fno-strict-aliasing -fpack-struct \
               -mno-sse
LDFLAGS     := -m elf_i386 -T linker.ld --nostdlib --static

OBJ         := kernel.o video.o idt.o io.o  fault.o isr.o \
			   keyboard.o pmm.o vmm.o paging.o panic.o \
               kheap.o fs.o string.o task.o sched.o \
               system_call.o vfs.o sh.o tss.o main.o

vpath %.c kernel drv/video drv/keyboard mm fs lib \
          usr init

vpath %.asm boot kernel mm 

.PHONY: all clean run push

all: os-image.img

os-image.img: boot.bin kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=boot.bin of=$@ conv=notrunc bs=512 count=1
	dd if=kernel.bin of=$@ conv=notrunc bs=512 seek=1

boot.bin: boot/boot.asm
	$(AS) $(ASFLAGS_BIN) $< -o $@

kernel.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o kernel.elf $(OBJ)
	$(OBJCOPY) -O binary kernel.elf $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS_ELF) $< -o $@

clean:
	$(RM) *.o *.bin *.elf 

cleane: 
	$(RM) *.o *.bin *.elf *.img

run: os-image.img
	qemu-system-i386 -drive file=os-image.img,format=raw 

push:
	git add .
	git commit -m "ATTOX"
	git push origin main --force

dd:
	-sudo umount /dev/mmcblk0*
	sudo dd if=os-image.img of=/dev/mmcblk0 status=progress conv=fsync

re:
	make cleane
	make

rer:
	make cleane
	make run
