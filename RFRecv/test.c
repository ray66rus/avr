#include "lcd.h"
#include "rfproto.h"
#include <util/delay.h>
#include <avr/io.h>

void main(void) {
	char message[15];

    lcd_init(LCD_DISP_ON);
    lcd_puts("start\n");
	rf_init();
    int q = rf_recv_block(message, 2000);
    if(q == RF_ERR_TOUT) {
    	lcd_puts("TIMEOUT!");
    	return -1;
    } else if(q == RF_ERR_CRC_ERROR) {
    	lcd_puts("CRC ERROR!");
    	return -1;
    }
    message[q] = 0;
    lcd_puts(message);
    return 0;
}

