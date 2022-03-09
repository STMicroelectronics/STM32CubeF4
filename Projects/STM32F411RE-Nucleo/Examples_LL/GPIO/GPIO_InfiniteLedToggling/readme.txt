/**
  @page GPIO_InfiniteLedToggling GPIO example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/GPIO/GPIO_InfiniteLedToggling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the GPIO example.
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

This example describes how to configure and use GPIOs to toggle every 250 ms
the user LEDs available on the board. This example is based on the STM32F4xx
LL API. Peripheral initialization is done using LL unitary services functions
for optimization purpose (performance and size).

PA.05 IO (configured in output pushpull mode) toggles in a forever loop.
On NUCLEO-F411RE board this IO is connected to LED2.

In this example, HCLK is configured at 100 MHz.

@par Keywords

System, GPIO, Output, Alternate function, Push-pull, Toggle

@par Directory contents 

  - GPIO/GPIO_InfiniteLedToggling/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - GPIO/GPIO_InfiniteLedToggling/Inc/main.h                  Header for main.c module
  - GPIO/GPIO_InfiniteLedToggling/Inc/stm32_assert.h          Template file to include assert_failed function
  - GPIO/GPIO_InfiniteLedToggling/Src/stm32f4xx_it.c          Interrupt handlers
  - GPIO/GPIO_InfiniteLedToggling/Src/main.c                  Main program
  - GPIO/GPIO_InfiniteLedToggling/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F411xE devices.
    
  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
