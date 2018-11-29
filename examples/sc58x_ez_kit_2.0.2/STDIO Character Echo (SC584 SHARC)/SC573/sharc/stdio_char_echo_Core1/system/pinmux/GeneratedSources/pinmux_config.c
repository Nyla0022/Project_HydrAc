/*
 **
 ** pinmux_config.c source file generated on June 16, 2016 at 15:41:39.	
 **
 ** Copyright (C) 2000-2016 Analog Devices Inc., All Rights Reserved.
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
 ** PD05, PD06, PF08, PF09
 */

#include <sys/platform.h>
#include <stdint.h>

#define UART0_CTS_PORTD_MUX  ((uint16_t) ((uint16_t) 2<<12))
#define UART0_RTS_PORTD_MUX  ((uint16_t) ((uint16_t) 2<<10))
#define UART0_RX_PORTF_MUX  ((uint32_t) ((uint32_t) 0<<18))
#define UART0_TX_PORTF_MUX  ((uint32_t) ((uint32_t) 0<<16))

#define UART0_CTS_PORTD_FER  ((uint16_t) ((uint16_t) 1<<6))
#define UART0_RTS_PORTD_FER  ((uint16_t) ((uint16_t) 1<<5))
#define UART0_RX_PORTF_FER  ((uint32_t) ((uint32_t) 1<<9))
#define UART0_TX_PORTF_FER  ((uint32_t) ((uint32_t) 1<<8))

int32_t adi_initpinmux(void);

/*
 * Initialize the Port Control MUX and FER Registers
 */
int32_t adi_initpinmux(void) {
    /* PORTx_MUX registers */
    *pREG_PORTD_MUX = UART0_CTS_PORTD_MUX | UART0_RTS_PORTD_MUX;
    *pREG_PORTF_MUX = UART0_RX_PORTF_MUX | UART0_TX_PORTF_MUX;

    /* PORTx_FER registers */
    *pREG_PORTD_FER = UART0_CTS_PORTD_FER | UART0_RTS_PORTD_FER;
    *pREG_PORTF_FER = UART0_RX_PORTF_FER | UART0_TX_PORTF_FER;
    return 0;
}

