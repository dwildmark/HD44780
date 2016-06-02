/**
 * \file
 *
 * \brief Empty user application template
 *
 */

#include <asf.h>
#include "lcd_driver/lcd_driver.h"
#include "com_driver/com_driver.h"
#include "lcd_driver/hd44780.h"

int main (void)
{
	sysclk_init();
	board_init();
	ioport_init();
	delay_init(sysclk_get_cpu_hz());
	configure_console();
	LCD_init();
	LCD_sv_mode();
	char str[20] = {0};
	while(1) {
		gets(str);
		LCD_WriteCommand(HD44780_CLEAR);
		LCD_WriteString(str);
	}
}
