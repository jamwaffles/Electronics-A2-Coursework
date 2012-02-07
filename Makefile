DEVICE  = atmega32u2
CLOCK = 16000000
PROGRAMMER = -c usbasp
OBJECTS = ADC.o
PROJECT_NAME = ADC

# Remove clock divider, set external crystal, enable clock output
FUSES = -U lfuse:w:0xbf:m -U hfuse:w:0xd9:m

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -std=gnu99 -g -Wall -Winline -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

LINK_FLAGS = -lc -lm

# symbolic targets:
all:	$(PROJECT_NAME).hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:$(PROJECT_NAME).hex:i

pflash:	all
	$(AVRDUDE) -n -U flash:w:$(PROJECT_NAME).hex:i

fuse:
	$(AVRDUDE) $(FUSES)

install: flash fuse

load: all
	bootloadHID $(PROJECT_NAME).hex

clean:
	rm -f $(PROJECT_NAME).hex $(PROJECT_NAME).elf $(OBJECTS)

# file targets:
$(PROJECT_NAME).elf: $(OBJECTS)
	$(COMPILE) -o $(PROJECT_NAME).elf $(OBJECTS) $(LINK_FLAGS)

$(PROJECT_NAME).hex: $(PROJECT_NAME).elf
	rm -f $(PROJECT_NAME).hex
	avr-objcopy -j .text -j .data -O ihex $(PROJECT_NAME).elf $(PROJECT_NAME).hex

disasm:	$(PROJECT_NAME).elf
	avr-objdump -S -d $(PROJECT_NAME).elf

cpp:
	$(COMPILE) -E $(PROJECT_NAME).c

%.lst: %.c
	{ echo '.psize 0' ; $(COMPILE) -S -g -o - $< ; } | avr-as -alhd -mmcu=$(DEVICE) -o /dev/null - > $@
