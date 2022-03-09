/**
  @page RNG_GenerateRandomNumbers_IT RNG : Random Number Generation using IT
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/RNG/RNG_GenerateRandomNumbers_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RNG_GenerateRandomNumbers_IT example.
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

This example shows how to configure RNG peripheral to allow generation of 
32-bit long Random Numbers, using interrupts. Peripheral initialization done
using LL unitary services functions for optimization purpose (performance and size).

Example execution:
After startup from reset and system configuration, RNG configuration is performed.
(Configure Main PLL to enable 48M domain, then enable PLLQ output mapped on 48MHz domain clock).

User is then asked to press key button (LED1 blinking fast).
On user button press, several (8) Random 32bit numbers are generated
(On each raised RNG interrupt, a random number is generated and retrieved from DR register).
Corresponding generated values are available and stored in a u32 array (aRandom32bit), 
whose content could be displayed using debugger (Watch or LiveWatch features).
After successful Random numbers generation, LED1 is turned On. 
In case of errors, LED1 is slowly blinking (1sec period).

@par Keywords

Analog, RNG, Random, FIPS PUB 140-2, Analog Random number generator, Entropy, Period, Interrupt

@par Directory contents 

  - RNG/RNG_GenerateRandomNumbers_IT/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - RNG/RNG_GenerateRandomNumbers_IT/Inc/main.h                  Header for main.c module
  - RNG/RNG_GenerateRandomNumbers_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - RNG/RNG_GenerateRandomNumbers_IT/Src/stm32f4xx_it.c          Interrupt handlers
  - RNG/RNG_GenerateRandomNumbers_IT/Src/main.c                  Main program
  - RNG/RNG_GenerateRandomNumbers_IT/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F429xx devices.
    
  - This example has been tested with STM32F429ZI-Nucleo Rev B board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 - Push User button and use Variable watch window from debugger to access to values of generated numbers.
   (A break point could be set on LED_On() call, at end of RandomNumbersGeneration_IT() function).


 */
