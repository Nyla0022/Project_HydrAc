/*
 **
 ** pinmux_config.c source file generated on May 5, 2016 at 09:28:49.	
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
 ** UART0 (RX, TX)
 **
 ** GPIO (unavailable)
 ** ------------------
 ** PF08, PF09
 */

#include <sys/platform.h>
#include <stdint.h>

#define UART0_RX_PORTF_MUX  ((uint32_t) ((uint32_t) 0<<18))
#define UART0_TX_PORTF_MUX  ((uint32_t) ((uint32_t) 0<<16))

#define UART0_RX_PORTF_FER  ((uint32_t) ((uint32_t) 1<<9))
#define UART0_TX_PORTF_FER  ((uint32_t) ((uint32_t) 1<<8))

int32_t adi_initpinmux(void);

/*
 * Initialize the Port Control MUX and FER Registers
 */
int32_t adi_initpinmux(void) {
    /* PORTx_MUX registers */
    *pREG_PORTF_MUX = UART0_RX_PORTF_MUX | UART0_TX_PORTF_MUX;

    /* PORTx_FER registers */
    *pREG_PORTF_FER = UART0_RX_PORTF_FER | UART0_TX_PORTF_FER;
    return 0;
}

