# Set the path to your AVR tool chain (Mac OSX)
# For Linux, omit the PATH and change the CC and OCPY to the toolnames i.e.
# 	CC=$(PATH)/avr-gcc -> CC=avr-gcc

PATH=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin
CONFIG=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf
CC=$(PATH)/avr-gcc
CFLAGS=-Os -Wall
OCPY=$(PATH)/avr-objcopy
MINI=minipro
CLK_SPEED=16000000UL
MMCU=atmega328p
MCU=ATMEGA328P@DIP28
HEX=main.hex

help:
	@echo "all:     Compiles, Converts object to hex, and uploads"
	@echo "compile: Compilation of source file(s)"
	@echo "obj2hex: Converts objects to hex format"
	@echo "dump: 	 Dumps the data from EEPROM, FLASH and FUSE bits of MCU"
	@echo "upload:  Uploads the code to MCU"
	@echo "clean:   Cleans the directory"

all: compile obj2hex upload

compile: main.o

main.o: LCD_16x2.o
	$(CC) $(CFLAGS) -DF_CPU=$(CLK_SPEED) -mmcu=$(MMCU) -o $@ main.c $^

LCD_16x2.o: LCD_16x2.c LCD_16x2.h
	$(CC) $(CFLAGS) -DF_CPU=$(CLK_SPEED) -mmcu=$(MMCU) $^ -c

obj2hex:
	$(OCPY) -O ihex -j .text -j .data main.o main.hex

dump:
	$(MINI) -p $(MCU) -f ihex -r dump -y

upload: dump
	mv dump.eeprom.hex eeprom.hex
	mv dump.fuses.conf fuses.conf
	$(MINI) -p $(MCU) -E -y
	$(MINI) -p $(MCU) -e -c data -f ihex -w eeprom.hex -y
	$(MINI) -p $(MCU) -e -c code -f ihex -w main.hex -y
	$(MINI) -p $(MCU) -e -c config -w fuses.conf -y

clean:
	rm main.o LCD_16x2.o dump LCD_16x2.h.gch
