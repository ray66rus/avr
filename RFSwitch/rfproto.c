/*
 * rfproto.h
 *
 *  Created on: Mar 13, 2014
 *      Author: ray
 */

#include "rfproto.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"

struct RF_TIMER_STRUCT {
	uint8_t is_on;
	uint16_t counter;
	uint8_t is_expired;
} __timer;

ISR(TIMER1_COMPA_vect) {
	if(__timer.is_on == 0)
		return;
	if(__timer.counter == 0) {
		__timer.is_expired = 1;
		__timer.is_on = 0;
		return;
	}
	__timer.counter--;
}

void set_timeout(uint16_t delay) {
	cli();
	__timer.counter = delay;
	__timer.is_expired = 0;
	__timer.is_on = 1;

	TCCR1A = 0; /* first timer control register */
	TCCR1B = 0b1011; /* second timer control register */
	TCNT1 = 0;  /* initial counter state */
	OCR1A = 125;  /* max counter */
	TIMSK = 0b10000; /* timer interrupt mask */
	sei();
}

void clear_timeout() {
	cli();
	__timer.counter = __timer.is_on = __timer.is_expired = 0;
}

int rf_init() {
	DDR(RX_PORT) &= ~_BV(RX_PIN);
	RX_PORT &= ~_BV(RX_PIN);
	return 1;
}

unsigned char calc_crc8(unsigned char *buf, unsigned int len) {
	unsigned char crc = 0xFF;
	unsigned int i;

	while(len--) {
		crc ^= *buf++;
		for(i=0; i<8; i++) {
			crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
		}
	}
	return crc;
}

unsigned char is_crc8_ok(unsigned char *buf, unsigned int len, unsigned char crc8) {
	unsigned char new_crc8 = calc_crc8(buf, len);
	return (crc8 == new_crc8) ? 1 : 0;
}

static inline uint8_t _is_level_high() {
	return PIN(RX_PORT) & _BV(RX_PIN);
}

static inline uint8_t _is_level_low() {
	return PIN(RX_PORT) & _BV(RX_PIN) ? 0 : 1;
}

static inline uint8_t _rf_get_next_bit() {
	uint8_t counter;
	while(_is_level_high() && !__timer.is_expired)
		_delay_us(ATOMIC_DELAY);
	for(counter=0;_is_level_low() && !__timer.is_expired;counter++)
		_delay_us(ATOMIC_DELAY);
	return (counter > ATOMIC_DELAYS_IN_TIME_LOW) ? 1 : 0;
}

uint8_t _get_preamble_and_sync() {
	int current_bit = _rf_get_next_bit();
	int preamble_length = 1;
	int next_bit = current_bit;
	do {
		current_bit = next_bit;
		next_bit = _rf_get_next_bit();
		preamble_length++;
	} while(current_bit != next_bit);
	if(preamble_length < 10 || current_bit == 0)
		return 0;
	for(uint8_t i=0;i<6;i++)
		current_bit = _rf_get_next_bit();
	return (current_bit == 1) ? 1 : 0;
}


int rf_recv_block(unsigned char *buf, uint16_t timeout) {
	unsigned char data[PACKET_DATA_LENGTH_WITH_SIZE_AND_CRC];

	if(timeout != 0)
		set_timeout(timeout);

	while(_is_level_low() && !__timer.is_expired)
		_delay_us(ATOMIC_DELAY);

	while(!_get_preamble_and_sync() && !__timer.is_expired) ;

	if(__timer.is_expired)
		return RF_ERR_TOUT;
	clear_timeout();
	set_timeout(MAX_DATA_RECV_TIME_MS);

	for(uint8_t l=0;l<PACKET_DATA_LENGTH_WITH_SIZE_AND_CRC;l++) {
		data[l] = 0;
		for(int p=0;p<8;p++) {
			data[l] <<= 1;
			data[l] |= _rf_get_next_bit();
		}
	}

	if(__timer.is_expired)
			return RF_ERR_TOUT;
	clear_timeout();

	if(!is_crc8_ok(data, PACKET_DATA_LENGTH + 1, data[PACKET_DATA_LENGTH + 1]))
		return RF_ERR_CRC_ERROR;

	memcpy(buf, data + 1, data[0] * sizeof(unsigned char));

	return data[0];
}
