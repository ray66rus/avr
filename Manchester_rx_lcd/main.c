#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "Manchester.h"


int main(void) {
	lcd_init(LCD_DISP_ON);
	while(1) {
		lcd_clrscr();
		lcd_puts("USART RX/TX\n");
		lcd_puts("test (1)");
		_delay_ms(2000);
		lcd_clrscr();
		_delay_ms(2000);
	}
	return 0;
}
