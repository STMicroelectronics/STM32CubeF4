/**
  @page RTC_ExitStandbyWithWakeUpTimer RTC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/RTC/RTC_ExitStandbyWithWakeUpTimer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RTC example.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
   @endverbatim

@par Example Description

This example shows how to configure the RTC in order to wake up from Standby mode
using RTC Wakeup Timer. Peripheral initialization is done using LL unitary services
functions for optimization purpose (performance and size).
  
In this example, after start-up, SYSCLK is configured to the max frequency using 
the PLL with HSI as clock source.

   @note LSI oscillator clock is used as RTC clock source by default.
      The user can use also LSE as RTC clock source. The user uncomment the adequate 
      line on the main.h file.
      @code
        #define RTC_CLOCK_SOURCE_LSI  
        /* #define RTC_CLOCK_SOURCE_LSE */
      @endcode
      LSI oscillator clock is delivered by a 32 kHz RC.
      LSE (when available on board) is delivered by a 32.768 kHz crystal.

Example execution:
   - 1st execution of the system, LED2 is quickly blinking (every 200ms). 
   - Press the user button:
     * RTC wakup timer is configured to 5 seconds
     * System enters in standby mode (LED2 is switched off)
   - After 5 seconds, system resumes from standby mode, then LED2 is slowly blinking (every 500ms).

   - LED2 is toggling every 1 second: This indicates that the system generates an error.

@par Keywords

System, RTC, RTC Wakeup timer, Standby mode, LSI, LSE, Interrupt,

@par Directory contents 

  - RTC/RTC_ExitStandbyWithWakeUpTimer/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - RTC/RTC_ExitStandbyWithWakeUpTimer/Inc/main.h                  Header for main.c module
  - RTC/RTC_ExitStandbyWithWakeUpTimer/Inc/stm32_assert.h          Template file to include assert_failed function
  - RTC/RTC_ExitStandbyWithWakeUpTimer/Src/stm32f4xx_it.c          Interrupt handlers
  - RTC/RTC_ExitStandbyWithWakeUpTimer/Src/main.c                  Main program
  - RTC/RTC_ExitStandbyWithWakeUpTimer/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
