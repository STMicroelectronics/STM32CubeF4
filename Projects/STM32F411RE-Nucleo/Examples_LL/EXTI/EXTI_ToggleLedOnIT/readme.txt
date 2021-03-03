/**
  @page EXTI_ToggleLedOnIT EXTI example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/EXTI/EXTI_ToggleLedOnIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the EXTI example.
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

This example describes how to configure the EXTI and use
GPIOs to toggle the user LEDs available on the board when
a user button is pressed. It is based on the STM32F4xx LL
API. Peripheral initialization is done using LL unitary services
functions for optimization purpose (performance and size).

In this example, one EXTI line (EXTI13) is configured to generate
an interrupt on each falling edge.

In the interrupt routine a led connected to a specific GPIO pin is toggled.

In this example:
    - EXTI13 is connected to PC.13 pin
      - when falling edge is detected on EXTI13 by pressing User push-button, LED2 toggles

On NUCLEO-F411RE:
    - EXTI13 is connected to User push-button

@par Keywords

System, GPIO, Output, Alternate function, EXTI, Toggle

@par Directory contents 

  - EXTI/EXTI_ToggleLedOnIT/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - EXTI/EXTI_ToggleLedOnIT/Inc/main.h                  Header for main.c module
  - EXTI/EXTI_ToggleLedOnIT/Inc/stm32_assert.h          Template file to include assert_failed function
  - EXTI/EXTI_ToggleLedOnIT/Src/stm32f4xx_it.c          Interrupt handlers
  - EXTI/EXTI_ToggleLedOnIT/Src/main.c                  Main program
  - EXTI/EXTI_ToggleLedOnIT/Src/system_stm32f4xx.c      STM32F4xx system source file

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
