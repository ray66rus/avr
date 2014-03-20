/*
 * rfproto.h
 *
 *  Created on: Mar 13, 2014
 *      Author: ray
 */

#ifndef RFPROTO_H_
#define RFPROTO_H_

#define PREAMBULE_LENGTH 3
#define PACKET_DATA_LENGTH 14
#define PACKET_LENGTH 20 /* PREAMBULE_LENGTH + 1 (SYNC BYTE) + 1 (DATA LENGTH) + PACKET_DATA_LENGTH + 1 (CRC) */
#define PREAMBULE_BYTE 0xAA
#define SYNC_BYTE 0xFF

#define HALF_PULSE_TIME 100 /* half of pulse length in us */
#define PULSE_TIME 200
#define LONG_PULSE_WAIT 140 /* HALF_PULSE_TIME + 2 * PULSE_JITTER_TIME */
#define SHORT_PULSE_WAIT 60 /* HALF_PULSE_TIME - 2 * PULSE_JITTER_TIME */
#define BIT_SPLIT_TIME 70
#define PULSE_JITTER 20

#define RF_ERR_OK 0
#define RF_ERR_BLOCK_IS_TOO_LONG -1
#define RF_ERR_RECV_TOUT -2
#define RF_ERR_PROTOCOL_ERROR -3
#define RF_ERR_CRC_ERROR -4

unsigned char calc_crc8(unsigned char *buf, unsigned int len);
unsigned char is_crc8_ok(unsigned char *buf, unsigned int len, unsigned char crc8);
int rf_send_block(unsigned char *buf, unsigned char len);
int rf_init();
int rf_recv_block(unsigned char *buf, unsigned long timeout);

#endif /* RFPROTO_H_ */
