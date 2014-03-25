#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "uart.h"

#define F_CPU 1000000UL
#define UART_BAUD_RATE      1200

void main(void) {
}
/*	char message[] = "hello there!";
	int message_len = strlen(message);

	char recv_message[20];

	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	sei();
	uart_puts(message);

	lcd_init(LCD_DISP_ON);
    lcd_clrscr();
	lcd_puts("USART RX/TX\n");
    lcd_puts("test (1)");
	_delay_ms(1000);
	lcd_clrscr();

	unsigned int pointer = 0;
	while(message_len >= 0) {
		unsigned int c = uart_getc();
		if(c & UART_NO_DATA) {
			lcd_clrscr();
			lcd_puts("NO DATA!");
			_delay_ms(1000);
			lcd_clrscr();
		} else if(c & UART_FRAME_ERROR) {
			lcd_clrscr();
			lcd_puts("FRAME ERROR!");
			_delay_ms(1000);
			lcd_clrscr();
		} else if(c & UART_OVERRUN_ERROR) {
			lcd_clrscr();
			lcd_puts("OVERRUN ERROR!");
			_delay_ms(1000);
			lcd_clrscr();
        } else if(c & UART_BUFFER_OVERFLOW) {
			lcd_clrscr();
			lcd_puts("BUFFER OVERFLOW!");
			_delay_ms(1000);
			lcd_clrscr();
        } else {
			recv_message[pointer++] = c;
			message_len--;
        }
	}*/

/*	unsigned char recv_crc = recv_message[pointer-1];
	recv_message[pointer-1] = 0;

	if(message_len <= 0)
		lcd_puts(recv_message);

	_delay_ms(1000);
	lcd_clrscr();
	if(is_crc8_ok(recv_message, pointer-1, recv_crc))
		lcd_puts("crc ok!");
	else
		lcd_puts("crc not ok!");*/

/*
static const PROGMEM unsigned char copyRightChar[] =
{
	0x07, 0x08, 0x13, 0x14, 0x14, 0x13, 0x08, 0x07,
	0x00, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x00
};



int main(void)
{
    char buffer[7];
    int  num=134;
    unsigned char i;


    DDRD &=~ (1 << PD2);
    PORTD |= (1 << PD2);



    lcd_init(LCD_DISP_ON);

    for (;;) {
        lcd_gotoxy(7,1);
        lcd_putc(':');
        lcd_command(LCD_DISP_ON_CURSOR);
        lcd_puts( "CurOn");
        lcd_clrscr();
        lcd_puts_P( "Line 1 longer than 14 characters\n" );
        lcd_puts_P( "Line 2 longer than 14 characters" );
        lcd_command(LCD_MOVE_DISP_LEFT);
        lcd_command(LCD_DISP_ON);
       lcd_puts("Copyright: ");
       lcd_command(_BV(LCD_CGRAM));
       for(i=0; i<16; i++)
       {
           lcd_data(pgm_read_byte_near(&copyRightChar[i]));
       }

       lcd_gotoxy(0,1);

       lcd_putc(0);
       lcd_putc(1);


    }
}*/

