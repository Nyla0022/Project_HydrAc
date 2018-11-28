/*****************************************************************************
 * UART_Testing_Core1.c
 *****************************************************************************/

#include <sys/platform.h>
#include <sys/adi_core.h>
#include "adi_initialize.h"
#include "UART_Testing_Core1.h"
#include <drivers/uart/adi_uart.h>



#if defined (__ADSPARM__)
/* Some variables in this code sketch are set for demonstration purposes
 * but not used. These variables are expected to be used in real
 * applications.
 */

   #pragma GCC diagnostic ignored "-Wunused-variable"
   #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif


/** 
 * If you want to use command program arguments, then place them in the following string. 
 */
char __argv_string[] = "";


/* flag indicating if UART processing is complete */
static volatile  bool bComplete = false;

/* UART callback */
void UARTCallback(void* pHandle, uint32_t u32Arg, void* pArg)
{
    ADI_UART_HANDLE pDevice = (ADI_UART_HANDLE)pHandle;
    ADI_UART_EVENT event    = (ADI_UART_EVENT)u32Arg;
    uint16_t *data = (uint16_t*)pArg;

    switch (event) {
        case ADI_UART_EVENT_TX_BUFFER_PROCESSED:
           bComplete = true;
           break;

        case ADI_UART_EVENT_OVERRUN_ERROR:
        	bComplete = false;
        	           break;
        case ADI_UART_EVENT_PARITY_ERROR:
        	bComplete = false;
        	           break;
        case ADI_UART_EVENT_FRAMING_ERROR:
        	bComplete = false;
        	           break;
        case ADI_UART_EVENT_BREAK_INTERRUPT:
        	bComplete = false;
        	           break;
        case ADI_UART_EVENT_RX_FIFO_WM:
        	bComplete = false;
        	           break;
        case ADI_UART_EVENT_RECIEVED_ADDR:
        	bComplete = false;
        	           break;
        case ADI_UART_EVENT_AUTOBAUD_COMPLETE:
        	bComplete = false;
        	           break;
        case ADI_UART_EVENT_TX_COMPLETE:
        	bComplete = true;
        	           break;
        case ADI_UART_EVENT_RX_DMA_ERROR:
        	bComplete = false;
        	           break;
        case ADI_UART_EVENT_TX_DMA_ERROR:
        	bComplete = false;
        	           break;
            /* Add code to handle these events */

    default:
        break;
    }
}

/* UART driver memory */
uint8_t driverMemory[ADI_UART_UNIDIR_DMA_MEMORY_SIZE];

int main(int argc, char *argv[])
{
	/**
	 * Initialize managed drivers and/or services that have been added to 
	 * the project.
	 * @return zero on success 
	 */
	adi_initComponents();
	


	  /* UART driver handle */
	    ADI_UART_HANDLE hDevice;

	    /* driver API result code */
	    ADI_UART_RESULT result;

	    /* buffer which holds data to transfer over UART */
	    uint8_t buffer[] = {1, 2, 3};

	    /*test data*/
	    int distance=0, angle=0;
	    int data_to_send = distance * 10000 + angle;

	    /* open the UART driver in Tx mode only */
	    result = adi_uart_Open(0, ADI_UART_DIR_TRANSMIT, driverMemory, ADI_UART_UNIDIR_DMA_MEMORY_SIZE, &hDevice);

	    /**/

	      adi_uart_SetMode();
	      adi_uart_SetBaudRate(9600);
	      adi_uart_SetNumStopBits(1);
	      adi_uart_SetWordLen(1);
	      adi_uart_EnableDMAMode();


	    /* register the callback function */
	    result = adi_uart_RegisterCallback(hDevice, UARTCallback, (void*)0);

	    /* submit the data to the UART device */
	    //result = adi_uart_SubmitTxBuffer(hDevice, buffer, sizeof(buffer));
	    result = adi_uart_SubmitTxBuffer(hDevice, data_to_send, 1);


	    /* enable the UART transfer */
	    result = adi_uart_EnableTx(hDevice, true);

	    while(!bComplete)
	    {
		    printf("UART should be transmitting......");
	    }

	    /* close the UART driver */
	    result = adi_uart_Close(hDevice);

	    return 0;






	}

