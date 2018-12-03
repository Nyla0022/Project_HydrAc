/*****************************************************************************
 * File:    ThreeCore_Core2.c
 *
 * Copyright(c) 2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

/*****************************************************************************
 * H E A D E R   F I L E S
 *****************************************************************************/

#include <sys/platform.h>
#include <sys/adi_core.h>
#include "adi_initialize.h"
#include "ThreeCore_Core2.h"
#include <services/gpio/adi_gpio.h>
#include <stdarg.h>
#include <builtins.h>
#include <stdbool.h>
#include <stdint.h>
#if defined(IO_TO_CONSOLE) || defined(IO_TO_UART)
   #include <stdio.h>
#endif

#if defined(_MISRA_2004_RULES) || defined(_MISRA_2012_RULES)
#pragma diag(push)
#if defined(_MISRA_2004_RULES)
#pragma diag(suppress:misra_rule_16_1:"print implementation needs varargs arguments")
#pragma diag(suppress:misra_rule_20_9:"This example produces output")
#else
#pragma diag(suppress:misra2012_rule_21_6:"This example produces output")
#endif
#endif

/*****************************************************************************
 * I N I T I A L I Z A T I O N    R O U T I N E S
 *****************************************************************************/

/**
 * Initialize the LED that we use for Core 2
 */
static ADI_GPIO_RESULT initialize_led(void);
static ADI_GPIO_RESULT initialize_led(void)
{
    return adi_gpio_SetDirection(CORE2_LED_PORT, CORE2_LED_PIN, ADI_GPIO_DIRECTION_OUTPUT);
}

static volatile bool buttonPressed = false;

/**
 * Callback when we press the push button.
 */
static void pintCallback(ADI_GPIO_PIN_INTERRUPT ePinInt, uint32_t PinIntData,  void *pCBParam);
static void pintCallback(ADI_GPIO_PIN_INTERRUPT ePinInt, uint32_t PinIntData,  void *pCBParam)
{
    buttonPressed = true;
}

/**
 * Initialize the Push Button that we use for Core 2
 */
static ADI_GPIO_RESULT initialize_push_button(void);
static ADI_GPIO_RESULT initialize_push_button(void)
{
    static uint8_t gpioMemory[ADI_GPIO_CALLBACK_MEM_SIZE];
    uint32_t gpioMaxCallbacks;

    /* Initialize the GPIO service */
    ADI_GPIO_RESULT result = adi_gpio_Init((void*)gpioMemory, ADI_GPIO_CALLBACK_MEM_SIZE, &gpioMaxCallbacks);

    if (result == ADI_GPIO_SUCCESS)
    {
        result = adi_gpio_SetDirection(CORE2_PB_PORT, CORE2_PB_PORT_PIN, ADI_GPIO_DIRECTION_INPUT);
    }
    if (result == ADI_GPIO_SUCCESS)
    {
        /* Set edge sense mode  */
        result = adi_gpio_SetPinIntEdgeSense(CORE2_PB_PINT, CORE2_PB_PINT_PIN, ADI_GPIO_SENSE_RISING_EDGE);
    }
    if (result == ADI_GPIO_SUCCESS)
    {
        /* Register pinint callback */
        result = adi_gpio_RegisterCallback(CORE2_PB_PINT, CORE2_PB_PINT_PIN, pintCallback, (void*)0);
    }
    if (result == ADI_GPIO_SUCCESS)
    {
        /* Assign pin interrupt  */
        result = adi_gpio_PinInterruptAssignment(CORE2_PB_PINT, CORE2_PIN_ASSIGN_BYTE, CORE2_PIN_ASSIGN);
    }
    if (result == ADI_GPIO_SUCCESS)
    {
        /* Set pin interrupt mask */
        result = adi_gpio_EnablePinInterruptMask(CORE2_PB_PINT, CORE2_PB_PINT_PIN, true);
    }
    return result;
}

/*****************************************************************************
 * O U T P U T    R O U T I N E S
 *****************************************************************************/

/**
 * Print routine to print output string to console or send to Core 0 to
 * print over UART.
 */
void print(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
#ifdef IO_TO_CONSOLE
    printf("[CORE %d]: ", adi_core_id());
    vprintf(fmt, ap);
#elif defined(IO_TO_UART)
    {
        int32_t outLen = vsnprintf(outputMessage.buffer, UART_DEBUG_BUFFER_LINE_SIZE, fmt, ap);
        send_output_to_core0();
        if (outLen >= (int32_t)UART_DEBUG_BUFFER_LINE_SIZE) {
          snprintf(outputMessage.buffer, UART_DEBUG_BUFFER_LINE_SIZE, "\n  ...(output truncated)...\n");
          send_output_to_core0();
        }
    }
#else
    /* no output */
#endif
}

/*****************************************************************************
 * A P P L I C A T I O N   R O U T I N E S
 *****************************************************************************/

#pragma optimize_for_speed /* to get a consistent timing */

/**
 * Function that delays for approximately 1ms
 */
static void delay(const uint32_t iSpeed);
static void delay(const uint32_t iSpeed)
{
    uint32_t x, y;
    uint32_t counter = 500000u;  /* cycles for 1ms */

    for( x = 0u; x < iSpeed; x++ )
    {
        for( y = 0u; y < counter; y++ )
        {
          NOP();
        }
    }
}

#pragma optimize_as_cmd_line

/**
 * Blink the LED while we wait for Push Button to be pressed.
 * If Push Button is not pressed carry on regardless.
 */
static void blink_led_and_wait_for_button(void);
static void blink_led_and_wait_for_button(void)
{
    uint32_t count = 50u;
    buttonPressed = false;
    print("(%s should now be blinking!)\n", CORE2_LED);
    while ((!buttonPressed) && (count > 0u))
    {
        adi_gpio_Toggle(CORE2_LED_PORT, CORE2_LED_PIN);
        delay(500u);
        count--;
    }
    adi_gpio_Set(CORE2_LED_PORT, CORE2_LED_PIN);
    print(buttonPressed ? "%s pressed\n" : "Timeout waiting for %s\n", CORE2_PB);
}

/**
 * Main function.
 * - Set up MCAPI communication with Core 0.
 * - Await message from Core 0 to start executing.
 * - Flash LED to show success and wait for Push Button press.
 * - Send confirmation message to Core 0.
 * - Await shutdown message from Core 0.
 */
int main(void)
{
    /**
     * Initialize managed drivers and/or services that have been added to 
     * the project.
     * @return zero on success 
     */
    adi_initComponents();

    /* Initialize LED, and start MCAPI communications with core 0.
    */
    if (initialize_led() != ADI_GPIO_SUCCESS)
    {
        print("Failed to initialize %s\n", CORE2_LED);
    }
    initialize_comms(PORT_COMMS);

    /* Wait for EXECUTE command from Core 0.
    */
    wait_for_command(CMD_EXECUTE);

    /* Initialize Push Button. Done after EXECUTE command received, as the
    ** push button may be shared with Core 1.
    */
    if (initialize_push_button() != ADI_GPIO_SUCCESS)
    {
        print("Failed to initialize %s\n", CORE2_PB);
    }

    /* Print instructions for next stage.
    */
    print("Core 2 received message from Core 0\n");
    print("Press %s to acknowledge the message back to Core 0.\n", CORE2_PB);
    print("When Core 0 has received that message, %s stays on and %s will blink.\n", CORE2_LED, CORE0_LED);

    /* Flash LED to show success, and wait for Push Button press.
    */
    blink_led_and_wait_for_button();

    /* Send acknowledgement back to Core 0.
    */
    acknowledge_command();

    /* Await command from Core 0 to shut down.
    */
    wait_for_command(CMD_TERMINATE);

    /* Switch off the lights and shut down.
    */
    adi_gpio_Clear(CORE2_LED_PORT, CORE2_LED_PIN);
    shut_down_comms();

    return 0;
}

#if defined(_MISRA_2004_RULES) || defined(_MISRA_2012_RULES)
#pragma diag(pop)
#endif /* _MISRA_RULES */
