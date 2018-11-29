/*********************************************************************************

Copyright(c) 2012-2016 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
 * @file      char_echo.c
 *
 * @brief     This is the primary source file for UART character echo example.
 *
 */

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_14_7:"main does not have a single return")
#pragma diag(suppress:misra_rule_19_4:"REPORT_ERROR defined to printf")
#pragma diag(suppress:misra_rule_20_9:"printf used for error reporting")
#endif /* _MISRA_RULES */

#include "char_echo.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <services/int/adi_int.h>
#include <drivers/uart/adi_uart.h>
#include <services/pwr/adi_pwr.h>

/* Rx/Tx Buffer size */
#define BUFFER_SIZE     2

/* ADI initialization header */
#include "adi_initialize.h"

/* ConfigSoftSwitches() sets the software-controlled switches. */
//#include "SoftConfig_SC584.c"


extern void ConfigSoftSwitches(void);


int main(void)
{
    /* UART Handle */
    static ADI_UART_HANDLE ghUART;

    /* Rx and Tx buffer */
    static uint8_t RxBuffer[BUFFER_SIZE];

    /* Memory required for operating UART in interrupt mode */
    static uint8_t gUARTMemory[ADI_UART_BIDIR_INT_MEMORY_SIZE];

    /* UART return code */
    ADI_UART_RESULT    eResult;

    /* Tx size */
    uint32_t           nTxSize = 1u;

    /* Flag which indicates whether to stop the program */
    bool bStopFlag = false;

    adi_initComponents(); /* auto-generated code */

    printf("UART Char echo blocking mode example\n");

    /* Set the Software controlled switches to default values */
    ConfigSoftSwitches();

    if(adi_pwr_Init(0u, CLKIN) != ADI_PWR_SUCCESS)
    {
		REPORT_ERROR("Failed to initialize power service\n");
		return FAILED;
    }

    /*
     * Initialize UART
     */
    /* Open UART driver */
    eResult = adi_uart_Open(UART_DEVICE_NUM,
                            ADI_UART_DIR_BIDIRECTION,
                            gUARTMemory,
                            ADI_UART_BIDIR_INT_MEMORY_SIZE,
                            &ghUART);
    if (eResult != ADI_UART_SUCCESS)
    {
        REPORT_ERROR("Could not open UART Device 0x%08X\n", eResult);
        return FAILED;
    }

    /* Set the UART Mode */
    eResult = adi_uart_SetMode(ghUART, ADI_UART_MODE_UART);
    if (eResult != ADI_UART_SUCCESS)
    {
        REPORT_ERROR("Could not set the Mode 0x%08X\n", eResult);
        return FAILED;
    }

    /* Set UART Baud Rate */
    eResult = adi_uart_SetBaudRate(ghUART, BAUD_RATE);
    if (eResult != ADI_UART_SUCCESS)
    {
        REPORT_ERROR("Could not set the Baud Rate 0x%08X\n", eResult);
        return FAILED;
    }

    /* Set number of stop bits */
    eResult = adi_uart_SetNumStopBits(ghUART, ADI_UART_ONE_STOPBIT);
    if (eResult != ADI_UART_SUCCESS)
    {
        REPORT_ERROR("Could not set the stop bits 0x%08X\n", eResult);
        return FAILED;
    }

    /* Set number of stop bits */
    eResult = adi_uart_SetWordLen(ghUART, ADI_UART_WORDLEN_8BITS);
    if (eResult != ADI_UART_SUCCESS)
    {
        REPORT_ERROR("Could not set word length 0x%08X\n", eResult);
        return FAILED;
    }

    printf("Set up a serial terminal application as described in Readme file.\n"
           "Press any key on the keyboard and notice the character being echoed to the serial terminal console.\n"
           "\n Press the return key to stop the program.\n");

    *RxBuffer = 9;
    /* UART processing loop */
    while(bStopFlag == false)
    {
        /* Write back the character */
        eResult = adi_uart_Write(ghUART, &RxBuffer[0], nTxSize);

        printf("received: %s\n", RxBuffer);
        if (eResult != ADI_UART_SUCCESS)
        {
            REPORT_ERROR("Could not do a write 0x%08X\n", eResult);
            return FAILED;
        }

//                /* Read a character */
//                eResult = adi_uart_Read(ghUART, &RxBuffer[0], 1u);
//                if (eResult != ADI_UART_SUCCESS)
//                {
//                    REPORT_ERROR("Could not do a read 0x%08X\n", eResult);
//                    return FAILED;
//                }
//
//                /* If return character is pressed, write back\n character along with \r */
//                if(RxBuffer[0] == (uint8_t)'\r')
//                {
//                    RxBuffer[1] = (uint8_t)'\n';
//
//                    /* Transmit two characters in this case */
//                    nTxSize     = 2u;
//
//                    /* Stop the program upon receiving carriage return */
//                    bStopFlag = true;
//                }
     }

    /* Close the UART */
    eResult = adi_uart_Close(ghUART);
    if (eResult != ADI_UART_SUCCESS)
    {
        REPORT_ERROR("Could not close UART driver 0x%08X\n", eResult);
        return FAILED;
    }

    return PASSED;
}

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */
