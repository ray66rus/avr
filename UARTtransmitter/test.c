#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "rfproto.h"

#define F_CPU 1000000UL
#define UART_BAUD_RATE		1200

void main(void) {
	char message[] = "hello _+_re!";
/*	int message_len = strlen(message);

	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	sei();
	unsigned char crc8 = calc_crc8(message, message_len);
	for(int i=0;i<message_len;i++) {
		uart_putc(message[i]);
	}
	uart_putc(crc8);
	_delay_ms(5000);*/
	rf_init();
	rf_send_block(message, strlen(message));
}

