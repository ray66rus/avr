#include "lcd.h"
#include "rfproto.h"
#include <util/delay.h>
#include <avr/io.h>

void main(void) {
	char message[15];

    lcd_init(LCD_DISP_ON);
    lcd_puts("start\n");
	rf_init();
    int q = rf_recv_block(message, 1000000);
    char err[10];
    sprintf(err, "%d:", q);
    lcd_puts(err);
    message[12] = 0;

    lcd_puts(message);
}

