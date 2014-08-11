/*
 * Written by Tudor Vișan, 2014
 *
 * Low power protocol for wireless sensor networks
 */ 

#ifndef SPARROW_H_
#define SPARROW_H_

#include <platform.h>


/* node id eeprom address */
#define NODE_ID_EEPROM_PTR	0x42

/* sleep period for your deep sleeping needs */
#define DEEP_SLEEP_PERIOD	600		

typedef void (*RX_callback) (frame_t *);
typedef void (*TX_callback) (void);

void sparrow_leaf_init(uint8_t, RX_callback, TX_callback, uint8_t);
void sparrow_root_init(uint8_t, RX_callback, TX_callback, uint8_t);
uint8_t sparrow_get_node_id(void);
#endif /* SPARROW_H_ */
