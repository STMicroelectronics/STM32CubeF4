/**
  @page HAL_TimeBase HAL Time base example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    HAL/HAL_TimeBase/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the HAL time base example.
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

This example describes how to customize the HAL time base using a general 
purpose timer(TIM6) instead of Systick as main source of time base.

Time base duration is kept unchanged: 1ms  since PPP_TIMEOUT_VALUEs are defined 
and handled in milliseconds basis.

The example brings, in user file, a new implementation of the following HAL weak functions:

HAL_InitTick() 
HAL_SuspendTick()
HAL_ResumeTick()

This implementation will overwrite native implementation in stm32f4xx_hal.c
and so user functions will be invoked instead when called.

The following time base functions are kept as implemented natively:

HAL_IncTick()
HAL_Delay()

When user pushes the Push-button, Tick increment is suspended if it is already
enabled, else it will be resumed.
In an infinite loop, LED2 toggles spaced out over 1s delay.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in TIM6 ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the TIM6 interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the TIM6 interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the TIM6 time base is always set to 1 millisecond
      to have correct HAL operation.

@note The clock setting is configured to have the max product performance (max clock frequency) 
      so not optimized in term of power consumption.

@par Keywords

System, General purpose Timer, Time base, HAL

@par Directory contents

  - HAL/HAL_TimeBase/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - HAL/HAL_TimeBase/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - HAL/HAL_TimeBase/Inc/main.h                  Main program header file  
  - HAL/HAL_TimeBase/Src/stm32f4xx_it.c          Interrupt handlers
  - HAL/HAL_TimeBase/Src/main.c                  Main program
  - HAL/HAL_TimeBase/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with STMicroelectronics STM32F4xx-Nucleo Rev C 
    boards and can be easily tailored to any other supported device 
    and development board.

  - For This example to work properly, a 2.2 µF Capacitor must be present on the board in slot C26.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
