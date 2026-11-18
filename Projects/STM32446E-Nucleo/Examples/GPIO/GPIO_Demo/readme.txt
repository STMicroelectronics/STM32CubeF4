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

This example shows how to use the STM32F446xx Nucleo board to make an LED blink at different 
speeds when you press a button.

The HCLK is configured at 180 MHz for STM32F446xx Devices.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The clock setting is configured to have the max product performance (max clock frequency) 
      so not optimized in term of power consumption.

@par Keywords

System, GPIO, Output, Alternate function, Push-pull, Toggle
 
@par Directory contents 

  - GPIO/GPIO_Demo/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - GPIO/GPIO_Demo/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - GPIO/GPIO_Demo/Inc/main.h                  Main program header file  
  - GPIO/GPIO_Demo/Src/stm32f4xx_it.c          Interrupt handlers
  - GPIO/GPIO_Demo/Src/main.c                  Main program
  - GPIO/GPIO_Demo/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with STMicroelectronics STM32F4xx-Nucleo rev C
    boards and can be easily tailored to any other supported device 
    and development board.

  - For This example to work properly, a 2.2 µF Capacitor must be present on the board in slot C26.
	

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
