/*
 * 
 *
 * Created: 2016-05-31 00:23:57
 *  Author: Dennis
 */ 

#include <asf.h>
#include "lcd_driver.h"
#include "hd44780.h"

/************************************************************************/
/* Writes a nibble of data to the screen controller.                    */
/************************************************************************/
void LCD_WriteNibble(uint8_t nibble) {
	ioport_set_pin_level(LCD_EN, (bool) 1);
	ioport_set_pin_level(LCD_D4, (bool)(nibble & (1<<0)));
	ioport_set_pin_level(LCD_D5, (bool)(nibble & (1<<1)));
	ioport_set_pin_level(LCD_D6, (bool)(nibble & (1<<2)));
	ioport_set_pin_level(LCD_D7, (bool)(nibble & (1<<3)));
	ioport_set_pin_level(LCD_EN, (bool) 0);
}

/************************************************************************/
/* Writes data (ASCII-character) to the screen controller.              */
/************************************************************************/
void LCD_WriteData(uint8_t data) {
	ioport_set_pin_level(LCD_RS, true);
	uint8_t lower_nibble = data & 0x0F;
	uint8_t upper_nibble = (data & 0xF0)>>4;
	LCD_WriteNibble(lower_nibble);
	LCD_WriteNibble(upper_nibble);
	delay_us(250);
}

/************************************************************************/
/* Writes a command to the screen controller.                           */
/************************************************************************/
void LCD_WriteCommand(uint8_t command) {
	ioport_set_pin_level(LCD_RS, false);
	uint8_t lower_nibble = command & 0x0F;
	uint8_t upper_nibble = (command & 0xF0)>>4;
	LCD_WriteNibble(lower_nibble);
	LCD_WriteNibble(upper_nibble);
	delay_ms(2);
}

/************************************************************************/
/* Writes a string to the display.                                      */
/************************************************************************/
void LCD_WriteString(char *input) {
	while(*input != '\0') {
		LCD_WriteData((uint8_t)*input);
		input++;
	}
}

/************************************************************************/
/* Initiates the LCD-controller.                                                                     */
/************************************************************************/
void LCD_init(void) {
	ioport_init();
	delay_init(sysclk_get_cpu_hz());
	ioport_set_pin_dir(LCD_D4, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LCD_D5, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LCD_D6, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LCD_D7, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LCD_EN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LCD_RS, IOPORT_DIR_OUTPUT);

	ioport_set_pin_level(LCD_EN, false);
	ioport_set_pin_level(LCD_RS, false);
	
	delay_ms(100);
	
	for(int i = 0; i < 3; i++) {
		LCD_WriteNibble(HD44780_INIT);
		delay_ms(5);
	}
	
	LCD_WriteNibble(HD44780_HOME);
	
	LCD_WriteCommand(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT);
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF);
	LCD_WriteCommand(HD44780_CLEAR);
	LCD_WriteCommand(HD44780_ENTRY_MODE |HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_ON | HD44780_CURSOR_BLINK);
}