/*
 * lcd_driver.h
 *
 * Created: 2016-05-31 00:24:09
 *  Author: Dennis
 */ 


#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

#define LCD_RS		PIO_PD2_IDX
#define LCD_EN		PIO_PD1_IDX
#define LCD_D4		PIO_PD0_IDX
#define LCD_D5		PIO_PA15_IDX
#define LCD_D6		PIO_PA14_IDX
#define LCD_D7		PIO_PB26_IDX

void LCD_WriteNibble(uint8_t nibble);
void LCD_WriteData(uint8_t data);
void LCD_WriteCommand(uint8_t command);
void LCD_WriteString(char *input);
void LCD_init(void);

#endif /* LCD_DRIVER_H_ */