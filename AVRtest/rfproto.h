/*
 * rfproto.h
 *
 *  Created on: Mar 13, 2014
 *      Author: ray
 */

#ifndef RFPROTO_H_
#define RFPROTO_H_

#define PREAMBLE_LENGTH 3
#define PACKET_DATA_LENGTH 14
#define PACKET_DATA_LENGTH_WITH_SIZE_AND_CRC 16 /* PACKET_DATA_LENGTH + 2 */
#define PACKET_LENGTH 20 /* PREAMBLE_LENGTH + 1 (SYNC BYTE) + 1 (DATA SIZE) + PACKET_DATA_LENGTH + 1 (CRC) */
#define PREAMBLE_BYTE 0xAA
#define SYNC_BYTE 0xFF

#define BIT_SPLIT_TIME 50
#define PULSE_TIME_HIGH 100
#define PULSE_TIME_LOW 40
#define ATOMIC_DELAY 5
#define ATOMIC_DELAYS_IN_TIME_LOW 10 /* PULSE_TIME_LOW / ATOMIC_DELAY + 2 */
#define MAX_DATA_RECV_TIME_MS 20 /* ((BIT_SPLIT_TIME + PULSE_TIME_HIGH * 8) * PACKET_DATA_LENGTH_WITH_SIZE_AND_CRC) div 1000 + 1 */

#define RF_ERR_OK 0
#define RF_ERR_BLOCK_IS_TOO_LONG -1
#define RF_ERR_TOUT -2
#define RF_ERR_CRC_ERROR -3

#include <avr/io.h>

unsigned char calc_crc8(unsigned char *buf, unsigned int len);
unsigned char is_crc8_ok(unsigned char *buf, unsigned int len, unsigned char crc8);
int rf_send_block(unsigned char *buf, unsigned char len);
int rf_init();
int rf_recv_block(unsigned char *buf, uint16_t timeout);

#endif /* RFPROTO_H_ */
