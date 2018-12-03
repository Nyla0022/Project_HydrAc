/*****************************************************************************
 * File:    ThreeCore_SC573.h
 *
 * Copyright(c) 2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#ifndef THREECORE_SC573_H_
#define THREECORE_SC573_H_

/* The LED used by Core 0 to report its status.
*/
#define CORE0_LED             "LED10"
#define CORE0_LED_PORT        (ADI_GPIO_PORT_E)
#define CORE0_LED_PIN         (ADI_GPIO_PIN_13)

/* The Push Button used by Core 0.
*/
#define CORE0_PB              "PB1"
#define CORE0_PB_PORT         (ADI_GPIO_PORT_C)
#define CORE0_PB_PORT_PIN     (ADI_GPIO_PIN_8)
#define CORE0_PB_PINT         (ADI_GPIO_PIN_INTERRUPT_1)
#define CORE0_PB_PINT_PIN     (ADI_GPIO_PIN_8)
#define CORE0_PIN_ASSIGN      (ADI_GPIO_PIN_ASSIGN_PCH_PINT1)
#define CORE0_PIN_ASSIGN_BYTE (ADI_GPIO_PIN_ASSIGN_BYTE_1)

/* The LED used by Core 1 to report its status.
*/
#define CORE1_LED             "LED11"
#define CORE1_LED_PORT        (ADI_GPIO_PORT_A)
#define CORE1_LED_PIN         (ADI_GPIO_PIN_9)

/* The Push Button used by Core 1.
*/
#define CORE1_PB              "PB2"
#define CORE1_PB_PORT         (ADI_GPIO_PORT_C)
#define CORE1_PB_PORT_PIN     (ADI_GPIO_PIN_9)
#define CORE1_PB_PINT         (ADI_GPIO_PIN_INTERRUPT_2)
#define CORE1_PB_PINT_PIN     (ADI_GPIO_PIN_9)
#define CORE1_PIN_ASSIGN      (ADI_GPIO_PIN_ASSIGN_PCH_PINT2)
#define CORE1_PIN_ASSIGN_BYTE (ADI_GPIO_PIN_ASSIGN_BYTE_1)

/* The LED used by Core 2 to report its status.
*/
#define CORE2_LED             "LED12"
#define CORE2_LED_PORT        (ADI_GPIO_PORT_E)
#define CORE2_LED_PIN         (ADI_GPIO_PIN_1)

/* The Push Button used by Core 2.
*/
#define CORE2_PB              "PB3"
#define CORE2_PB_PORT         (ADI_GPIO_PORT_C)
#define CORE2_PB_PORT_PIN     (ADI_GPIO_PIN_10)
#define CORE2_PB_PINT         (ADI_GPIO_PIN_INTERRUPT_2)
#define CORE2_PB_PINT_PIN     (ADI_GPIO_PIN_10)
#define CORE2_PIN_ASSIGN      (ADI_GPIO_PIN_ASSIGN_PCH_PINT2)
#define CORE2_PIN_ASSIGN_BYTE (ADI_GPIO_PIN_ASSIGN_BYTE_1)


#endif /* THREECORE_SC573_H_ */
