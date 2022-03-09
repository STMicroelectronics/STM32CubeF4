/**
  @page RCC_ClockConfig RCC Clock Config example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    RCC/RCC_ClockConfig/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RCC Clock Config example.
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

This example describes how to use the RCC HAL API to configure the system clock (SYSCLK) 
and modify the clock settings in Run mode.

In this example, after startup SYSCLK is configured to the max frequency using the PLL with
HSI as clock source, the User push-button (connected to EXTI_Line15_10) will be 
used to change the PLL source: 
- from HSI to HSE bypass
- from HSE bypass to HSI

 @note In HSE bypass mode, input clock will come from the MCO from
       ST_LINK MCU. This frequency cannot be changed, and it is fixed
       at 8 MHz. To use MCO from ST_LINK you need to check the Board
       User Manual to know how to connect the MCO pin to the STM32 device.
        
Each time the User push-button is pressed; EXTI_Line15_10 interrupt is generated and in the ISR
the PLL oscillator source is checked using __HAL_RCC_GET_PLL_OSCSOURCE() macro:

- If the HSE bypass oscillator is selected as PLL source, the following steps will be followed to switch 
   the PLL source to HSI oscillator:
     a- Switch the system clock source to HSE bypass to allow modification of the PLL configuration
     b- Enable HSI Oscillator, select it as PLL source and finally activate the PLL
     c- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
     d- Disable the HSE bypass oscillator (optional, if the HSE bypass is no more needed by the application)

- If the HSI oscillator is selected as PLL source, the following steps will be followed to switch 
   the PLL source to HSE bypass oscillator:
     a- Switch the system clock source to HSI to allow modification of the PLL configuration
     b- Enable HSE bypass Oscillator, select it as PLL source and finally activate the PLL
     c- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
     d- Disable the HSI oscillator (optional, if the HSI is no more needed by the application)

In this example the SYSCLK  / 1 is outputted on the MCO1 pin(PA.08).

 @note The selected clock to output onto MCO must not exceed 100 MHz (the maximum I/O speed).

LED1 is toggled with a timing defined by the HAL_Delay() API.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

RCC, System, Clock Configuration, HSE bypass mode, HSI, System clock, Oscillator, PLL

@par Directory contents 

  - RCC/RCC_ClockConfig/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - RCC/RCC_ClockConfig/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - RCC/RCC_ClockConfig/Inc/main.h                  Header for main.c module  
  - RCC/RCC_ClockConfig/Src/stm32f4xx_it.c          Interrupt handlers
  - RCC/RCC_ClockConfig/Src/main.c                  Main program
  - RCC/RCC_ClockConfig/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with NUCLEO-F446ZE Rev.B
    board and can be easily tailored to any other supported device
    and development board.
    
  - Connect PA.08 (pin 23 in CN12) to an oscilloscope to monitor the waveform


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
