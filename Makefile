# By Ember2819
# C compiler
CC = clang
CXX = clang++
# Assembler (for boot.s)
AS = nasm
# Linker
LD = ld
# Objcopy (to translate elf to bin)
OBJCOPY = objcopy
OBJCOPY_ARGS = -O binary
include_folder = include
CC_FLAGS = -target i386-linux-gnu -march=i686 -m32 -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -g -c $(addprefix -I,$(include_folder))
LD_FLAGS = -m elf_i386
ISO = geckoos.iso
LINKERF = linker.ld

# nfoxers
SOURCES := $(shell find ./ -name "*.c" -o -name "*.s")
# change all the .c's
OBJECTS := $(patsubst ./kernel/%.c,./build/%.o, $(SOURCES))
# then all the .s's, name change to avoid conflict with .c sources w the same name
OBJECTS := $(patsubst %.s,./build/%_s.o, $(OBJECTS))

.DEFAULT_GOAL: $(ISO)

# If no clang detected, use gcc
build/%.o: kernel/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) $< -o $@
build/%_s.o: ./%.s
	@mkdir -p $(dir $@)
	$(AS) -felf32 $< -o $@

# Link all kernel objects
kernel.elf: $(OBJECTS)
	$(LD) $(LD_FLAGS) -T $(LINKERF) $^ -o $@
kernel.bin: kernel.elf
	$(OBJCOPY) $(OBJCOPY_ARGS) $^ $@
$(ISO): kernel.elf
	mv $^ iso/boot
	grub-mkrescue -o $@ iso

# Launch the image in QEMU
run: $(ISO)
	qemu-system-i386 -cdrom $^ -monitor stdio -m 1G
run-debug: $(ISO)
	qemu-system-i386 -cdrom $^ -d int,cpu,guest_errors -no-reboot -no-shutdown -m 40M

fat16.img:
	dd if=/dev/zero of=fat16.img bs=1M count=16
	mkfs.fat -F 16 -n "GECKOOS" fat16.img
	@echo "fat16.img created. Copy files onto it with:"
	@echo "  mcopy -i fat16.img yourfile.txt ::yourfile.txt"

run-fat16: $(ISO) fat16.img
	qemu-system-i386 -s \
	  -cdrom $(ISO) \
	  -drive format=raw,file=fat16.img \

clean:
	rm -f $(OBJECTS) boot.iso
	rm -f fat16.img $(ISO)
.PHONY: all run run-fat16 clean
