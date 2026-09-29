/**
  @page ADC_RegularConversion_DMA ADC3 conversion using DMA for Data transfer

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    ADC/ADC_RegularConversion_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC RegularConversion DMA example.
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

How to use the ADC1 and DMA to transfer continuously converted data from 
ADC1 to memory.

The ADC1 is configured to convert continuously channel8 (ADC1_IN8, pin PB.00 on STM32F407xx Device).

Each time an end of conversion occurs the DMA transfers(DMA2 Stream0, Channel 0), in circular mode,
the converted data from ADC1 DR register to the uhADCxConvertedValue variable.

In this example, the system clock is 84MHz, APB2 = 42MHz and ADC clock = APB2/2. 
Since ADC1 clock is 21 MHz and sampling time is set to 3 cycles, the conversion 
time to 12bit data is 12 cycles so the total conversion time is (12+3)/21= 0.71us(7.14Msps).

User can vary the ADC1 channel8(for STM32F401C-Discovery voltage probing external
power supply on PB.00 Gpio pin.

STM32 DISCO boards LEDs can be used to monitor the transfer status:
 - LED4 is ON when the conversion is complete.
 - LED5 is ON when there are an error in initialization.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Analog, ADC, Analog to Digital Converter, Regular Conversion, DMA, Continuous Conversion

@par Directory contents 

  - ADC/ADC_RegularConversion_DMA/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_RegularConversion_DMA/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - ADC/ADC_RegularConversion_DMA/Inc/main.h                  Main program header file  
  - ADC/ADC_RegularConversion_DMA/Src/stm32f4xx_it.c          Interrupt handlers
  - ADC/ADC_RegularConversion_DMA/Src/main.c                  Main program
  - ADC/ADC_RegularConversion_DMA/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - ADC/ADC_RegularConversion_DMA/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment 
  - This example runs on STM32F401xCx device.    
  - This example has been tested with STMicroelectronics STM32F401-Discovery RevB
    boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

@par Application note: Analog liquid level sensor (Fire water tank monitoring)
This ADC example can be adapted for analog‑output liquid level sensor:
1. Raw ADC to voltage(12‑bit resolution): Voltage = Raw_ADC * 3.3 / 4095
2. Water level percentage calculation:
WaterLevel_Percent = (V_read - V_empty) / (V_full - V_empty) * 100%

- V_empty: sensor output voltage when tank is empty
- V_full: sensor output voltage when tank is full

@note Hardware requirement: Sensor and STM32 must share common GND, otherwise ADC readings will drift.

 */

