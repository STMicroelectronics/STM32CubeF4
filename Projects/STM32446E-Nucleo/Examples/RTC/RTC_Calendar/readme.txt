/**
  @page RTC_Calendar RTC Calendar Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    RTC/RTC_Calendar/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RTC Calendar example.
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

This example guides you through the different configuration steps by mean of HAL API 
to ensure Calendar configuration using the RTC peripheral.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

The RTC peripheral configuration is ensured by the HAL_RTC_Init() function.
This later is calling the HAL_RTC_MspInit()function which core is implementing
the configuration of the needed RTC resources according to the used hardware (CLOCK, 
PWR, RTC clock source and BackUp). You may update this function to change RTC configuration.

The LSE oscillator clock is used as RTC clock source. LSE oscillator clock usually 
delivered by a 32.768 kHz quartz, which can be one of the listed configurations 
depending the version of NUCLEO board hardware:
 - On-board oscillator : X2 crystal 32.768 kHz
 - Oscillator from external depending the version of NUCLEO board hardware.


HAL_RTC_SetTime()and HAL_RTC_SetDate() functions are then called to initialize the 
time and the date.

A key value is written in backup data register 1 to indicate if the RTC is already configured.  
      
The program behaves as follows:

1. After startup the program checks the backup data register 1 value:
    - BKP_DR1 value not correct: (RTC_BKP_DR1 value is not correct or has not yet
      been programmed when the program is executed for the first time) the RTC is
      configured.
    
    - BKP_DR1 value correct: this means that the RTC is configured and the time
      and date are displayed on Debugger.

2. When power on reset occurs:
LED2 is turned ON and toggles when there is an error. 
The current time and date are updated and displayed on the debugger in aShowTime 
and aShowDate variables.

@note Using stdio.h may increase the code size, it is optional and intended only to
      ease the example by displaying the date variables (aShowTime and aShowDate) on the debugger.
      By default this option is enabled, user can disable it by commenting
       "#define DISPLAY_ON_DEBUGGER" in main.h file.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
             
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The clock setting is configured to have the max product performance (max clock frequency) 
      so not optimized in term of power consumption.

@par Keywords

System, RTC, Calendar, Backup Domain, Reset

@par Directory contents  

  - RTC/RTC_Calendar/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - RTC/RTC_Calendar/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - RTC/RTC_Calendar/Inc/main.h                  Header for main.c module
  - RTC/RTC_Calendar/Src/stm32f4xx_it.c          Interrupt handlers
  - RTC/RTC_Calendar/Src/main.c                  Main program
  - RTC/RTC_Calendar/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - RTC/RTC_Calendar/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F446xx Devices.
    
  - This example has been tested with STMicroelectronics STM32F4xx-Nucleo rev C 
    board and can be easily tailored to any other supported device 
    and development board.

  - For This example to work properly, a 2.2 µF Capacitor must be present on the board in slot C26.

  - STM32F4xx-Nucleo Set-up
    - Use LED2 connected to PA.05 pin


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
