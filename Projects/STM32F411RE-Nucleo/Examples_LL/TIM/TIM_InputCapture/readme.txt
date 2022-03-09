/**
  @page TIM_InputCapture TIM example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/TIM/TIM_InputCapture/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_InputCapture example.
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

This example shows how to use the TIM peripheral to measure the frequency of
a periodic signal provided either by an external signal generator or by
another timer instance; 
Example using the STM32F4xx TIM LL API, 
peripheral initialization done using LL unitary services functions
for optimization purpose (performance and size).
  
TIM3_CH1 is configured in input capture mode. The TIM3CLK frequency is set to 
its maximum value (Prescaler is 1) in order to get the best possible resolution.
So the TIM3 counter clock is SystemCoreClock.

SystemCoreClock is set to 100 MHz for STM32F4xx Devices.

The "uwMeasuredFrequency" variable contains the measured signal frequency.
It is calculated within the capture/compare 1 interrupt service routine.

The minimum frequency value to measure is TIM3 counter clock / TIMx_CCR1 MAX
                                              = 100 MHz / 65535

Due to TIM3_IRQHandler processing time (around 3.50us), the maximum
frequency value to measure is around 500 kHz.

TIM2_CH1 is configured to generate a PWM signal.  User push-button can be used to
change the frequency of this signal from 2 kHz up to 20 kHz by steps of 2 kHz.
It is then possible to run this example without a signal generator by connecting
TIM2_CH1 to TIM3_CH1.

@par Keywords

Timers, Frequency, Input, Capture, External Signal, Measurement

@par Directory contents 

  - TIM/TIM_InputCapture/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_InputCapture/Inc/main.h                  Header for main.c module
  - TIM/TIM_InputCapture/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_InputCapture/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_InputCapture/Src/main.c                  Main program
  - TIM/TIM_InputCapture/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F411RE Set-up
    - When no signal generator is used TIM2_CH1 can be connected to TIM3_CH1:
      - TIM3_CH1  PA.06: connected to pin 5 of CN5 connector  
      - TIM2_CH1  PA.05: connected to pin 6 of CN5 connector 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
