/**
  @page ADC_DualModeInterleaved  Use ADC1 and ADC2 in Dual interleaved mode and DMA mode3

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    ADC/ADC_DualModeInterleaved/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Dual interleaved mode and DMA mode3 example
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

How to use two ADC peripherals to perform conversions in dual interleaved mode.

This example provides a short description of how to use the ADC peripheral to
convert a regular channel in Dual interleaved mode using DMA in mode 3 with 5Msps.

DMA mode 3 is used in interleaved mode in 6-bit and 8-bit resolutions.

The Dual interleaved delay is configured 6 ADC clk cycles.

On each DMA request (two data items are available) two bytes representing two 
ADC-converted data items are transferred as a half word.
The data transfer order is similar to that of the DMA mode 2.

A DMA request is generated each time 2 data items are available
1st request: ADC_CDR[15:0] = (ADC2_DR[7:0] << 8) | ADC1_DR[7:0] 
2nd request: ADC_CDR[15:0] = (ADC2_DR[7:0] << 8) | ADC1_DR[7:0]

The ADC1 and ADC2 are configured to convert ADC Channel 12, with conversion 
triggered by software.
By this way, ADC channel 12 is converted each 6 cycles.

In this example, the system clock is 144MHz, APB2 =72MHz and ADC clock = APB2 /2.
Since ADCCLK= 36MHz and Conversion rate = 6 cycle
==> Conversion Time = 36M/6cyc = 6Msps.

STM32 Eval board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the ADC conversion is complete.
 - LED3 is ON when there is an error in initialization. 
  
 @note Refer to "simulation.xls" file to have the diagram simulation of the example.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Analog, ADC, Analog to Digital, Dual mode, Interleaved, Continuous conversion, Software Trigger, DMA, Measurement,

@par Directory contents 

  - ADC/ADC_DualModeInterleaved/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_DualModeInterleaved/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - ADC/ADC_DualModeInterleaved/Inc/main.h                  Main program header file  
  - ADC/ADC_DualModeInterleaved/Src/stm32f4xx_it.c          Interrupt handlers
  - ADC/ADC_DualModeInterleaved/Src/main.c                  Main program
  - ADC/ADC_DualModeInterleaved/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - ADC/ADC_DualModeInterleaved/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment 

  - This example runs on STM32F429xx/STM32F439xx devices.
    
  - This example has been tested with STM324x9I-EVAL RevB evaluation board and can be
    easily tailored to any other supported device and development board. 

  - STM324x9I-EVAL RevB Set-up
    - Connect PC.2 to a power supply (do not forget to connect the power supply 
      GND to the EVAL board GND)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
