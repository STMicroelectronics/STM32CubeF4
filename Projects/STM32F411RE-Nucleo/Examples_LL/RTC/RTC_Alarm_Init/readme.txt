/**
  @page RTC_Alarm_Init RTC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/RTC/RTC_Alarm_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RTC example.
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

This example guides you through the different configuration steps by mean of LL API 
to ensure Alarm configuration and generation using the RTC peripheral. Peripheral 
initialization done using LL initialization function to demonstrate LL init usage.

In this example, after start-up, SYSCLK is configured to the max frequency using the PLL with
HSI as clock source.

The RTC peripheral configuration is ensured by the Configure_RTC() function 
(configure of the needed RTC resources according to the used hardware CLOCK, 
PWR, RTC clock source and BackUp). You may update this function to change RTC configuration.

@note LSI oscillator clock is used as RTC clock source by default.
      The user can use also LSE as RTC clock source. The user uncomment the adequate 
      line on the main.c file.
      @code
        #define RTC_CLOCK_SOURCE_LSI  
        /* #define RTC_CLOCK_SOURCE_LSE */
      @endcode
      LSI oscillator clock is delivered by a 32 kHz RC.
      LSE (when available on board) is delivered by a 32.768 kHz crystal.

Configure_RTC_Alarm function is then called to initialize the 
time, date and alarm.

In this example, the Time is set to 11:59:55 PM and the Alarm must be generated after 
30 seconds on 12:00:25 AM.

LED2 is turned ON when the RTC Alarm is generated correctly.
The current time and date are updated and displayed on the debugger in aShowTime 
and aShowDate variables (watch or live watch).

In case of error, LED2 is toggling.

@par Keywords

System, RTC, Alarm, wakeup timer, Backup domain, Counter, LSE, LSI

@par Directory contents 

  - RTC/RTC_Alarm_Init/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - RTC/RTC_Alarm_Init/Inc/main.h                  Header for main.c module
  - RTC/RTC_Alarm_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - RTC/RTC_Alarm_Init/Src/stm32f4xx_it.c          Interrupt handlers
  - RTC/RTC_Alarm_Init/Src/main.c                  Main program
  - RTC/RTC_Alarm_Init/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
