# Makefile for sparrow low-power communication protocol
CC = avr-gcc

MCU = atmega128rfa1
F_CPU = 16000000UL
F_RTC = 32768UL
OPT = s
SPARROW_LIBPATH = /home/tudor/Documents/sparrow/software/lib
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DF_RTC=$(F_RTC) -Wall -Wextra\
	 -O$(OPT) -I$(SPARROW_LIBPATH)

SRC = \
      $(SPARROW_LIBPATH)/platform/adc/adc.c \
      $(SPARROW_LIBPATH)/platform/radio/radio.c \
      $(SPARROW_LIBPATH)/platform/sc/sc.c \
      $(SPARROW_LIBPATH)/board/sht21/sht21.c


# General targets
default: help 

help:
	@echo "makefile for sparrow low power communication protocol firmware"
	@echo "targets summary:"
	@echo "\tgeneral targets:"
	@echo "\t\tmake [help]\t\t- display this helpful text :)"
	@echo "\t\tmake build\t\t- build everything"
	@echo "\t\tmake clean\t\t- clean everything\n"
	@echo "\tleaf nodes:"
	@echo "\t\tmake leaf.hex\t\t- build code for leaf nodes"
	@echo "\t\tmake leaf\t\t- build and upload code to leaf nodes"
	@echo "\t\tmake clean_leaf\t\t- clean code for leaf nodes"
	@echo "\troot nodes:"
	@echo "\t\tmake root.hex\t\t- build code for root nodes"
	@echo "\t\tmake root\t\t- build and upload code to root nodes"
	@echo "\t\tmake clean_root\t\t- clean code for root nodes"
	@echo "\tgateway node:"
	@echo "\t\tmake gateway.hex\t- build code for gateway node"
	@echo "\t\tmake gateway\t\t- build and upload code to gateway node"
	@echo "\t\tmake clean_gateway\t- clean code for gateway node"

build: leaf.hex root.hex gateway.hex

clean: clean_leaf clean_root clean_gateway

.PHONY: default help build clean


# Leaf nodes
leaf: leaf.hex
	sudo avrdude -c avrispmkII -P usb -p m128rfa1 -U flash:w:$^:a

leaf.hex: leaf.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@
	avr-size $^

leaf.elf: leaf.c $(SRC)
	@echo Building firmware for leaf node...
	$(CC) -DBOARD_SPARROWV32 $(CFLAGS) -o $@ $^

clean_leaf:
	rm -rf leaf.hex leaf.elf

# Root nodes
root: root.hex
	sudo avrdude -c avrispmkII -P usb -p m128rfa1 -U flash:w:$^:a

root.hex: root.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@
	avr-size $^

root.elf: root.c $(SRC)
	@echo Building firmware for root node...
	$(CC) -DBOARD_SPARROWV32 $(CFLAGS) -o $@ $^

clean_root:
	rm -rf root.hex root.elf

# Gateway
gateway: gateway.hex
	sudo avrdude -c avrispmkII -P usb -p m128rfa1 -U flash:w:$^:a

gateway.hex: gateway.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@
	avr-size $^

gateway.elf: gateway.c $(SRC)
	@echo Building firmware for gateway...
	$(CC) -DBOARD_SPARROWDONGLE $(CFLAGS) -o $@ $^

clean_gateway:
	rm -rf gateway.hex gateway.elf
