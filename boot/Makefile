# Makefile for boot
# Programfs, flags, etc.
ASM		= nasm
ASMFLAGS	= 

# This Program
TARGET		= boot.bin setup.bin

# All Phony Targets
.PHONY : everything clean all

# Default starting position
everything : $(TARGET)
	cp setup.bin /mnt/flp/

clean:
	rm -f $(TARGET)

all:
	clean everything

boot.bin: boot.S ./include/fat12hdr.inc ./include/load.inc
	$(ASM) $(ASMFLAGS) -o $@ $<

setup.bin: setup.S ./include/load.inc ./include/fat12hdr.inc ./include/pm.inc
	$(ASM) $(ASMFLAGS) -o $@ $<
