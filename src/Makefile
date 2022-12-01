# Set the path to your AVR tool chain (Mac OSX)
# For Linux, omit the PATH and change the CC and OCPY to the toolnames i.e.
# 	CC=$(PATH)/avr-gcc -> CC=avr-gcc

PATH=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin
CONFIG=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf
CC=$(PATH)/avr-gcc
CFLAGS=-Os -Wall
OCPY=$(PATH)/avr-objcopy
DUDE=$(PATH)/avrdude
CLK_SPEED=16000000UL
MMCU=atmega328p
MCU=ATmega328P
PROG=arduino
HEX=main.hex
UPLD=flash:w:$(HEX):i

help:
	@echo "all:     Compiles, Converts object to hex, and uploads"
	@echo "compile: Compilation of source file(s)"
	@echo "obj2hex: Converts objects to hex format"
	@echo "upload:  Uploads the code to MCU"
	@echo "clean:   Cleans the directory"

all: compile obj2hex upload

compile: main.o

main.o: LCD_16x2.o
	$(CC) $(CFLAGS) -DF_CPU=$(CLK_SPEED) -mmcu=$(MMCU) -o $@ main.c $^

LCD_16x2.o: LCD_16x2.c LCD_16x2.h
	$(CC) $(CFLAGS) -DF_CPU=$(CLK_SPEED) -mmcu=$(MMCU) $^ -c

obj2hex:
	$(OCPY) -O ihex -j .text -j .data main.o $(HEX)

upload:
	$(DUDE) -C $(CONFIG) -p $(MCU) -c $(PROG) -U $(UPLD) -D
clean:
	rm main.o main.hex LCD_16x2.o LCD_16x2.h.gch
