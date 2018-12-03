/*
 * (C) Copyright 2016 - Analog Devices, Inc.  All rights reserved.
 *
 * FILE:     uart.h
 *
 * CHANGES:  1.00.0  - initial release
 */

#ifndef UART_H
#define UART_H

#include <drivers/uart/adi_uart.h>

/* APIs used by the test code */
ADI_UART_RESULT init_uart(void);
ADI_UART_RESULT uart_debug_print(void);

extern char uart_debug_buffer[];

#endif/* UART_H */
