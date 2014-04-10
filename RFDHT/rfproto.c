/*
 * rfproto.h
 *
 *  Created on: Mar 13, 2014
 *      Author: ray
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "rfproto.h"
#include "macros.h"

int rf_init() {
	DDR(TX_PORT) |= _BV(TX_PIN);
	TX_PORT &= ~_BV(TX_PIN);
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

unsigned char *_rf_prepare_send_buf(unsigned char *buf, unsigned char len) {
	unsigned char *send_buf = calloc(PACKET_LENGTH, sizeof(unsigned char));

	int i=0;
	while(i<PREAMBLE_LENGTH)
		send_buf[i++] = PREAMBLE_BYTE;
	send_buf[i++] = SYNC_BYTE;

	send_buf[i++] = len;

	int j = 0;
	while(j < len)
		send_buf[i++] = buf[j++];
	i += PACKET_DATA_LENGTH - j;

	unsigned char crc = calc_crc8(send_buf + (i - (PACKET_DATA_LENGTH+1)), PACKET_DATA_LENGTH+1);
	send_buf[i] = crc;

	return send_buf;
}

static inline void _send_high() {
	TX_PORT |= _BV(TX_PIN);
	_delay_us(BIT_SPLIT_TIME);
	TX_PORT &= ~_BV(TX_PIN);
	_delay_us(PULSE_TIME_HIGH);
	TX_PORT |= _BV(TX_PIN);
}

static inline void _send_low() {
	TX_PORT |= _BV(TX_PIN);
	_delay_us(BIT_SPLIT_TIME);
	TX_PORT &= ~_BV(TX_PIN);
	_delay_us(PULSE_TIME_LOW);
	TX_PORT |= _BV(TX_PIN);
}

void _rf_send_data(unsigned char *send_buf) {
	unsigned char bits[PACKET_LENGTH*8];
	int i,j;
	for(i=0;i<PACKET_LENGTH;i++) {
		uint8_t bit_index = (i+1)*8-1;
		for(j=7; j>=0; j--)
			bits[bit_index-j] = (send_buf[i] >> j) & 0x01;
	}
	for(i=0;i<PACKET_LENGTH*8;i++) {
		if(bits[i] == 0)
			_send_low();
		else
			_send_high();
	}
	_delay_us(FINILIZE_PULSE_TIME);
	TX_PORT &= ~_BV(TX_PIN);
}

int rf_send_block(unsigned char *buf, unsigned char len) {
	if(len > PACKET_DATA_LENGTH)
		return RF_ERR_BLOCK_IS_TOO_LONG;

	unsigned char *send_buf = _rf_prepare_send_buf(buf, len);
	_rf_send_data(send_buf);
	free(send_buf);

	return 0;
}
