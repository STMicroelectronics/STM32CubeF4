/**
  @page RTC_TimeStamp RTC Time Stamp example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    RTC/RTC_TimeStamp/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RTC Time Stamp example.
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

This example guides you through the different configuration steps by means of the RTC 
HAL API to demonstrate the timestamp feature.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 168 MHz.

The RTC peripheral configuration is ensured by the HAL_RTC_Init() function.
This later is calling the HAL_RTC_MspInit()function which core is implementing
the configuration of the needed RTC resources according to the used hardware (CLK, 
PWR, RTC clock source and BackUp). You may update this function to change RTC configuration.

@note LSE oscillator clock is used as RTC clock source by default. LSE oscillator 
      clock usually delivered by a 32.768 kHz quartz.
      The user can use also LSI as RTC clock source. The user uncomment the adequate 
      line on the main.h file.
      @code
        #define RTC_CLOCK_SOURCE_LSE  
        /* #define RTC_CLOCK_SOURCE_LSI */
      @endcode 

HAL_RTCEx_SetTamper()function is called to initialize the tamper feature.
HAL_RTCEx_SetTimeStamp_IT()function is then called to initialize the time stamp feature 
with interrupt mode. It configures The Time Stamp pin to be falling edge and enables
the time Stamp detection on tamper pin.
HAL_RTC_SetTime()and HAL_RTC_SetDate() functions are then called to initialize the 
time and the date.

A key value is written in backup data register 0 to indicate if the RTC is already configured.  

The program behaves as follows:

1. After startup the program configure the RTC (Time date) and enable the feature 
   timeStamp. LED1 is on means that the RTC is configured and the time is displayed.

2. Applying a low level on the Time Stamp pin (PC13) by pressing to Tamper button, 
   a time stamp event is detected and the calendar is saved in the time stamp structures.
   Each time the user presses on tamper button, LED4 toggles.

The time stamp and the current time are displayed on the LCD integrated on the eval board
if the user uncomment the define USE_LCD in main.h file.
If the user comment USE_LCD, the current time and Time Stamp are updated and displayed 
on the debugger in aShowTime and aShowTimeStamp variables.

LED1 is turned ON when the RTC configuration is done correctly.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

RTC, Timer, Timestamp, Counter, LSE, LSI, Current time, Real Time Clock

@par Directory contents 

  - RTC/RTC_TimeStamp/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - RTC/RTC_TimeStamp/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - RTC/RTC_TimeStamp/Inc/main.h                  Main program header file  
  - RTC/RTC_TimeStamp/Src/stm32f4xx_it.c          Interrupt handlers
  - RTC/RTC_TimeStamp/Src/main.c                  Main program
  - RTC/RTC_TimeStamp/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - RTC/RTC_TimeStamp/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment
  
  - This example runs on STM32F407xx/417xx devices.
  
  - This example has been tested with STMicroelectronics STM324xG-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


  */
