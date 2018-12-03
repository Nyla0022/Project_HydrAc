/*
 *  (C) Copyright 2016 Analog Devices, Inc. All Rights Reserved.
 *
 * This software is proprietary and confidential.  By using this software
 * you agree to the terms of the associated Analog Devices License Agreement.
 *
 * For details on how to use a terminal to display UART output, see the
 * example Readme file.
 */

#include <services/pwr/adi_pwr.h>
#include "ThreeCore_Core0.h"
#include "uart.h"
#include <stddef.h>

#define BAUD_RATE                 9600u
#define UART_DEVICE_NUM           0u
/* default frequency settings */
#define MHZTOHZ                   1000000u
#define CLKIN                     (25u  * MHZTOHZ)

char uart_debug_buffer[UART_DEBUG_BUFFER_LINE_SIZE];
char uart_debug_buffer_send[UART_DEBUG_BUFFER_LINE_SIZE];

/* UART Handle */
static ADI_UART_HANDLE  ghUART = NULL;

/* Memory required for operating UART in interrupt mode */
static uint8_t  gUARTMemory[ADI_UART_UNIDIR_DMA_MEMORY_SIZE];

/*
 *   Function:    init_uart
 *   Description: Initialize the UART.
 */
ADI_UART_RESULT init_uart(void)
{
    /* UART return code */
    ADI_UART_RESULT    eResult;

    /* Initialize Power service */
    if(adi_pwr_Init(0u, CLKIN) != ADI_PWR_SUCCESS)
    {
        return ADI_UART_FAILURE;
    }

    /* Open UART driver */
    eResult = adi_uart_Open(UART_DEVICE_NUM,
                            ADI_UART_DIR_TRANSMIT,
                            gUARTMemory,
                            ADI_UART_UNIDIR_DMA_MEMORY_SIZE,
                            &ghUART);
    if (eResult != ADI_UART_SUCCESS)
    {
        return eResult;
    }

    /* Set the UART Mode */
    eResult = adi_uart_SetMode(ghUART, ADI_UART_MODE_UART);
    if (eResult != ADI_UART_SUCCESS)
    {
        return eResult;
    }

    /* Set UART Baud Rate */
    eResult = adi_uart_SetBaudRate(ghUART, BAUD_RATE);
    if (eResult != ADI_UART_SUCCESS)
    {
        return eResult;
    }

    /* Set number of stop bits */
    eResult = adi_uart_SetNumStopBits(ghUART, ADI_UART_ONE_STOPBIT);
    if (eResult != ADI_UART_SUCCESS)
    {
        return eResult;
    }

    /* Set number of stop bits */
    return adi_uart_SetWordLen(ghUART, ADI_UART_WORDLEN_8BITS);
}


/*
 *   Function:    uart_debug_print
 *   Description: Prints debug info over the UART using a predefined
 *                 buffer.
 */
ADI_UART_RESULT uart_debug_print(void)
{
    uint32_t i = 0;
    uint32_t j = 0;

    while ((j < UART_DEBUG_BUFFER_LINE_SIZE) && (uart_debug_buffer[i] != '\0'))
    {
        char c = uart_debug_buffer[i++];
        uart_debug_buffer_send[j++] = c;
        if ( c == '\n' )
        {
            uart_debug_buffer_send[j++] = '\r';
        }
    }

    return adi_uart_Write(ghUART, &uart_debug_buffer_send, j);
}
