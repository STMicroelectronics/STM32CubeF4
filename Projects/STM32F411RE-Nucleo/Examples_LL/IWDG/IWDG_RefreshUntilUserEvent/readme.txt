/**
  @page IWDG_RefreshUntilUserEvent IWDG example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/IWDG/IWDG_RefreshUntilUserEvent/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the IWDG_RefreshUntilUserEvent example.
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

This example describes how to configure the IWDG to ensure period counter update and
generate an MCU IWDG reset when a user button is pressed. Peripheral initialization is 
done using LL unitary services functions for optimization purpose (performance and size).

Example Configuration:
Configure the IWDG (prescaler, counter) and enable it.
Infinite refresh of the IWDG down-counter done in the main loop.
LED2 is blinking fast & continuously.

Example Execution:
When User Button is pressed, the down-counter automatic refresh mechanism is 
disable and thus, reset will occur. After a reset, when re-entering in the main, 
RCC IWDG Reset Flag will be checked and if we are back from a IWDG reset the LED2
will be switch ON.

Waiting a new user button pressed to re-activate the IWDG

@par Keywords

System, IWDG, Timeout, MCU Reset, Downcounter, Timer, Measure Frequency

@par Directory contents 

  - IWDG/IWDG_RefreshUntilUserEvent/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - IWDG/IWDG_RefreshUntilUserEvent/Inc/main.h                  Header for main.c module
  - IWDG/IWDG_RefreshUntilUserEvent/Inc/stm32_assert.h          Template file to include assert_failed function
  - IWDG/IWDG_RefreshUntilUserEvent/Src/stm32f4xx_it.c          Interrupt handlers
  - IWDG/IWDG_RefreshUntilUserEvent/Src/main.c                  Main program
  - IWDG/IWDG_RefreshUntilUserEvent/Src/system_stm32f4xx.c      STM32F4xx system source file


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
