/*
 * address_helper.c
 * Written by Tudor Vișan, 2014
 *
 * Tool for writing node id's in eeprom
 */ 

#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

/* 
 * node address should be specified at compile time (yeah... I know)
 * via the NODE_ID macro
 */
#ifndef NODE_ID
#define NODE_ID 		0xFF
#endif
#define NODE_ID_EEPROM_PTR	0x42

int main(void) {
	// write node id in eeprom
	eeprom_write_byte((uint8_t*)NODE_ID_EEPROM_PTR, NODE_ID);
	
	// go to sleep indefinitely
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sleep_enable();
	sei();
	sleep_cpu();

	while(1);

	return 0;
}
