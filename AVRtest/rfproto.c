/*
 * rfproto.h
 *
 *  Created on: Mar 13, 2014
 *      Author: ray
 */

#include "rfproto.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

int rf_init() {
	DDRD=0x01;
	PORTD=0x00;
	DDRC=0x00;
	PORTC=0x00;
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

unsigned char *_rf_prepare_send_buf(unsigned char *buf, unsigned char len) {
	unsigned char *send_buf = calloc(PACKET_LENGTH, sizeof(unsigned char));

	int i=0;
	while(i<PREAMBULE_LENGTH)
		send_buf[i++] = PREAMBULE_BYTE;
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
	PORTD |= 0x01;
	_delay_us(PULSE_TIME);
	PORTD &= 0xfe;
}

static inline void _send_low() {
	PORTD |= 0x01;
	_delay_us(HALF_PULSE_TIME);
	PORTD &= 0xfe;
	_delay_us(HALF_PULSE_TIME);
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
		_delay_us(BIT_SPLIT_TIME);
	}
}

int rf_send_block(unsigned char *buf, unsigned char len) {
	if(len > PACKET_DATA_LENGTH)
		return RF_ERR_BLOCK_IS_TOO_LONG;

	unsigned char *send_buf = _rf_prepare_send_buf(buf, len);
	_rf_send_data(send_buf);
	free(send_buf);

	return 0;
}


static inline uint8_t _is_level_high() {
	return PINC & 0x01;
}

static inline uint8_t _is_level_low() {
	return (PINC & 0x01) ? 0 : 1;
}

static inline uint8_t _rf_get_next_bit() {
	uint8_t counter;
	for(counter=0;_is_level_low() && counter < PULSE_TIME;counter+=PULSE_JITTER)
		_delay_us(PULSE_JITTER);
	_delay_us(LONG_PULSE_WAIT);
	uint8_t bit = _is_level_high();
	_delay_us(SHORT_PULSE_WAIT);
	return bit;
}

int rf_recv_block(unsigned char *buf, unsigned long timeout) {
	unsigned long counter;
	unsigned char data[PACKET_DATA_LENGTH + 2];
	for(counter=0;_is_level_low() && (timeout == 0 || counter < timeout);counter+=100)
		_delay_us(100);

	if(timeout != 0 && counter >= timeout)
		return -4;

	int current_bit = _rf_get_next_bit();
	if(current_bit < 0)
		return current_bit;
	int preambule_length = 1;
	int next_bit = current_bit;
	do {
		current_bit = next_bit;
		next_bit = _rf_get_next_bit();
		if(next_bit < 0)
			return next_bit;
		preambule_length++;
	} while(current_bit != next_bit);
	if(preambule_length < 10)
		return -1;
	if(current_bit != 1)
		return -1;
	for(uint8_t k=0;k<6;k++) {
		current_bit = _rf_get_next_bit();
		if(current_bit != 1)
			return -1;
	}
	for(uint8_t l=0;l<PACKET_DATA_LENGTH + 2;l++) {
		data[l] = 0;
		for(int p=0;p<8;p++) {
			data[l] <<= 1;
			data[l] |= _rf_get_next_bit();
		}
	}
	if(!is_crc8_ok(data, PACKET_DATA_LENGTH + 1, data[PACKET_DATA_LENGTH + 1]))
		return -2;

	memcpy(buf, data + 1, data[0] * sizeof(unsigned char));

	return data[0];
}
