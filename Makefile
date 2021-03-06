
######################
# Makefile for Tinix #
######################


# Entry point of Tinix
# It must be as same as 'KernelEntryPointPhyAddr' in load.inc!!!
ENTRYPOINT	= 0x30400

# Offset of entry point in kernel file
# It depends on ENTRYPOINT
ENTRYOFFSET	=   0x400


# Programs, flags, etc.
TOPDIR		:= $(shell /bin/pwd)
ASM		= nasm
DASM		= ndisasm
CC		= gcc
LD		= ld
ASMBFLAGS	= -I$(TOPDIR)/boot/include -Iinclude/
ASMKFLAGS	= -I$(TOPDIR)/include -f elf
CFLAGS		= -I$(TOPDIR)/include -c -fno-builtin
LDFLAGS		= -s -Ttext $(ENTRYPOINT)
DASMFLAGS	= -u -o $(ENTRYPOINT) -e $(ENTRYOFFSET)

# This Program
TINIXBOOT	= boot/boot.bin boot/setup.bin
TINIXKERNEL	= kernel.bin
OBJS	=
#OBJS		= kernel/kernel.o kernel/start.o kernel/main.o kernel/clock.o
#OBJS		= kernel/kernel.o kernel/main.o kernel/clock.o\
#			kernel/i8259.o kernel/global.o kernel/protect.o\
#			lib/klib.o lib/klibc.o lib/string.o
DASMOUTPUT	= kernel.bin.asm

export TOPDIR ASM DASM CC LD ASMBFLAGS ASMKFLAGS\
	CFLAGS LDFLAGS DASMFLAGS OBJS

include kernel/Makefile

include lib/Makefile

# All Phony Targets
.PHONY : everything final image clean realclean disasm all buildimg

# Default starting position
everything : $(TINIXBOOT) $(TINIXKERNEL)

all : realclean everything

final : all clean

image : final buildimg

clean :
	rm -f boot/boot.bin
	rm -f boot/setup.bin
	rm -f $(OBJS)

realclean :
	rm -f $(OBJS) $(TINIXBOOT) $(TINIXKERNEL)

disasm :
	$(DASM) $(DASMFLAGS) $(TINIXKERNEL) > $(DASMOUTPUT)

# Write "boot.bin" & "loader.bin" into floppy image "TINIX.IMG"
# We assume that "TINIX.IMG" exists in current folder
buildimg :
	mount gosix.flp /mnt/fly -o loop
	cp -f boot/setup.bin /mnt/floppy/
	cp -f kernel.bin /mnt/floppy
	umount  /mnt/floppy

#boot/boot.bin : boot/boot.asm boot/include/load.inc boot/include/fat12hdr.inc
boot/boot.bin : boot/boot.S boot/include/load.inc boot/include/fat12hdr.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

#boot/loader.bin : boot/loader.asm boot/include/load.inc boot/include/fat12hdr.inc boot/include/pm.inc
boot/setup.bin : boot/setup.S boot/include/load.inc boot/include/fat12hdr.inc boot/include/pm.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

$(TINIXKERNEL) : $(OBJS)
	echo $(OBJS)
	$(LD) $(LDFLAGS) -o $(TINIXKERNEL) $(OBJS)
	mount gosix.flp /mnt/flp -o loop
	cp -f boot/setup.bin /mnt/flp/
	cp -f kernel.bin /mnt/flp/
	umount  /mnt/flp

.c.o:
	$(CC) $(CFLAGS) -o $@ $<
.S.o:
	$(ASM) $(ASMKFLAGS) -o $@ $<

