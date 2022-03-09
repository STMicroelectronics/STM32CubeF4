/**
  @page PWR_STOP Power Stop Mode Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    PWR/PWR_STOP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Power Stop Mode example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
   @endverbatim

@par Example Description 


This example shows how to enter Stop mode and wake up from this mode by using the 
RTC Wakeup timer event or an interrupt.

This example shows how to enters the system to STOP mode and wake-up from this
mode using RTC Wake-up Timer Event connected to EXTI Line 22 or Key push button
EXTI Line 13.

In the associated software
  - the system clock is set to 180 MHz
  - the EXTI_Line13 is configured to generate interrupt on falling edge
  - the EXTI_Line22 connected internally to the RTC Wake-up event is configured
    to generate an interrupt on rising edge each 4s
  - the SysTick is programmed to generate an interrupt each 1 ms 
In the SysTick interrupt handler, LED2 is toggled, this is used to indicate whether
the MCU is in STOP or RUN mode.

The system enters STOP mode and will wait for the RTC Wake-up event to be 
generated each 4s, or Key push button is pressed. 
 - If the RTC WakeUp event (EXTI_Line22) is the source of wake-up from STOP, LED1 is toggled.
 - If the Key button (EXTI_Line13) is the source of wake-up from STOP, LED4 is toggled.

User may select either Stop mode or under drive stop mode by uncommenting the corresponding line inside the main.h file
  /* #define STOP_MODE            */
  /* #define STOP_UNDERDRIVE_MODE */
 
This behavior is repeated in an infinite loop.

LEDs are used to monitor the system state as following:
 - LED1 toggled: system woken up from STOP using RTC WakeUp Interrupt
 - LED2 toggling: system in RUN mode
 - LED3 Initialization error (RTC, RCC,...)
 - LED4 toggled: system woken up from STOP using EXTI_Line13 (KEY push button)
 
 @note To measure the current consumption in STOP mode, please refer to 
      @subpage PWR_CurrentConsumption example.

 @note This example can not be used in DEBUG mode, this is due to the fact 
       that the Cortex-M4 core is no longer clocked during low power mode 
       so debugging features are disabled
       
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select the RTC clock source; in this 
       case the Backup domain will be reset in order to modify the RTC Clock source, as consequence RTC  
       registers (including the backup registers) and RCC_BDCR register are set to their reset values.

@par Keywords

Power, PWR, Stop mode, Interrupt, EXTI, Wakeup, Low Power, External reset

@par Directory contents 

  - PWR/PWR_STOP/Inc/stm32f4xx_hal_conf.h     HAL configuration file
  - PWR/PWR_STOP/Inc/stm32f4xx_it.h           Interrupt handlers header file
  - PWR/PWR_STOP/Inc/main.h                   header file for main.c
  - PWR/PWR_STOP/Src/system_stm32f4xx.c       STM32F4xx system clock configuration file
  - PWR/PWR_STOP/Src/stm32f4xx_it.c           Interrupt handlers
  - PWR/PWR_STOP/Src/main.c                   Main program
  - PWR/PWR_STOP/Src/stm32f4xx_hal_msp.c      HAL MSP module


@par Hardware and Software environment

  - This example runs on STM32F429xx/STM32F439xx devices.
  
  - This example has been tested with STMicroelectronics STM324x9I-EVAL RevB 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.   
      
  - STM324x9I-EVAL RevB Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PG.06, PG.07, PG.10 and PG.12 pins
    - Use the Key push-button connected to pin PC13 (EXTI_Line13)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
