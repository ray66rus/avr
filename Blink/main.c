#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "macros.h"

#define PORT PORTB
#define LED_PIN 0

int main() {

	DDR(PORT) |= _BV(LED_PIN);
	while(1) {
		PORT |= _BV(LED_PIN);
		_delay_ms(1000);
		PORT &= ~_BV(LED_PIN);
		_delay_ms(1000);
	}

	return 0;
}
