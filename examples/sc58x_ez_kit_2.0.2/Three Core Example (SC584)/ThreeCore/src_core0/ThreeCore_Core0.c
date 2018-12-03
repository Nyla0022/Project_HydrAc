/*****************************************************************************
 * File:    ThreeCore_Core0.c
 *
 * Copyright(c) 2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

/*****************************************************************************
 * Please see the Readme for details of how to use CLDP to flash this example.
 *****************************************************************************/

/*****************************************************************************
 * H E A D E R   F I L E S
 *****************************************************************************/

#include <sys/platform.h>
#include <sys/adi_core.h>
#include "adi_initialize.h"
#include "ThreeCore_Core0.h"
#include <services/gpio/adi_gpio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#if defined(IO_TO_CONSOLE) || defined(IO_TO_UART)
   #include <stdio.h>
#endif

#include "../SoftConfig.h"

/*****************************************************************************
 * I N I T I A L I Z A T I O N    R O U T I N E S
 *****************************************************************************/

/**
 * Initialize the LED that we use for Core 0
 */
static bool initialize_led(void)
{
    return adi_gpio_SetDirection(CORE0_LED_PORT, CORE0_LED_PIN, ADI_GPIO_DIRECTION_OUTPUT);
}

static volatile bool buttonPressed = false;

/**
 * Callback when we press the push button.
 */
static void pintCallback(ADI_GPIO_PIN_INTERRUPT ePinInt, uint32_t PinIntData,  void *pCBParam)
{
    buttonPressed = true;
}

/**
 * Initialize the Push Button that we use for Core 0
 */
static bool initialize_push_button(void)
{
      static uint8_t gpioMemory[ADI_GPIO_CALLBACK_MEM_SIZE];
      uint32_t gpioMaxCallbacks;

      /* Initialize the GPIO service */
      ADI_GPIO_RESULT result = adi_gpio_Init((void*)gpioMemory, ADI_GPIO_CALLBACK_MEM_SIZE, &gpioMaxCallbacks);

      if (result == ADI_GPIO_SUCCESS)
      {
          result = adi_gpio_SetDirection(CORE0_PB_PORT, CORE0_PB_PORT_PIN, ADI_GPIO_DIRECTION_INPUT);
      }
      if (result == ADI_GPIO_SUCCESS)
      {
          /* Set edge sense mode  */
          result = adi_gpio_SetPinIntEdgeSense(CORE0_PB_PINT, CORE0_PB_PINT_PIN, ADI_GPIO_SENSE_RISING_EDGE);
      }
      if (result == ADI_GPIO_SUCCESS)
      {
          /* Register pinint callback */
          result = adi_gpio_RegisterCallback(CORE0_PB_PINT, CORE0_PB_PINT_PIN, pintCallback, (void*)0);
      }
      if (result == ADI_GPIO_SUCCESS)
      {
          /* Assign pin interrupt  */
          result = adi_gpio_PinInterruptAssignment(CORE0_PB_PINT, CORE0_PIN_ASSIGN_BYTE, CORE0_PIN_ASSIGN);
      }
      if (result == ADI_GPIO_SUCCESS)
      {
          /* Set pin interrupt mask */
          result = adi_gpio_EnablePinInterruptMask(CORE0_PB_PINT, CORE0_PB_PINT_PIN, true);
      }
      return result;
}

/*****************************************************************************
 * O U T P U T    R O U T I N E S
 *****************************************************************************/

/**
 * Print routines to print output string to console or over UART.
 */
static void print_core_guts(int core, char *fmt, va_list ap)
{
#ifdef IO_TO_CONSOLE
    printf("[CORE %d]: ", core);
    vprintf(fmt, ap);
#elif defined(IO_TO_UART)
    int outLen = snprintf(uart_debug_buffer, UART_DEBUG_BUFFER_LINE_SIZE, "[CORE %d]: ", core);
    uart_debug_print();
    outLen = vsnprintf(uart_debug_buffer, UART_DEBUG_BUFFER_LINE_SIZE, fmt, ap);
    uart_debug_print();
    if (outLen >= UART_DEBUG_BUFFER_LINE_SIZE) {
      snprintf(uart_debug_buffer, UART_DEBUG_BUFFER_LINE_SIZE, "\n  ...(output truncated)...\n");
      uart_debug_print();
    }
#else
    /* no output */
#endif
}

void print_core(int core, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    print_core_guts(core, fmt, ap);
}

void print(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    print_core_guts(adi_core_id(), fmt, ap);
}

/*****************************************************************************
 * A P P L I C A T I O N   R O U T I N E S
 *****************************************************************************/

/**
 * Function that delays for approximately 1ms
 */
static void delay(const uint32_t iSpeed)
{
    uint32_t x, y;
    uint32_t counter = 125000u;  /* cycles for 1ms, approx. 4 cycles per loop */

    for( x = 0u; x < iSpeed; x++ )
    {
        for( y = 0u; y < counter; y++ )
        {
            asm volatile("nop;");
        }
    }
}

/**
 * Blink the LED while we wait for Push Button to be pressed.
 * If Push Button is not pressed carry on regardless.
 */
static void blink_led_and_wait_for_button(void)
{
    uint32_t count = 50u;
    buttonPressed = false;
    print("(%s should now be blinking!)\n", CORE0_LED);
    while ((!buttonPressed) && (count > 0u))
    {
        adi_gpio_Toggle(CORE0_LED_PORT, CORE0_LED_PIN);
        delay(500u);
        count--;
    }
    adi_gpio_Set(CORE0_LED_PORT, CORE0_LED_PIN);
    print(buttonPressed ? "%s pressed\n" : "Timeout waiting for %s\n", CORE0_PB);
}

/**
 * Main function.
 * - Start Cores 1 and 2.
 * - Flash LED to show Core 0 is running, and wait for Push Button press.
 * - Send message to Core 1, to tell it to blink its LED.
 * - Await confirmation that Core 1 received that message and acted on it.
 * - Flash LED to show success and wait for Push Button press.
 * - Send message to Core 2, to tell it to blink its LED.
 * - Await confirmation that Core 2 received that message and acted on it.
 * - Flash LED to show success and wait for Push Button press.
 */
int main(void)
{
    /**
     * Initialize managed drivers and/or services that have been added to 
     * the project.
     * @return zero on success 
     */
    adi_initComponents();
    
    /* Configure the soft switches for the EZ-Kit.
    */
    ConfigSoftSwitches();

#ifdef IO_TO_UART
    init_uart();
#endif

    print("Starting test\n");

    /* Enable Cores 1 and 2.
    */
    adi_core_enable(ADI_CORE_SHARC0);
    adi_core_enable(ADI_CORE_SHARC1);

    /* Initialize Push Button and LED, and start MCAPI communications with
    ** other cores.
    */
    if (initialize_push_button() != ADI_GPIO_SUCCESS)
    {
        print("Failed to initialize %s\n", CORE0_PB);
    }
    if (initialize_led() != ADI_GPIO_SUCCESS)
    {
        print("Failed to initialize %s\n", CORE1_LED);
    }
    initialize_comms(PORT_COMMS);

    /* Print instructions for next stage.
    */
    print("%s blinking - Core 0 is running.\n", CORE0_LED);
    print("Please press %s to send a message to Core 1.\n", CORE0_PB);
    print("When Core 1 has received that message, %s will blink.\n", CORE1_LED);

    /* Flash LED to show Core 0 is running, and wait for Push Button press. 
    */
    blink_led_and_wait_for_button();

    /* Tell Core 1 to do something.
    */
    send_command(ADI_CORE_SHARC0, CMD_EXECUTE);

    /* Wait to hear it has complete.
    */
    wait_for_command(CMD_RESPONSE);

    /* Print instructions for next stage.
    */
    print("Received confirmation of message received from Core 1.\n");
    print("Please press %s to send a message to Core 2.\n", CORE0_PB);
    print("When Core 2 has received that message, %s will blink.\n", CORE2_LED);

    /* Flash LED to show success, and wait for Push Button press. 
    */
    blink_led_and_wait_for_button();

    /* Tell Core 2 to do something.
    */
    send_command(ADI_CORE_SHARC1, CMD_EXECUTE);

    /* Wait to hear it has complete.
    */
    wait_for_command(CMD_RESPONSE);

    /* Print instructions for next stage.
    */
    print("Received confirmation of message received from Core 2.\n");
    print("Press %s to continue\n", CORE0_PB);

    /* Wait for one last Button press before we shut down.
    */
    blink_led_and_wait_for_button();

    /* Switch off the lights and shut down.
    */
    adi_gpio_Clear(CORE0_LED_PORT, CORE0_LED_PIN);
    shut_down_comms();

    print("All done!\n");

    return 0;
}

