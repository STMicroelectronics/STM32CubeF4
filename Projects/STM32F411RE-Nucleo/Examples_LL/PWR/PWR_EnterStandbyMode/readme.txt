/**
  @page PWR_EnterStandbyMode PWR standby example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/PWR/PWR_EnterStandbyMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR STANDBY mode example.
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

This example shows how to enter the system in STANDBY mode and wake-up from this
mode using external RESET or wake-up interrupt.

In the associated software, the system clock is set to 100 MHz, an EXTI line
is connected to the user button thru PC.13 and configured to generate an 
interrupt on rising edge upon key press.
LED2 is toggled each 200 ms in order to indicate that MCU is in RUN mode.

When a rising edge is detected on the EXTI line, an interrupt is generated. 
In the EXTI handler routine, the wake-up pin LL_PWR_WAKEUP_PIN1 is enabled and the 
corresponding wake-up flag cleared. Then, the system enters STANDBY mode causing 
LED2 to stop toggling. 

Next, a rising edge on PA.00 (connected to LL_PWR_WAKEUP_PIN1) will 
wake-up the system from STANDBY.
Alternatively, an external RESET of the board will lead to a system wake-up as well.

After wake-up from STANDBY mode, program execution restarts in the same way as 
after a RESET and LED2 restarts toggling.

LED2 is used to monitor the system state as follows:
 - LED2 fast toggling: system in RUN mode
 - LED2 slow toggling: system in RUN mode after exiting from standby mode
 - LED2 off : system in STANDBY mode

These steps are repeated in an infinite loop.

@note To measure the current consumption in STANDBY mode, remove JP6 jumper 
      and connect an ampere meter to JP6 to measure IDD current.     

@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disabled.

@par Keywords

Power, PWR, Standby mode, Interrupt, EXTI, Wakeup, Low Power, External reset,

@par Directory contents 

  - PWR/PWR_EnterStandbyMode/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - PWR/PWR_EnterStandbyMode/Inc/main.h                  Header for main.c module
  - PWR/PWR_EnterStandbyMode/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_EnterStandbyMode/Src/stm32f4xx_it.c          Interrupt handlers
  - PWR/PWR_EnterStandbyMode/Src/main.c                  Main program
  - PWR/PWR_EnterStandbyMode/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-F411RE
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-F411RE Set-up
    - Put a wire between PA.00 (connected to pin 28 on CN7 connector) and 
    3.3V pin (pin 5 on CN7) to generate a rising edge on LL_PWR_WAKEUP_PIN1.
    - WakeUp Pin LL_PWR_WAKEUP_PIN1 connected to PA.00

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
