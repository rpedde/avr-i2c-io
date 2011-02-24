AVRLIB=~/working.garage/avrlib
TARGET=i2c-io
SOURCES=$(TARGET).c $(AVRLIB)/i2c.c $(AVRLIB)/timerx8.c
LDFLAGS=-Wl,-Map=$(TARGET).map,--cref
MCU=atmega168
F_CPU=20000000

# These could really be included from a toplevel makefile
CC=avr-gcc
AS=avr-gcc -x assembler-with-cpp
OBJCOPY=avr-objcopy
SIZE=avr-size
FORMAT=ihex

OBJECTS=$(SOURCES:.c=.o)
CFLAGS:=-mmcu=$(MCU) -I. -I$(shell pwd) -I$(shell cd ${AVRLIB}; pwd) -Os -DF_CPU=$(F_CPU) -DDEVICE=$(MCU)
ASFLAGS:=-mmcu=$(MCU)
LDFLAGS:=-mmcu=$(MCU)
INCDIR=`pwd`

all:	$(TARGET).elf $(TARGET).hex $(TARGET).ok

.o.c:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $< -o $@

.s.c:
	$(CC) -S $(CFLAGS) -I$(INCDIR) $< -o $@

.o.s:
	$(AS) -c $(ASFLAGS) -I$(INCDIR) $< -o $@

%.elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIB) $(LDFLAGS) -o $@

%.bin: %.elf
	$(OBJCOPY) -O binary -R .eeprom $< $@

%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

# %.eep: %.elf
# 	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# %.cof: %.elf
# 	$(OBJCOPY) --debugging -O coff-ext-avr \
# 		--change-section-address   .data-0x800000 \
# 		--change-section-address    .bss-0x800000 \
# 		--change-section-address .noinit-0x800000 \
# 		--change-section-address .eeprom-0x810000 \
# 		$< $@

%ok:
	$(SIZE) $(TARGET).elf
	@echo "Errors: none" 

clean:
	$(RM) $(OBJ)
	$(RM) $(SOURCES:.c=.s)
	$(RM) $(SOURCES:.c=.lst)
	$(RM) $(TARGET).map
	$(RM) $(TARGET).elf
	$(RM) $(TARGET).cof
	$(RM) $(TARGET).obj
	$(RM) $(TARGET).a90
	$(RM) $(TARGET).sym
	$(RM) $(TARGET).eep
	$(RM) $(TARGET).hex
	$(RM) $(TARGET).bin
	@echo "Errors: none"

size:
	$(SIZE) $(TARGET).elf

