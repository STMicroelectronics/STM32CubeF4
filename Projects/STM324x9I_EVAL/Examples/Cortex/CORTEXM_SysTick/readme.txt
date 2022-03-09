/**
  @page CORTEXM_SysTick CORTEXM SysTick example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Cortex/CORTEXM_SysTick/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CortexM SysTick example.
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

  This example shows how to use the default configuration of SysTick with a time 
  base equal to 1 ms in order to insert a delay between LEDs toggling. 
  The system clock is set to 180 MHz for STM32F42/43xxx devices, the SysTick is 
  clocked by the AHB clock(HCLK).

  A "HAL_Delay" function is implemented based on the SysTick end-of-count event.
  Four LEDs are toggled with a timing defined by the HAL_Delay function.
  
  @note: Caution, the SysTick time base 1 msec must not be changed due to use 
         of these time base by HAL driver.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, Cortex, Time base, Systick, HCLK 

@par Directory contents 

  - Cortex/CORTEXM_SysTick/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - Cortex/CORTEXM_SysTick/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - Cortex/CORTEXM_SysTick/Inc/main.h                  Main program header file  
  - Cortex/CORTEXM_SysTick/Src/stm32f4xx_it.c          Interrupt handlers
  - Cortex/CORTEXM_SysTick/Src/main.c                  Main program
  - Cortex/CORTEXM_SysTick/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment 

  - This example runs on STM32F42xxx/STM32F43xxx devices.
    
  - This example has been tested with STM324x9I-EVAL RevB evaluation board and can be
    easily tailored to any other supported device and development board. 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
