# Makefile for sparrow low-power communication protocol
CC = avr-gcc

MCU = atmega128rfa1
F_CPU = 16000000UL
OPT = s
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall -Wextra\
	 -O$(OPT)


# General targets
default: help

help:
	@echo "makefile for address_helper"
	@echo "targets summary:"
	@echo "\tmake [help]\t\t- display this helpful text :)"
	@echo "\tmake build\t\t- build address_helper"
	@echo "\tmake clean\t\t- clean address_helper\n"
	@echo "\tmake address_helper.hex\t- build code for address_helper"
	@echo "\tmake address_helper\t- build and upload code to node"

build: address_helper.hex

clean:
	rm -rf address_helper.hex address_helper.elf

.PHONY: default help build clean


# address_helper
address_helper: address_helper.hex
	sudo avrdude -c avrispmkII -P usb -p m128rfa1 -U flash:w:$^:a

address_helper.hex: address_helper.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@
	avr-size $^

address_helper.elf: address_helper.c
ifeq ("a$(NODE_ID)", "a")
	@echo You must specify a node address via the NODE_ID variable.
	@echo example: make address_helper.hex NODE_ID=0x42
	@false
else 	
	@echo Building firmware for address_helper with node id $(NODE_ID)...
	$(CC) -DNODE_ID=$(NODE_ID) $(CFLAGS) -o $@ $^
endif

.PHONY: address_helper address_helper.hex address_helper.elf
