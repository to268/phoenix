# Base variables
export ARCH?=$(shell uname -m)
CONFIG_PATH=config
NCPUS=$(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)
QEMU_FLAGS=-enable-kvm -cpu host -m 512M -no-reboot -no-shutdown

# Output files
KERNEL_NAME=phoenix
KERNEL=$(KERNEL_NAME).elf
ISO=$(KERNEL_NAME)-$(ARCH).iso
MAP=$(KERNEL).map
ISO_DIR=isofiles

# Limine
LIMINE_CD=utils/limine/limine-cd.bin
LIMINE_CFG=utils/limine/limine.cfg
LIMINE_SYS=utils/limine/limine.sys
LIMINE_EFI=utils/limine/limine-cd-efi.bin
LIMINE_DEPLOY=utils/limine/limine-deploy

# Linker script
LD_SCRIPT=arch/$(ARCH)/stivale2.ld

ASM_FILES:=$(shell find -path ./utils -prune -false -o -name "*.asm")
ASM_OBJS:=$(ASM_FILES:.asm=.o)
C_FILES=$(shell find -path ./utils -prune -false -o -name "*.c")
OBJS=$(C_FILES:.c=.o)

# Suppress "Entering directory..."
MAKEFLAGS+=--no-print-directory
-include $(C_FILES:.c=.d)

all: config $(KERNEL)

config:
# Check if config file exists
ifeq ("$(wildcard $(CONFIG_PATH))", "")
	cp -f arch/$(ARCH)/default-config config
	echo "$(ARCH) default config copied, type make to start compiling now"
	exit -1
else

# Include config file
include $(CONFIG_PATH)

# Only x86_64 is supported at this time
ifneq ($(ARCH), x86_64)
$(error Unknow Architecture $(ARCH))
else
# Export default CFLAGS
export CFLAGS =	-mcmodel=kernel -mno-red-zone -mno-mmx \
				-mno-sse -mno-sse2 -std=gnu18 -Wall -Wextra \
				-O2 -ffreestanding -fno-common -fno-pic \
				-pipe -fno-stack-protector -fno-exceptions \
				-fno-non-call-exceptions -nostdlib -MMD \
				--sysroot=$(PWD) -isystem=/include \
				-mabi=sysv $(CONFIG_CFLAGS)
				#-fstack-protector-strong

# Export default LDFLAGS
export LDFLAGS = -n -nostdlib -static -z max-page-size=0x1000 \
				 $(CONFIG_LDFLAGS)

endif

# If the Cross Compiler is provided, set Make variables
ifndef CONFIG_CROSS_COMPILER_PREFIX
$(error The Cross Compiler Prefix is needed, Native Compiler is unsupported)
else
# Make variables
export CC=$(CONFIG_CROSS_COMPILER_PREFIX)gcc
export AR=$(CONFIG_CROSS_COMPILER_PREFIX)ar
export NM=$(CONFIG_CROSS_COMPILER_PREFIX)nm
export LD=$(CONFIG_CROSS_COMPILER_PREFIX)ld
# Only tested with nasm assembler
export ASM=nasm

export MKISOFS=xorriso -as mkisofs

endif

endif

$(KERNEL): $(ASM_OBJS) $(OBJS) lib/libk.a config
	echo "   LD        $(KERNEL)"
	$(LD) $(LDFLAGS) -T $(LD_SCRIPT) -o $(KERNEL) $(ASM_OBJS) $(OBJS)
	echo "   NM        $(MAP)"
	$(NM) -n $(KERNEL) > $(MAP)

# Process asm files
$(ASM_OBJS): $(ASM_FILES)
	echo "   ASM       $*.o"
	$(ASM) -f elf64 -i arch/$(ARCH)/asm/ $*.asm -o $@

# Process C Files
$(OBJS): $(C_FILES)
	echo "   CC        $*.o"
	$(CC) $(CFLAGS) -c $*.c -o $@

# Libs
lib/libk.a: $(wildcard lib/libk/*.c)
	echo "   AR        $@"
	$(AR) rcs $@ $(wildcard lib/libk/*.o)

toolchain:
	cd utils/toolchain/ && bash build.sh -j$(NCPUS)

clean:
	echo "   RM        *.o"
	rm -f $(ASM_OBJS) $(OBJS)
	echo "   RM        *.a"
	rm -f $(shell find -name "*.a")
	echo "   RM        *.d"
	rm -f $(shell find -name "*.d")
	echo "   RM        $(MAP)"
	rm -f $(MAP)
	echo "   RM        serial.log"
	rm -f serial.log

iso: $(KERNEL) config
	echo "   ISO       $(ISO)"
	mkdir $(ISO_DIR)
	cp $(KERNEL) $(ISO_DIR)
	cp $(LIMINE_CFG) $(ISO_DIR)
	cp $(LIMINE_SYS) $(ISO_DIR)
	cp $(LIMINE_CD) $(ISO_DIR)
	cp $(LIMINE_EFI) $(ISO_DIR)
	$(MKISOFS) -b limine-cd.bin -no-emul-boot \
			-boot-load-size 4 -boot-info-table \
			--efi-boot limine-cd-efi.bin \
			-efi-boot-part --efi-boot-image \
			--protective-msdos-label $(ISO_DIR) -o $(ISO)
	rm -rf $(ISO_DIR)
	$(LIMINE_DEPLOY) $(ISO)

run: $(KERNEL) iso
	echo "   QMU       $(ISO)"
	qemu-system-$(ARCH) $(QEMU_FLAGS) -cdrom $(ISO)

serial: $(KERNEL) iso
	echo "   SRL       $(ISO)"
	qemu-system-$(ARCH) $(QEMU_FLAGS) -serial file:serial.log -cdrom $(ISO)

debug: $(KERNEL) iso
	echo "   GDB       $(ISO)"
	qemu-system-$(ARCH) $(QEMU_FLAGS) -s -S -cdrom $(ISO)

mrproper: clean
	echo "   RM        $(KERNEL)"
	rm -f $(KERNEL)
	echo "   RM        $(ISO)"
	rm -f $(ISO)
	echo "   RM        $(CONFIG_PATH)"
	rm -f $(CONFIG_PATH)
	echo "   RM        utils/toolchain/*"
	cd utils/toolchain/ && rm -rf bin/ gcc* binutils* \
		include/ share/ lib* x86_64-elf/

.SILENT:
.PHONY: all config iso run runv serial debug toolchain clean mrproper
