###############################################################################
# Makefile for the project OK2560_BD
###############################################################################

## General Flags
PROJECT = OK2560_BD
MCU = atmega2560
TARGET = OK2560_BD.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2       -DF_CPU=16000000UL -O0 -fsigned-char
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += 


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\work\SEOHO_VD\OK2560(080617)\AnybusIC" -I"D:\work\SEOHO_VD\OK2560(080617)\source" -I"D:\work\SEOHO_VD\OK2560(080617)\." 

## Objects that must be built in order to link
OBJECTS = main.o 128by64GLCD.o 16by2CLCD.o serial.o key.o abic.o crc.o mb.o sd.o tm.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) OK2560_BD.hex OK2560_BD.eep size

## Compile
main.o: ../source/main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

128by64GLCD.o: ../source/128by64GLCD.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

16by2CLCD.o: ../source/16by2CLCD.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

serial.o: ../source/serial.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

key.o: ../source/key.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

abic.o: ../AnybusIC/abic.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

crc.o: ../AnybusIC/crc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mb.o: ../AnybusIC/mb.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sd.o: ../AnybusIC/sd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

tm.o: ../AnybusIC/tm.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) OK2560_BD.elf dep/* OK2560_BD.hex OK2560_BD.eep

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

