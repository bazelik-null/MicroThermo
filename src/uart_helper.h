//
// Created by niko on 08.02.2026.
//

#ifndef THERMOSTAT_UART_HELPER_H
#define THERMOSTAT_UART_HELPER_H

#define UART_NUM UART_NUM_1
#define BUF_SIZE (1024)

void uart_init();
void uart_write(const char* src);

#endif //THERMOSTAT_UART_HELPER_H