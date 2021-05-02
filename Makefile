# Base variables
export ARCH?=$(shell uname -m)
CONFIG_PATH=config
GENCONFIG=utils/genconfig/genconfig

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

# Linker script
LD_SCRIPT=arch/$(ARCH)/linkers/stivale2.ld

ASM_FILES:=$(shell find -path ./utils -prune -false -o -name "*.asm")
ASM_OBJS:=$(ASM_FILES:.asm=.o)
C_FILES=$(shell find -path ./utils -prune -false -o -name "*.c")
OBJS=$(C_FILES:.c=.o)

# Suppress "Entering directory..."
MAKEFLAGS += --no-print-directory

all: config $(KERNEL)

config: genconfig
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
				-fno-stack-protector -fno-exceptions \
				-fno-non-call-exceptions -nostdlib \
				--sysroot=$(PWD) -isystem=/include \
				$(CONFIG_CFLAGS)
				#-fstack-protector-strong -fno-exceptions

# Export default LDFLAGS
export LDFLAGS = -n --data-sections $(CFLAGS) $(CONFIG_LDFLAGS)

# Export default LIBS
LIBS = -lk

endif

# If the Cross Compiler is provided, set Make variables
ifndef CROSS_COMPILER_PREFIX
$(error The Cross Compiler Prefix is needed, Native Compiler is unsupported)
else
# Make variables
export CC=$(CROSS_COMPILER_PREFIX)gcc
export AR=$(CROSS_COMPILER_PREFIX)ar
export NM=$(CROSS_COMPILER_PREFIX)nm
export LD=$(CROSS_COMPILER_PREFIX)ld
# Only tested with nasm assembler
export ASM=nasm

export MKISOFS=xorriso -as mkisofs

endif

endif

$(KERNEL): $(ASM_OBJS) $(OBJS) libk.a config
	echo "   LD        $(KERNEL)"
	$(CC) $(LDFLAGS) -T $(LD_SCRIPT) -o $(KERNEL) $(ASM_OBJS) $(OBJS) $(LIBS)
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
libk.a: $(wildcard lib/libk/*.c)
	echo "   AR        lib/$@"
	$(AR) rcs lib/$@ $(wildcard lib/libk/*.o)

toolchain:
	cd utils/toolchain/ && bash build.sh $(MAKEFLAGS)

clean:
	echo "   RM        *.o"
	rm -f $(ASM_OBJS) $(OBJS)
	echo "   RM        *.a"
	rm -f $(shell find -name "*.a")
	echo "   RM        $(MAP)"
	rm -f $(MAP)
	echo "   RM        serial.log"
	rm -f serial.log
	echo "   CLEAN     genconfig"
	$(MAKE) -C utils/genconfig/ clean

iso: $(KERNEL) config
	echo "   ISO       $(ISO)"
	mkdir $(ISO_DIR)
	cp $(KERNEL) $(ISO_DIR)
	cp $(LIMINE_CFG) $(ISO_DIR)
	cp $(LIMINE_SYS) $(ISO_DIR)
	cp $(LIMINE_CD) $(ISO_DIR)
	$(MKISOFS) -no-emul-boot -b limine-cd.bin \
            -boot-load-size 4 -boot-info-table -o $(ISO) $(ISO_DIR)
	rm -rf $(ISO_DIR)

run: $(KERNEL) iso
	echo "   QMU       $(ISO)"
	qemu-system-$(ARCH) -no-reboot -cdrom $(ISO)

runv: $(KERNEL) iso
	echo "   VBX       $(ISO)"
	virtualbox $(ISO)

serial: $(KERNEL) iso
	echo "   SRL       $(ISO)"
	qemu-system-$(ARCH) -no-reboot -serial file:serial.log -cdrom $(ISO)

gdb: $(KERNEL) iso
	echo "   GDB       $(ISO)"
	qemu-system-$(ARCH) -no-reboot -s -S -cdrom $(ISO)

genconfig:
	$(MAKE) -C utils/genconfig/

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
	echo "   RM        $(GENCONFIG)"
	$(MAKE) -C utils/genconfig/ mrproper

.SILENT:
.PHONY: all config genconfig iso run runv serial gdb toolchain clean mrproper
