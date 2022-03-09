/**
  @page Templates  Description of the Templates example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Templates/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Templates example.
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

This project provides a reference template that can be used to build any firmware application.

This directory provides a reference template project that can be used to build any firmware application for
STM32F401xEx devices using STM32CubeF4 HAL and running on STM32F4xx-Nucleo board from STMicroelectronics. 

Two workspace are provided for STM32F4xx-Nucleo (STM32F411xEx devices) configuration.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The clock setting is configured to have the max product performance (max clock frequency) 
      so not optimized in term of power consumption.
      
@par Directory contents 

  - Templates/Src/main.c                 Main program
  - Templates/Src/system_stm32f4xx.c     STM32F4xx system clock configuration file
  - Templates/Src/stm32f4xx_it.c         Interrupt handlers 
  - Templates/Src/stm32f4xx_hal_msp.c    HAL MSP module
  - Templates/Inc/main.h                 Main program header file  
  - Templates/Inc/stm32f4xx_hal_conf.h   HAL Configuration file
  - Templates/Inc/stm32f4xx_it.h         Interrupt handlers header file

        
@par Hardware and Software environment  

  - This example runs on STM32F411xEx devices.
    
  - This example has been tested with STMicroelectronics STM32F4xx-Nucleo RevC 
    boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
