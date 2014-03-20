#include <util/delay.h>
#include <avr/io.h>
#include "rfproto.h"

void main(void) {
	char message[] = "hello _+_re!";
	if(!rf_init())
		return 1;
	_delay_ms(10);
	rf_send_block(message, strlen(message));
	return 0;
}

