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
/* Pulses EN-pin to enable transmit                                     */
/************************************************************************/
void LCD_PulseEnable(void) {
	ioport_set_pin_level(LCD_EN, false);
	delay_us(1);
	ioport_set_pin_level(LCD_EN, true);
	delay_us(1);
	ioport_set_pin_level(LCD_EN, false);
	delay_us(100);
}

/************************************************************************/
/* Writes a nibble of data to the screen controller.                    */
/************************************************************************/
void LCD_WriteNibble(uint8_t nibble) {
	ioport_set_pin_level(LCD_D4, (nibble & (0x01<<0)));
	ioport_set_pin_level(LCD_D5, (nibble & (0x01<<1)));
	ioport_set_pin_level(LCD_D6, (nibble & (0x01<<2)));
	ioport_set_pin_level(LCD_D7, (nibble & (0x01<<3)));
	LCD_PulseEnable();
}

/************************************************************************/
/* Writes data (ASCII-character) to the screen controller.              */
/************************************************************************/
void LCD_WriteData(uint8_t data) {
	ioport_set_pin_level(LCD_RS, true);
	uint8_t lower_nibble = data & 0x0F;
	uint8_t upper_nibble = (data & 0xF0)>>4;
	LCD_WriteNibble(upper_nibble);
	LCD_WriteNibble(lower_nibble);
	delay_us(250);
}

/************************************************************************/
/* Writes a command to the screen controller.                           */
/************************************************************************/
void LCD_WriteCommand(uint8_t command) {
	ioport_set_pin_level(LCD_RS, false);
	uint8_t lower_nibble = command & 0x0F;
	uint8_t upper_nibble = (command & 0xF0)>>4;
	LCD_WriteNibble(upper_nibble);
	LCD_WriteNibble(lower_nibble);
	delay_ms(2);
}

/************************************************************************/
/* Writes a string to the display.                                      */
/************************************************************************/
void LCD_WriteString(char *input) {
	char str[20] = {0};
	while(*input != '\0') {
		LCD_WriteData(char_convert_sv(*input));
		itoa(*input, str, 10);
		printf(str);
		input++;
	}
}

/************************************************************************/
/* Initiates the LCD-controller.                                                                     */
/************************************************************************/
void LCD_init(void) {
	delay_ms(50);
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

/************************************************************************/
/* Adds a special character to the register.                            */
/************************************************************************/

void LCD_AddCharacter(uint8_t location, uint8_t charmap[]) {
	location &= 0x7;
	LCD_WriteCommand(HD44780_CGRAM_SET | (location << 3));
	for(int i = 0; i < 8; i++) {
			LCD_WriteData(charmap[i]);
	}
	LCD_WriteCommand(HD44780_HOME);
}

uint8_t char_convert_sv(uint8_t character) {
	switch(character) {
		case 0xF6:
			return 0xEF;
		case 0xE4:
			return 0xE1;
		case 0xE5:
			return 0x00;
		case 0xC5:
			return 0x01;
		case 0xC4:
			return 0x02;
		case 0xD6:
			return 0x03;
		default:
			return character;
	}
}

void LCD_sv_mode(void) {
	uint8_t char_aa[8] = {0x4, 0x0, 0xe, 0x1, 0xf, 0x11, 0xf};
	uint8_t char_AA[8] = {0x4, 0x0, 0xe, 0x11, 0x1f, 0x11, 0x11};
	uint8_t char_AE[8] = {0xa, 0x0, 0xe, 0x11, 0x1f, 0x11, 0x11};
	uint8_t char_OE[8] = {0xa, 0xe, 0x11, 0x11, 0x11, 0x11, 0xe};
	LCD_AddCharacter(0, char_aa);
	LCD_AddCharacter(1, char_AA);
	LCD_AddCharacter(2, char_AE);
	LCD_AddCharacter(3, char_OE);
}