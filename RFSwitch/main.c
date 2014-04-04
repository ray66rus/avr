/*
 * main.c
 *
 *  Created on: Apr 5, 2014
 *      Author: ray
 */

#include "macros.h"
#include "rfproto.h"

#define CONTROL_PORT PORTB
#define CONTROL_PIN 1
#define INITIAL_STATE 0

#define OUR_ID 48

#define IS_COMMAND_ON(data, len) len == 3 && data[0] == OUR_ID && data[2] == 49
#define IS_COMMAND_OFF(data, len) len == 3 && data[0] == OUR_ID && data[2] == 48

void control_pin_init(void) {
	DDR(CONTROL_PORT) |= _BV(CONTROL_PIN);
	if(INITIAL_STATE)
		CONTROL_PORT |= _BV(CONTROL_PIN);
	else
		CONTROL_PORT &= ~_BV(CONTROL_PIN);
}

int main(void) {
	unsigned char data[PACKET_LENGTH];
	int8_t data_length;

	rf_init();
	control_pin_init();
	while(1) {
		data_length = rf_recv_block(data, 1000);
		if(data_length < 0)
			continue;
		if(IS_COMMAND_ON(data, data_length))
			CONTROL_PORT |= _BV(CONTROL_PIN);
		else if(IS_COMMAND_OFF(data, data_length))
			CONTROL_PORT &= ~_BV(CONTROL_PIN);
	}
	return 0;
}
