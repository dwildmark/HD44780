/*
 * com_driver.c
 *
 * Created: 2016-06-02 01:15:56
 *  Author: Dennis
 */ 
#include <asf.h>

void configure_console(void)
/* Enables serial communication via the USB interface. */
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}