//
// Created by niko on 08.02.2026.
//
#include <string.h>

#include <driver/gpio.h>
#include <driver/uart.h>

#include "uart_helper.h"

void uart_init() {
	const uart_config_t uart_config = {
		.baud_rate = 9600,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.rx_flow_ctrl_thresh = UART_HW_FLOWCTRL_DISABLE,
		.source_clk = UART_SCLK_DEFAULT,
		.flags = {
			.allow_pd = 0,             // Set depending on whether you want power down allowed
			.backup_before_sleep = 0   // Set depending on whether you want backup before sleep
		}
	};

	uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
	uart_param_config(UART_NUM, &uart_config);

	uart_set_pin(UART_NUM, GPIO_NUM_17, GPIO_NUM_16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); // TX, RX
}

void uart_write(const char* src)
{
	uart_write_bytes(UART_NUM, src, strlen(src));
}