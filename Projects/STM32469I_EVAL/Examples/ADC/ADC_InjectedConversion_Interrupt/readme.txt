/**
  @page ADC_InjectedConversion_Interrupt conversion using interrupt

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    ADC/ADC_InjectedConversion_Interrupt/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC RegularConversion interrupt example.
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

How to interrupt continuous ADC regular channel conversion using ADC injected 
channels, and how to get the result of this conversion.

The ADC3 is configured to convert continuously regular ADC_CHANNEL_10
and convert injected ADC_CHANNEL_12 (connected to PC.02) when injected conversion starts
(after one second of the start of regular channel).

Each time a regular channel end of conversion occurs an interrupt is generated 
and the converted data of ADC3 DR register is affected to the uhADCxConvertedRegValue 
variable in the ADC conversion complete callback function. After one second of
the start of regular ADC_CHANNEL_10 conversion the injected ADC_CHANNEL_12 conversion starts 
so regular conversion is temporary interrupted until the end of injected conversion. 
When the injected conversion is finished, an interrupt is generated, and the  
converted injected data of ADC3 DR register is affected to the uhADCxConvertedInjValue  
variable in the ADC conversion complete callback function.

Conversion time (reference manual, Reset and Clock control part):
  The function SystemClock_Config() configures the clock divider as follows:
  1) The system clock is 180 MHz.
  2) AHB Prescaler = 1 => AHB clock is 180 MHz.
  2) APB2 Prescaler = 2 => APB2 clock is 90 MHz.
  3) ADC Prescaler = 4 => ADC clock is 22.5 MHz. 

  Sampling time is set to ADC_SAMPLETIME_56CYCLES (56 cycles).
  ConvTime = Sampling time + 12 cycles ADC conversion time + 3 cycles sampling time.
           = 71 clock cycles
           = 3.2 us


User can vary the ADC_CHANNEL_10 voltage using the Eval Board potentiometer (RV1) connected to PC.0.
The converted values could be monitered through a debugger:
  - uhADCxConvertedRegValue for regular conversion
  - uhADCxConvertedInjValue for injected conversion

STM32 Eval boards LEDs can be used to monitor the transfer status:
  - LED3 is ON when there is an initialization error.

@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

Analog, ADC, Analog to Digital, Injected mode, Continuous conversion, Interrupt, Measurement,

@par Directory contents 

  - ADC/ADC_InjectedConversion_Interrupt/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_InjectedConversion_Interrupt/Inc/stm32f4xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_InjectedConversion_Interrupt/Inc/main.h                  Header for main.c module  
  - ADC/ADC_InjectedConversion_Interrupt/Src/stm32f4xx_it.c          DMA interrupt handlers
  - ADC/ADC_InjectedConversion_Interrupt/Src/main.c                  Main program
  - ADC/ADC_InjectedConversion_Interrupt/Src/stm32f4xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_InjectedConversion_Interrupt/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F469xx/STM32F479xx devices.
  
  - This example has been tested and validated with STM32469I-EVAL RevC board and can be
    easily tailored to any other supported device and development board.

  - STM32469I-EVAL RevC Set-up
    - Use the Potentiometer (RV1) of the Eval board (connected to  PC.0).
    - Connect PC.02 to a power supply (do not forget to connect the power supply 
      GND to the EVAL board GND)
    - Make sure that JP3 is fitted in 2-3 positions to use potentiometer.
      
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
