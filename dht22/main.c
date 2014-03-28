#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

int main() {
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_puts("wait for data\n");
	DDRB = 0x01;
	PORTB = 0x00;
	DDRC = 0x01;
	PORTC |= 0x01;

	/* ask for data */
	PORTC &= 0xFE;
	_delay_ms(20);
	DDRC &= 0xFE;
	PORTC |= 0x01;
	_delay_us(20);
	/* --- */

	/* wait for data start */
//	PORTB |= 0x01;
	while((PINC & 0x01) == 0)
		_delay_us(10);
//	PORTB &= 0xFE;
	while((PINC & 0x01) == 1)
		_delay_us(10);
	/* --- */

	/* WTF??? */
	while((PINC & 0x01) == 0)
		_delay_us(5);
	while((PINC & 0x01) == 1)
		_delay_us(5);
	/* --- */


	uint8_t data[5];
	for(uint8_t i=0;i<5;i++) {
		data[i] = 0;
		for(uint8_t j=0;j<8;j++) {
			while((PINC & 0x01) == 0)
				_delay_us(5);
			uint8_t counter = 0;
			while((PINC & 0x01) == 1) {
				counter++;
				_delay_us(5);
			}
			data[i] <<= 1;
			if(counter > 7)
				data[i] |= 0x01;
		}
	}

	if(((data[0] + data[1] + data[2] + data[3]) & 0xff) != data[4]) {
		lcd_clrscr();
		lcd_puts("CRC ERROR!");
		return 1;
	}

	char msg[16];
	uint16_t t = ((data[2] & 0x7F) << 8) + data[3];
	uint16_t h = (data[0] << 8) + data[1];
	uint16_t t_int = t / 10;
	uint16_t h_int = h / 10;
	uint16_t t_dec = t - t_int*10;
	uint16_t h_dec = h - h_int*10;
	if(data[2] & 0x80)
		t_dec *= -1;
	lcd_clrscr();
	sprintf(msg, "Temp: %d.%d\nHumidity: %d.%d", t_int, t_dec, h_int, h_dec);
	lcd_puts(msg);
	while(1) {
		PORTB |= 0x01;
//		lcd_puts("buy me");
		_delay_ms(1000);
		PORTB &= 0xFE;
		_delay_ms(1000);
	}
	return 0;
}
