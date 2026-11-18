/**
  @page GPIO_Demo GPIO Demo example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    GPIO/GPIO_Demo/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the GPIO Demo example.
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

This example shows how to use the NUCLEO-F446ZE board to make an LED blink at different 
speeds when you press a button.

In this example:
when falling edge is detected on EXTI_Line15_10 by pressing User push-button, LEDs blink at different 
speeds.

In this example, HCLK is configured at 180 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, GPIO, Output, Alternate function, Demo, Toggle

@par Directory contents 

  - GPIO/GPIO_Demo/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - GPIO/GPIO_Demo/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - GPIO/GPIO_Demo/Inc/main.h                  Header for main.c module  
  - GPIO/GPIO_Demo/Src/stm32f4xx_it.c          Interrupt handlers
  - GPIO/GPIO_Demo/Src/main.c                  Main program
  - GPIO/GPIO_Demo/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with NUCLEO-446ZE Rev.B board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example



 */
