/**
  @page ADC_DualModeInterleaved  Use ADC1 and ADC2 in Dual interleaved mode and DMA mode3

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    ADC/ADC_DualModeInterleaved/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Dual interleaved mode and DMA mode3 example
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

How to use two ADC peripherals to perform conversions in dual interleaved mode.

DMA mode 3 is used in interleaved mode in 6-bit and 8-bit resolutions.

The Dual interleaved delay is configured to 6 ADC clk cycles (ADC_TWOSAMPLINGDELAY_6CYCLES).

On each DMA request (two data items are available) two bytes representing two 
ADC-converted data items are transferred as a half word to uhADCDualConvertedValue variable.
The data transfer order is similar to that of the DMA mode 2.

A DMA request is generated each time 2 data items are available
1st request: ADC_CDR[15:0] = (ADC2_DR[7:0] << 8) | ADC1_DR[7:0] 
2nd request: ADC_CDR[15:0] = (ADC2_DR[7:0] << 8) | ADC1_DR[7:0]

The ADC1 and ADC2 are configured to convert ADC Channel 1, with conversion 
triggered by software.
By this way, ADC channel 1 is converted each 6 cycles.

In this example, the system clock is 180MHz, APB2 = 90MHz and ADC clock = APB2/4.
Since ADCCLK= 22.5MHz and conversion rate = 6 cycles
==> Conversion Time = 22.5M/6cyc = 3.75Msps.

STM32 Eval boards LEDs can be used to monitor the transfer status:
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

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with STM32446E-EVAL evaluation board and can be
    easily tailored to any other supported device and development board. 

  - STM32446E-EVAL Set-up
    - Connect PA.1 to a power supply (do not forget to connect the power supply 
      GND to the EVAL board GND)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
