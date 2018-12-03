/*****************************************************************************
 * File:    ThreeCore_SC584.h
 *
 * Copyright(c) 2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#ifndef THREECORE_SC584_H_
#define THREECORE_SC584_H_

/* The LED used by Core 0 to report its status.
*/
#define CORE0_LED             "LED10"
#define CORE0_LED_PORT        (ADI_GPIO_PORT_E)
#define CORE0_LED_PIN         (ADI_GPIO_PIN_1)

/* The Push Button used by Core 0.
*/
#define CORE0_PB              "PB1"
#define CORE0_PB_PORT         (ADI_GPIO_PORT_A)
#define CORE0_PB_PORT_PIN     (ADI_GPIO_PIN_15)
#define CORE0_PB_PINT         (ADI_GPIO_PIN_INTERRUPT_0)
#define CORE0_PB_PINT_PIN     (ADI_GPIO_PIN_31)
#define CORE0_PIN_ASSIGN      (ADI_GPIO_PIN_ASSIGN_PAH_PINT0)
#define CORE0_PIN_ASSIGN_BYTE (ADI_GPIO_PIN_ASSIGN_BYTE_3)

/* The LED used by Core 1 to report its status.
*/
#define CORE1_LED             "LED11"
#define CORE1_LED_PORT        (ADI_GPIO_PORT_E)
#define CORE1_LED_PIN         (ADI_GPIO_PIN_2)

/* The Push Button used by Core 1.
*/
#define CORE1_PB              "PB2"
#define CORE1_PB_PORT         (ADI_GPIO_PORT_B)
#define CORE1_PB_PORT_PIN     (ADI_GPIO_PIN_0)
#define CORE1_PB_PINT         (ADI_GPIO_PIN_INTERRUPT_1)
#define CORE1_PB_PINT_PIN     (ADI_GPIO_PIN_0)
#define CORE1_PIN_ASSIGN      (ADI_GPIO_PIN_ASSIGN_PBL_PINT1)
#define CORE1_PIN_ASSIGN_BYTE (ADI_GPIO_PIN_ASSIGN_BYTE_0)

/* The LED used by Core 2 to report its status.
*/
#define CORE2_LED             "LED12"
#define CORE2_LED_PORT        (ADI_GPIO_PORT_E)
#define CORE2_LED_PIN         (ADI_GPIO_PIN_3)

/* The Push Button used by Core 2.
*/
/* We can use PB3 as long as we don't use the UART0 RTS signal */ 
#define CORE2_PB              "PB3"
#define CORE2_PB_PORT         (ADI_GPIO_PORT_C)
#define CORE2_PB_PORT_PIN     (ADI_GPIO_PIN_15)
#define CORE2_PB_PINT         (ADI_GPIO_PIN_INTERRUPT_2)
#define CORE2_PB_PINT_PIN     (ADI_GPIO_PIN_31)
#define CORE2_PIN_ASSIGN      (ADI_GPIO_PIN_ASSIGN_PCH_PINT2)
#define CORE2_PIN_ASSIGN_BYTE (ADI_GPIO_PIN_ASSIGN_BYTE_3)


#endif /* THREECORE_SC584_H_ */
