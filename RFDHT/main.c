/*
 * main.c
 *
 *  Created on: Apr 10, 2014
 *      Author: ray
 */

#define DHT_DATA_LENGTH 5
#define WAIT_TIMEOUT 60 // seconds

#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "macros.h"
#include "rfproto.h"

#define OUR_ID 50
#define RPI_ID 0

#define PORT_DHT PORTC
#define PIN_DHT 0

#define CRC_LENGTH 1
#define ADDR_DATA_LENGTH 2

ISR(TIMER1_COMPA_vect) {}

void timer_setup(void) {
	cli();
	TCCR1A = 0; /* first timer control register */
	TCCR1B = 0b1100; /* second timer control register */
	TCNT1 = 0;  /* initial counter state */
	OCR1A = 31250;  /* max counter (8Mhz) */
	TIMSK = 0b10000; /* timer interrupt mask */
}

void wait_timeout(void) {
	unsigned int wait_counter = 0;
	sei();
	while(wait_counter++ < WAIT_TIMEOUT) {
		set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_mode();
	}
	cli();
}

void dht_port_set_to_transmit(void) {
    DDR(PORT_DHT) |= _BV(PIN_DHT);
    PORT_DHT &= ~_BV(PIN_DHT);
}

void dht_port_set_to_receive(void) {
    DDR(PORT_DHT) &= ~_BV(PIN_DHT);
    PORT_DHT |= _BV(PIN_DHT);
}

static inline uint8_t _is_level_high() {
	return PIN(PORT_DHT) & _BV(PIN_DHT);
}

static inline uint8_t _is_level_low() {
	return PIN(PORT_DHT) & _BV(PIN_DHT) ? 0 : 1;
}

int get_dht_data(uint8_t *data) {
	dht_port_set_to_transmit();
	_delay_ms(20);
	dht_port_set_to_receive();
	_delay_us(20);

	while(_is_level_low())
		_delay_us(10);
	while(_is_level_high())
		_delay_us(10);

	while(_is_level_low())
		_delay_us(5);
	while(_is_level_high())
		_delay_us(5);

	for(uint8_t i=0;i<5;i++) {
		data[i] = 0;
		for(uint8_t j=0;j<8;j++) {
			while(_is_level_low())
				_delay_us(5);
			uint8_t counter = 0;
			while(_is_level_high()) {
				counter++;
				_delay_us(5);
			}
			data[i] <<= 1;
			if(counter > 7)
				data[i] |= 0x01;
		}
	}

	if(((data[0] + data[1] + data[2] + data[3]) & 0xff) != data[4])
    	 return -1;

     return 0;
}

int main(void) {
	uint8_t data[DHT_DATA_LENGTH+ADDR_DATA_LENGTH];
	rf_init();
	timer_setup();
	data[0] = RPI_ID;
	data[1] = OUR_ID;
	while(1) {
		if(get_dht_data(data+ADDR_DATA_LENGTH) == 0) {
			rf_send_block(data, DHT_DATA_LENGTH+ADDR_DATA_LENGTH-CRC_LENGTH);
			wait_timeout();
		} else
			_delay_ms(2000);
	}
	return 0;
}
