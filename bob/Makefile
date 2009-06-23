# Makefile for AT91SAM7 Chumby BOBoard
# Matt Roman 12/08.

MCU 	= arm7tdmi
LDSCRIPT = ld_flash.cmd
# LDSCRIPT = AT91SAM7S64-ROM.ld
OPENOCD = openocd

# Target file name (no extension)
TARGET = main

# variables
# List C source files
SRC = serial.c printf.c $(TARGET).c util.c dma.c

# Assembler source files 
ASRC = crt.s

# Optimization level, can be [0,1,2,3,s]
# 0=turn off optimization. s=optimize for size
# (Note: 3 is not always the best opt level)
OPT = 2

# Compiler flag options !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# set the C standard level.
# c89	- "ANSI" C
# CSTANDARD = -std=c89

# Compiler flags
# -g*:		generate debug info
# -O*:		optimization level
# -f...:	tuning, see GCC and avr-libc docs
# -W...:	warning level
# -Wa,...:	tell GCC to pass these options to the assembler
# -adhlns...:	create assembler listing
#
# Flags for C (arm-elf-gcc)
CFLAGS = -mcpu=$(MCU) -I. -c
CFLAGS += -fno-common
CFLAGS += -O$(OPT)
CFLAGS += -g$(DEBUG)
CFLAGS += -Wall -Wpointer-arith
CFLAGS += -Wredundant-decls -Wshadow -Wcast-qual -fomit-frame-pointer 

# flags only for C
CONLYFLAGS = -Wnested-externs
# CONLYFLAGS += $(CSTANDARD)
CONLYFLAGS += -Wmissing-prototypes -Wstrict-prototypes 
CONLYFLAGS += -Wmissing-declarations

# Assembler flags
# -Wa,...:	tell GCC to pass these options to the assembler
# -ahlns:	create listing
# -g$(DEBUG):	have the assembler create line number info
ASFLAGS = -c -mcpu=$(MCU) -I. -Wa,-ahls,-mapcs-32,-g$(DEBUG)

MATH_LIB = -lm

# Linker flags
# -Wl,...:	tell GCC to pass these options to the linker
# -Map:		create map file
# --cref:	add cross ref to map file
# -T...:	load the flash script to write program to correct mem space
LDFLAGS = -nostdlib -Wl,-Map,$(TARGET).map,--cref,--gc-sections
#LDFLAGS += $(MATH_LIB)
#LDFLAGS += -lc
#LDFLAGS += -lgcc
LDFLAGS += -T$(LDSCRIPT)

LIBRARIES = -lgcc

# ---------------------------------------------------------------------

CC = arm-elf-gcc
AS = arm-elf-as
OBJCOPY = arm-elf-objcopy
OBJDUMP = arm-elf-objdump
SIZE = arm-elf-size
REMOVE = rm -f
COPY = cp

# ---------------------------------------------------------------------
# Define messages 
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = ---------- BEGIN -----------
MSG_END = -------- END ---------
MSG_FLASH = Creating load file for Flash:
MSG_FLASHING_CHIP = !!!!!!! FLASHING CHIP NOW !!!!!!!
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_ARM = "Compiling C (ARM-only):"
MSG_ASSEMBLING = Assembling:
MSG_ASSEMBLING_ARM = "Assembling (ARM-only):"
MSG_CLEANING = Cleaning project:
MSG_BAR = "*****************************************************************"
# ---------------------------------------------------------------------

# DEFINE ALL OBJECT FILES
COBJ	=$(SRC:.c=.o)
AOBJ	=$(ASRC:.s=.o)

# DEFINE ALL LISTING FILES
LST = $(ASRC:.s=.lst) $(SRC:.c=.lst)

all: begin clean build sizeafter flash finished end

build: elf bin dmp
bin: $(TARGET).bin
elf: $(TARGET).elf
dmp: $(TARGET).dmp

# Create final output file (.bin) from ELF output file
%.bin: %.elf
	@echo $(MSG_BAR)
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O binary $< $@

# Linking ELF output file from object files
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(AOBJ) $(COBJ)
%.elf:	$(AOBJ) $(COBJ)
	@echo $(MSG_BAR)
	@echo $(MSG_LINKING) $@
	$(CC) $(LDFLAGS) -o $@ $(AOBJ) $(COBJ) $(LIBRARIES)

%.dmp: %.elf
	@echo $(MSG_BAR)
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -x $< > $@

# Compile created object files from C source files. ARM/Thumb
$(COBJ) : %.o : %.c
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(CFLAGS) $(CONLYFLAGS) $< -o $@
	@echo


# Assemble created object files from assembler source files. ARM-only
$(AOBJ) : %.o : %.s
	@echo
	@echo $(MSG_ASSEMBLING_ARM) $<
	$(CC) $(ASFLAGS) $< -o $@ > $*.lst
	@echo

clean:
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map 
	$(REMOVE) $(TARGET).dmp
	$(REMOVE) $(LST)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)
	$(REMOVE) $(COBJ)
	$(REMOVE) $(AOBJ)
	@echo $(MSG_BAR)

begin:
	@echo
	@echo $(MSG_BEGIN)
	@echo
	@$(CC) --version

finished:
	@echo $(MSG_ERRORS_NONE)
	@echo "Kick Ass !"

end:
	@echo $(MSG_END)
	@echo

# Display size of file.
ELFSIZE = $(SIZE) -A $(TARGET).elf
sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi

flash: $(TARGET).bin
	@ echo $(MSG_FLASHING_CHIP)
	$(OPENOCD) -f openocd-flash.cfg

program: begin flash end

.PHONY : all begin finished flash end sizeafter build elf bin dmp clean program


