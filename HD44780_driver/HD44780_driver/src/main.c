/**
 * \file
 *
 * \brief Empty user application template
 *
 */

#include <asf.h>
#include "lcd_driver/lcd_driver.h"

int main (void)
{
	sysclk_init();
	board_init();
	LCD_init();
}
