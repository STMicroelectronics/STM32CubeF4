/**
  @page PWR_Standby PWR_STANDBY_RTC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    PWR/PWR_STANDBY_RTC/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR STANDBY RTC example.
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

This example shows how to enter Standby mode and wake up from this mode using
an external RESET or the RTC Wakeup Timer through the STM32F4xx RTC and RCC HAL
and LL API (LL API usage for performance improvement).

It allows to measure the current consumption in STANDBY mode with RTC enabled.

In the associated software, the system clock is set to 100 MHz and the SysTick is 
programmed to generate an interrupt each 1 ms.
The Low Speed Internal (LSI) clock is used as RTC clock source by default.
EXTI_Line20 is internally connected to the RTC Wakeup event.

The system automatically enters STANDBY mode 5 sec. after start-up. The RTC wake-up 
is configured to generate an interrupt on rising edge about 28 sec. afterwards.
Current consumption in STANDBY mode with RTC feature enabled can be measured during that time.
More than half a minute is chosen to ensure current convergence to its lowest operating point.

Note: Due to LSI frequency variations, wake-up time is not guarantee. Adjustments need to be
done after getting the real measurement of LSI oscillator (or if available, LSE oscillator can
be used as well)

After wake-up from STANDBY mode, program execution restarts in the same way as after
a RESET.

LED2 is used to monitor the system state as follows:
 - LED2 ON: configuration failed (system will go to an infinite loop)
 - LED2 toggling: system in RUN mode
 - LED2 off : system in STANDBY mode

These steps are repeated in an infinite loop.
 
@note To measure the current consumption in STANDBY mode, remove JP6 jumper 
      and connect an amperemeter to JP6 to measure IDD current.     


@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
       so debugging features are disabled.

@note Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
      the RTC clock source; in this case the Backup domain will be reset in  
      order to modify the RTC Clock source, as consequence RTC registers (including 
      the backup registers) and RCC_CSR register are set to their reset values.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@par Keywords

Power, PWR, Standby mode, Interrupt, EXTI, Wakeup, Low Power, RTC, External reset, LSI,

@par Directory contents 

  - PWR/PWR_STANDBY_RTC/Inc/stm32f4xx_conf.h         HAL Configuration file
  - PWR/PWR_STANDBY_RTC/Inc/stm32f4xx_it.h           Header for stm32f4xx_it.c
  - PWR/PWR_STANDBY_RTC/Inc/main.h                   Header file for main.c
  - PWR/PWR_STANDBY_RTC/Src/system_stm32f4xx.c       STM32F4xx system clock configuration file
  - PWR/PWR_STANDBY_RTC/Src/stm32f4xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY_RTC/Src/main.c                   Main program
  - PWR/PWR_STANDBY_RTC/Src/stm32f4xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32F4xx devices

  - This example has been tested with STMicroelectronics NUCLEO-F411RE
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-F411RE Set-up :
    - LED2 connected to PA.05 pin

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Once the image is loaded, power off the NUCLEO board in unplugging
   the power cable then power on the board again 
 - Run the example



 */
