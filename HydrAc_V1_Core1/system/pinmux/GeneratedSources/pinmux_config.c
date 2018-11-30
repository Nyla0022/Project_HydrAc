/*
 **
 ** pinmux_config.c source file generated on December 2, 2015 at 17:44:32.
 **
 ** Copyright (C) 2000-2015 Analog Devices Inc., All Rights Reserved.
 **
 ** This file is generated automatically based upon the options selected in 
 ** the Pin Multiplexing configuration editor. Changes to the Pin Multiplexing
 ** configuration should be made by changing the appropriate options rather
 ** than editing this file.
 **
 ** Selected Peripherals
 ** --------------------
 ** UART0 (CTS, RTS, RX, TX)
 **
 ** GPIO (unavailable)
 ** ------------------
 ** PC13, PC14, PC15, PD00
 */

#include <sys/platform.h>
#include <stdint.h>

#define UART0_CTS_PORTD_MUX  ((uint16_t) ((uint16_t) 0<<0))
#define UART0_RTS_PORTC_MUX  ((uint32_t) ((uint32_t) 0<<30))
#define UART0_RX_PORTC_MUX  ((uint32_t) ((uint32_t) 0<<28))
#define UART0_TX_PORTC_MUX  ((uint32_t) ((uint32_t) 0<<26))

#define UART0_CTS_PORTD_FER  ((uint16_t) ((uint16_t) 1<<0))
#define UART0_RTS_PORTC_FER  ((uint32_t) ((uint32_t) 1<<15))
#define UART0_RX_PORTC_FER  ((uint32_t) ((uint32_t) 1<<14))
#define UART0_TX_PORTC_FER  ((uint32_t) ((uint32_t) 1<<13))

int32_t adi_initpinmux(void);

/*
 * Initialize the Port Control MUX and FER Registers
 */
int32_t adi_initpinmux(void) {
    /* PORTx_MUX registers */
    *pREG_PORTC_MUX = UART0_RTS_PORTC_MUX | UART0_RX_PORTC_MUX
     | UART0_TX_PORTC_MUX;
    *pREG_PORTD_MUX = UART0_CTS_PORTD_MUX;

    /* PORTx_FER registers */
    *pREG_PORTC_FER = UART0_RTS_PORTC_FER | UART0_RX_PORTC_FER
     | UART0_TX_PORTC_FER;
    *pREG_PORTD_FER = UART0_CTS_PORTD_FER;
    return 0;
}

