/*
 * sparrow.c
 * Written by Tudor Vișan, 2014
 *
 * Low power protocol for wireless sensor networks
 * Leaf node implementation
 */ 

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "sparrow.h"


/* protocol states */
typedef enum {
	INIT,
	REGISTER,
	NORMAL_OPERATION,
} state_t;


/* local variables */
static uint8_t node_id		= 0xFF;
static uint8_t parent_id	= 0xFF;
static uint16_t parent_score	= 0x115F; // WORST SCORE EVAAAAR!
static uint8_t battery		= 100;
static uint8_t wakeup_period	= 0;
static state_t state		= INIT;

static RX_callback user_rx	= 0;
static frame_t rx_frame;

static TX_callback user_tx	= 0;
static frame_t tx_frame;


/*
 * initialise the uC
 * disable all peripherals
 */
inline void mcu_init(void)
{
	// disable peripherals
	PRR0 = 0xFF;
	PRR1 = 0xFF;
	
	// enable SRAM data retention for all blocks
	DRTRAM0 |= (1 << ENDRT);
	DRTRAM1 |= (1 << ENDRT);
	DRTRAM2 |= (1 << ENDRT);
	DRTRAM3 |= (1 << ENDRT);

	// get node id from eeprom
	node_id = eeprom_read_byte((uint8_t*)NODE_ID_EEPROM_PTR);
}

/*
 * callback on user wakeup
 * called be scounter
 */
void user_wakeup(void)
{
	switch (state)
	{
		case NORMAL_OPERATION:
			// execute user function and set next wakeup timer
			user_tx();
			scounter_delay(wakeup_period, 0, OCR2);
			break;

		default:
			// TODO: report error
	}
}

/*
 * callback on transmission wakeup
 * called by scounter
 */
void tx_wakeup(void)
{
	switch (state)
	{
		case NORMAL_OPERATION:
			// wake up transceiver
			TRXPR &= ~(1 << SLPTR);
			break;

		default:
			// TODO: report error
	}
}

/*
 * callback on transceiver wakeup
 * called by transceiver
 */
void trx_wakeup(void)
{
	switch (state)
	{
		case NORMAL_OPERATION:
			// build new frame
			tx_frame.data[NODE_ID]	= node_id;
			tx_frame.data[BATTERY]	= battery; // TODO: measure battery
			tx_frame.data[TYPE]	= FRAME_DATA;
			tx_frame.phr = 3;

			// send new frame
			radio_send_frame(&tx_frame);

			// set wakeup timer
			scounter_delay(wakeup_period, 0, OCR1);
			break;

		default:
			// TODO: report error
	}
}

/*
 * callback on transmission end
 * called by transceiver
 */
void tx_end(void)
{
	// put transceiver to sleep
	radio_set_state_force(CMD_TRX_OFF);
	TRXPR |= (1 << SLPTR);
}

/*
 * callback on reception end
 * called by transceiver
 */
void rx_end(void)
{
	uint8_t i;

	if (!radio_frame_is_valid())
		return;

	// save received frame
	rx_frame.phr = TRX_FRAME_BUFFER(0);
	for (i = 0; i < rx_frame.phr; ++i)
		rx_frame.data[i] = TRX_FRAME_BUFFER(i + 1);

	// process received frame
	switch (state)
	{
		case REGISTER:
			if (rx_frame.data[TYPE] == FRAME_RA)
			{
				uint16_t score = 16 * rx_frame.data[HOP_COUNT];
				score += 4 * radio_get_frame_rssi();
				score += rx_frame.data[NUM_CHILDREN];

				if (score < parent_score)
				{
					// if this RA advertises a node with a
					// better score than your currently
					// chosen parent, choose this new node
					parent_id = rx_frame.data[NODE_ID];
					parent_score = score;

					// TODO: choose a slot from this node
				}
			} 

			break;

		case NORMAL_OPERATION:
			// TODO: synchronize

			if (user_rx != 0)
				user_rx(_frame);

			break;

		default:
			// TODO: report error
	}
}

/*
 * puts the uC to sleep
 */
inline void sleep(void)
{
	// go to sleep
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);

	cli();
	sleep_enable();
	sei();
	sleep_cpu();

	// stop sleeping
	sleep_disable();
}

/*
 * initialise sparrow protocol
 * THE FUNCTION DOES NOT RETURN!
 * 
 * @param rx
 * 	a function to be called after a packet is received
 * @param tx
 * 	a function to be called before a packet is transmitted
 * @param tx_delay
 * 	how long, in seconds, before the transmission window to call the tx function
 * 	must be less than or equal to period
 * 	if only taken into account if tx is not NULL
 */
void __noreturn__ sparrow_leaf_init(RX_callback rx, TX_callback tx, 
		uint8_t tx_delay)
{
	uint8_t user_tx_delay = 0, registered = 0;
	uint32_t timestamp;

	while (1)
	{
		switch (state)
		{
			case INIT:
				// initialise system
				mcu_init();
				radio_init(trx_wakeup, 0, rx_end, tx_end);
				scounter_init(tx_wakeup, (tx != 0)? user_wakeup: 0);
				sei();

				// save user parameters
				if (rx != 0)
					user_rx = rx;

				if (tx != 0)
				{
					user_tx = tx;
					user_tx_delay = tx_delay;
				}

				state = REGISTER;
				break;

			case REGISTER:
				// wake up transceiver
				TRXPR &= ~(1 << SLPTR);

				// wait for transceiver to wake up
				while (radio_get_state() == SLEEP)
					sleep();

				// the network period is still unknown so sleep
				// until you receive a RA
				while (wakeup_period == 0)
					sleep();

				// collect all RAs for one time period
				scounter_delay(wakeup_period, 0, OCR1);
				while (!scounter_timeout_expired(OCR1))
					sleep();
				
				// TODO: register to parent node

				// use OCR1 for transmission wakeup
				scounter_delay(wakeup_period, 0, OCR1);

				state = NORMAL_OPERATION;
				break;

			case NORMAL_OPERATION:
				sleep();
				break;

			default:
				// TODO: report error
				state = INIT;
		}
	}
}

/*
 * returns the node's id
 *
 * @return the node's id
 */
uint8_t sparrow_get_node_id(void)
{
	return node_id;
}
