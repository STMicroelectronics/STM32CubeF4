/**
  @page PWR_BOR PWR Brown Out Reset (BOR) example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    PWR/PWR_BOR/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR Brown Out Reset (BOR) example.
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

This example shows how to configure the programmable BOR thresholds using the 
FLASH option bytes.

By maintaining the Key push-button pressed at Reset, the new BOR level will be 
programmed. To modify the BOR threshold, select the voltage range using the 
appropriate line "#define BOR_LEVEL OB_BOR_XXXX" in main.c file.

STM32 Eval boards LEDs can be used to monitor the example status:
  - LED1 is toggling to indicate that the reset is released and the system is in RUN
    mode otherwise the BOR is maintaining the reset
  - LED3 is ON when there is an error

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Power, PWR, EXTI, BOR, Interrupt, Wakeup, External reset

@par Directory contents 

  - PWR/PWR_BOR/Inc/stm32f4xx_hal_conf.h     HAL configuration file
  - PWR/PWR_BOR/Inc/stm32f4xx_it.h           Interrupt handlers header file
  - PWR/PWR_BOR/Inc/main.h                   header file for main.c
  - PWR/PWR_BOR/Src/system_stm32f4xx.c       STM32F4xx system clock configuration file
  - PWR/PWR_BOR/Src/stm32f4xx_it.c           Interrupt handlers
  - PWR/PWR_BOR/Src/main.c                   Main program
  - PWR/PWR_BOR/Src/stm32f4xx_hal_msp.c      HAL MSP module


@par Hardware and Software environment

  - This example runs on STM32F407xx/417xx devices.  
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM324xG-EVAL RevC Set-up
    - Use LED1 and LED3 connected respectively to PG.06 and PI.09 pins
    - Use the Key push-button connected to pin PG15 (EXTI_Line15)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
