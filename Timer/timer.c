#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "lcd.h"

uint8_t s, m, h;

ISR(TIMER1_COMPA_vect) {
	s++;
	if(s >= 60) {
		s -= 60;
		m++;
	}
	if(m >= 60) {
		m -= 60;
		h++;
	}
	if(h >= 24)
		h -= 24;
}

int main(void) {
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);
	lcd_puts("loading...");
	_delay_ms(1500);
	lcd_init(LCD_DISP_ON);
	
	/* timer setup */
	TCCR1A = 0; /* first timer control register */ 
	TCCR1B = 0b1101; /* second timer control register */
	TCNT1 = 0;  /* initial counter state */
	OCR1A = 976;  /* max counter */
	TIMSK = 0b10000; /* timer interrupt mask */

	sei();
	
	while(1) {
		char time[9];
		lcd_gotoxy(0,0);
		sprintf(time, "%02d:%02d:%02d", h, m, s);
		lcd_puts(time);
		set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_mode();
	}
	
	return 0;
}