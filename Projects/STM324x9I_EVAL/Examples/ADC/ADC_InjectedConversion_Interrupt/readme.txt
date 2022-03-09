/**
  @page ADC_InjectedConversion_Interrupt ADC3 conversion using Interrupt

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    ADC/ADC_InjectedConversion_Interrupt/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC InjectedConversion Interrupt example.
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

This example describes how to interrupt continuous ADC3 regular channel8 conversion 
using ADC3 injected channel12 and how to get the converted value of this conversion.

The ADC3 is configured to convert continuously regular channel8(for STM324x9I-EVAL RevB)
and convert injected channel12 (connected to PC.2) when injected conversion starts
(after one second of the start of regular channel).

Each time a regular channel end of conversion occurs an interrupt is generated 
and the converted data of ADC3 DR register is affected to the uhADCxConvertedRegValue 
variable in the ADC conversion complete callback function. After one second of
the start of regular channel8 conversion the injected channel12 conversion starts 
so regular conversion is temporary interrupted until the end of injected conversion. 
When the injected conversion is finished, an interrupt is generated, and the  
converted injected data of ADC3 DR register is affected to the uhADCxConvertedInjValue  
variable in the ADC conversion complete callback function.     

In this example, the system clock is 144MHz, APB2 = 72MHz and ADC clock = APB2/2. 
Since ADC3 clock is 36 MHz and sampling time is set to 3 cycles, the conversion 
time to 12bit data is 12 cycles so the total conversion time is (12+3)/36= 0.41us(2.4Msps).

STM32 Eval boards LEDs can be used to monitor the transfer status:
  - LED3 is ON when there are an error in initialization.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Analog, ADC, Analog to Digital, Injected mode, Continuous conversion, Interrupt, Measurement,

@par Directory contents 

  - ADC/ADC_InjectedConversion_Interrupt/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_InjectedConversion_Interrupt/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - ADC/ADC_InjectedConversion_Interrupt/Inc/main.h                  Main program header file  
  - ADC/ADC_InjectedConversion_Interrupt/Src/stm32f4xx_it.c          Interrupt handlers
  - ADC/ADC_InjectedConversion_Interrupt/Src/main.c                  Main program
  - ADC/ADC_InjectedConversion_Interrupt/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - ADC/ADC_InjectedConversion_Interrupt/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment 

  - This example runs on STM32F429xx/STM32F439xx devices.
    
  - This example has been tested with STM324x9I-EVAL RevB evaluation board and can be
    easily tailored to any other supported device and development board.
  
  - STM324x9I-EVAL RevB Set-up
    - Use the Potentiometer (RV1) of the Eval board (connected to PF.10).
    - Connect PC.2 to a power supply (do not forget to connect the power supply 
      GND to the EVAL board GND)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
