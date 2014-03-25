#include <util/delay.h>
#include <avr/io.h>
#include "rfproto.h"

int main(void) {
	char message[] = "hello _+_re!";
	if(!rf_init())
		return 1;
	_delay_ms(1990);
	PORTD = 0x01;
	_delay_ms(10);
	PORTD = 0x00;
	_delay_ms(10);
	PORTD = 0x01;
	_delay_us(100);
	rf_send_block(message, strlen(message));
	return 0;
}

