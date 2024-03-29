# Base variables
export ARCH?=$(shell uname -m)
CONFIG_PATH=config
NCPUS=$(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)
QEMU_FLAGS=-enable-kvm -cpu host -m 2G -no-reboot -no-shutdown

# Output files
KERNEL_NAME=phoenix
KERNEL=$(KERNEL_NAME).elf
ISO=$(KERNEL_NAME)-$(ARCH).iso
MAP=$(KERNEL).map
ISO_DIR=iso_files

# Limine
LIMINE_UTILITY=utils/limine/limine
LIMINE_CFG=utils/limine/limine.cfg
LIMINE_BIOS_CD=utils/limine/limine-bios-cd.bin
LIMINE_BIOS_SYS=utils/limine/limine-bios.sys
LIMINE_UEFI_CD=utils/limine/limine-uefi-cd.bin

# Linker script
LD_SCRIPT=arch/$(ARCH)/limine.ld

ASM_FILES:=$(shell find -path ./utils -prune -false -o -name "*.asm")
ASM_OBJS:=$(ASM_FILES:.asm=.o)
C_FILES=$(shell find -path ./utils -prune -false -o -name "*.c")
OBJS=$(C_FILES:.c=.o)

# Console font
FONT=utils/font.psf
FONT_OBJ=$(FONT:.psf=.o)

# Suppress "Entering directory..."
MAKEFLAGS+=--no-print-directory
-include $(C_FILES:.c=.d)

all: config $(KERNEL)

config:
	# Check if config file exists
ifeq ("$(wildcard $(CONFIG_PATH))", "")
	cp -f arch/$(ARCH)/default-config config
	$(info $(ARCH) default config copied, refer to the documentation if nessesary)
	exit -1
else

# Include config file
include $(CONFIG_PATH)

# Only x86_64 is supported at this time
ifneq ($(ARCH), x86_64)
	$(error Unknow Architecture $(ARCH))
else
# Export default CFLAGS
export CFLAGS = \
	-Wall \
	-Wextra \
	-Wimplicit-fallthrough \
	-Wunused \
	-std=c2x \
	-march=$(MARCH) \
	-O2 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-PIE \
	-fno-PIC \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel \
	--sysroot=$(PWD) \
	-isystem=/include \
	-DKEYBOARD_$(CONFIG_KEYBOARD) \
	$(CONFIG_CFLAGS)

# Export default LDFLAGS
export LDFLAGS = \
	-nostdlib \
	-static \
	-m elf_$(ARCH) \
	-z max-page-size=0x1000 \
	$(CONFIG_LDFLAGS)

endif

ifeq ($(CONFIG_COMPILER),gcc)

# Check if the GCC cross compiler prefix is provided
ifndef CONFIG_GCC_PREFIX
	$(error The GCC compiler prefix is needed, the native compiler is unsupported)
endif

export CFLAGS += \
	-fno-lto

export CC=$(CONFIG_GCC_PREFIX)gcc
export AR=$(CONFIG_GCC_PREFIX)ar
export NM=$(CONFIG_GCC_PREFIX)nm
export LD=$(CONFIG_GCC_PREFIX)ld
export OBJCPY=$(CONFIG_GCC_PREFIX)objcopy

else ifeq ($(CONFIG_COMPILER),llvm)

export CFLAGS += \
	-flto \
	-I $(PWD)/include \
	--target=$(TRIPLE)

ifndef CONFIG_LLVM_PREFIX

export CC=clang
export AR=llvm-ar
export NM=llvm-nm
export LD=ld.lld
export OBJCPY=llvm-objcopy

else

export CC=$(CONFIG_LLVM_PREFIX)/bin/clang
export AR=$(CONFIG_LLVM_PREFIX)/bin/llvm-ar
export NM=$(CONFIG_LLVM_PREFIX)/bin/llvm-nm
export LD=$(CONFIG_LLVM_PREFIX)/bin/ld.lld
export OBJCPY=$(CONFIG_LLVM_PREFIX)/bin/llvm-objcopy

endif

else
	$(error The compiler \($(CONFIG_COMPILER)\) is unknown or not supported)
endif

endif

# Only tested with nasm assembler
export ASM=nasm
export MKISOFS=xorriso -as mkisofs

$(KERNEL): $(ASM_OBJS) $(OBJS) $(FONT_OBJ) lib/libk.a lib/libpsf.a config
	echo "   LD        $(KERNEL)"
	$(LD) $(LDFLAGS) -T $(LD_SCRIPT) -o $(KERNEL) $(ASM_OBJS) $(OBJS) $(FONT_OBJ)
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

lib/libpsf.a: $(wildcard lib/libpsf/*.c)
	echo "   AR        $@"
	$(AR) rcs $@ $(wildcard lib/libpsf/*.o)

$(FONT_OBJ): $(FONT)
	echo "   OBJCPY    $@"
	$(OBJCPY) -O elf64-x86-64 -B i386 -I binary $*.psf $@

toolchain:
ifneq ($(CONFIG_COMPILER),llvm)
	cd utils/toolchain/ && bash build.sh -j$(NCPUS)
else
	$(error You do not need to compile the cross compiler with LLVM)
endif

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
	$(LIMINE_UTILITY) enroll-config --reset $(LIMINE_BIOS_SYS) --quiet

iso: $(KERNEL) config
	echo "   ISO       $(ISO)"
	$(LIMINE_UTILITY) enroll-config --quiet $(LIMINE_BIOS_SYS) $(shell b2sum $(LIMINE_CFG) | cut -d' ' -f1)
	mkdir $(ISO_DIR)
	cp $(KERNEL) $(ISO_DIR)
	cp $(LIMINE_CFG) $(ISO_DIR)
	cp $(LIMINE_BIOS_SYS) $(ISO_DIR)
	cp $(LIMINE_BIOS_CD) $(ISO_DIR)
	cp $(LIMINE_UEFI_CD) $(ISO_DIR)
	$(MKISOFS) -b limine-bios-cd.bin -no-emul-boot \
		-boot-load-size 4 -boot-info-table \
		--efi-boot limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image \
		--protective-msdos-label $(ISO_DIR) -o $(ISO)
	rm -rf $(ISO_DIR)
	$(LIMINE_UTILITY) bios-install $(ISO)

run: $(KERNEL) iso
	echo "   QMU       $(ISO)"
	qemu-system-$(ARCH) $(QEMU_FLAGS) -cdrom $(ISO)

serial: CFLAGS += -fno-lto
serial: $(KERNEL) iso
	echo "   SRL       $(ISO)"
	qemu-system-$(ARCH) $(QEMU_FLAGS) -serial file:serial.log -cdrom $(ISO)

debug: CFLAGS += -fno-lto
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
.ONESHELL:
.PHONY: all config iso run runv serial debug toolchain clean mrproper
