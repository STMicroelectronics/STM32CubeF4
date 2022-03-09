/**
  @page TIM_TimeBase Time Base example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_TimeBase/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM Time Base example
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

This example shows how to configure the TIM peripheral to generate a time base of 
one second with the corresponding Interrupt request.

In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2 (Hz)
To get TIM3 counter clock at 10 KHz, the Prescaler is computed as following:
   - Prescaler = (TIM3CLK / TIM3 counter clock) - 1

SystemCoreClock is set to 168MHz for STM32F4xx Devices.

The TIM3 ARR register value is equal to 10000 - 1, 
Update rate = TIM3 counter clock / (Period + 1) = 1 Hz,
So the TIM3 generates an interrupt each 1 s

When the counter value reaches the auto-reload register value, the TIM updates 
interrupt is generated and, in the handler routine, PD.12 is toggled at the following frequency: 0.5Hz. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timer, TIM, Time Base, Interrupt, Clock source

@par Directory contents 

  - TIM/TIM_TimeBase/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_TimeBase/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_TimeBase/Inc/main.h                  Main program header file  
  - TIM/TIM_TimeBase/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_TimeBase/Src/main.c                  Main program
  - TIM/TIM_TimeBase/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - TIM/TIM_TimeBase/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment 

  - This example runs on STM32F407xx devices.
    
  - This example has been tested with STM32F4-Discovery RevB & RevC board (MB997) and can be
    easily tailored to any other supported device and development board.
  
  - STM32F4-Discovery RevB & RevCboard (MB997) Set-up
    - Use LED4(GREEN) connected respectively PD.12 pin and connect them on an oscilloscope to show 
      the different Time Base signals.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
