/**
  @page RCC_ClockConfig RCC Clock Configuration example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    RCC/RCC_ClockConfig/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RCC Clock Configuration example.
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
HSI as clock source, the STM32F411RE-Nucleo board Key button (connected to EXTI Line13) will be 
used to change the PLL source: from HSI to HSE, and vice versa. 
Each time the Key button is pressed; EXTI Line13 interrupt is generated and the 
PLL oscillator source is checked using __HAL_RCC_GET_PLL_OSCSOURCE() macro:

1- If the HSI oscillator is selected as PLL source, the following steps will be followed to switch 
   the PLL source to HSE oscillator:
     a- Switch the system clock source to HSI to allow modification of the PLL configuration
     b- Depending on the uncommented define in the main.h file, one of the two following 
        possibilities is applied:
        - if defined "HSE_CRYSTAL": the HSE Oscillator is then selected as the input 
           PLL source and consequently we need to Enable it before selecting it as PLL
           source and finally activate the PLL 
        - if defined "HSE_BYPASS": then the input clock will come from the MCO from
          ST_LINK MCU-STM32F103. This frequency cannot be changed, and it is fixed
          at 8 MHz, connected to PF1/PH0-OSC_IN of STM32F4xx device.
          This will require to enable the HSE Bypass before selecting it as PLL
          source and finally activate the PLL
     c- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
     d- Disable the HSI oscillator (optional, if the HSI is no more needed by the application)
     
2- If the HSE oscillator is selected as PLL source, the following steps will be followed to switch 
   the PLL source to HSI oscillator:
     a- Switch the system clock source to HSE to allow modification of the PLL configuration
     b- Enable HSI Oscillator, select it as PLL source and finally activate the PLL
     c- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
     d- Disable the HSE oscillator (optional, if the HSE is no more needed by the application)

In this example the SYSCLK divided by 2 is outputted on the MCO2 pin(PC9).

 @note The selected clock to output onto MCO must not exceed 100 MHz (the maximum I/O speed).
 @note The SysTick 1 msec interrupt is required for the HAL process (Timeout management);
       by default the configuration is done using the HAL_Init() API, and when the
       system clock configuration is updated the SysTick configuration will be 
       adjusted by the HAL_RCC_ClockConfig() API.

One LED is toggled with a timing defined by the HAL_Delay() API.


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

@note The clock setting is configured to have the max product performance (max clock frequency) 
      so not optimized in term of power consumption.
 
@par Keywords

RCC, System, Clock Configuration, HSE bypass mode, HSI, System clock, Oscillator, PLL

@par Directory contents  

  - RCC/RCC_ClockConfig/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - RCC/RCC_ClockConfig/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - RCC/RCC_ClockConfig/Inc/main.h                  Main program header file  
  - RCC/RCC_ClockConfig/Src/stm32f4xx_it.c          Interrupt handlers
  - RCC/RCC_ClockConfig/Src/main.c                  Main program
  - RCC/RCC_ClockConfig/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment

  - This example runs on STM32F411xEx devices.
    
  - This example has been tested with STMicroelectronics STM32F4xx-Nucleo RevC 
    boards and can be easily tailored to any other supported device 
    and development board.
    
  - STM32F4xx-Nucleo RevC Set-up
    - Use LED2 connected to PA.05 pins.
    - Use Key push button connected to PC.13 pin.
    - HSE crystal (8MHZ) need to be mounted on Nucleo board if the HSE_CRYSTAL is 
      uncommented on in the main.h file
      Otherwise, if HSE_BYPASS is selected, the following configuration on the board
      need to be applied:
      – Solder bridges SB54 and SB55 have to be OFF
      – Resistor R35 has to be removed
      – Solder bridges SB16 and SB50 have to be ON.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 


 */
