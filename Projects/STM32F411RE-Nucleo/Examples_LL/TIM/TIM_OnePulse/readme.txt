/**
  @page TIM_OnePulse TIM example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/TIM/TIM_OnePulse/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_OnePulse example.
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

This example shows how to configure a timer to generate a positive pulse in
Output Compare mode with a length of tPULSE and after a delay of tDELAY;
This example is based on the STM32F4xx TIM LL API; peripheral initialization is done
using LL unitary services functions for optimization purpose (performance and size).

The pulse is generated on OC1.

This example uses 2 timer instances:

  - TIM3 generates a positive pulse of 50 us after a delay of 50 us. User push-button
    is used to start TIM3 counter. 
                                              ___ 
                                             |   |
    User push-button ________________________|   |________________________________
                                                            ___________
                                                           |           |
    OC1              ______________________________________|           |________
    (TIM3_CH1)                                <---50 us---><---50 us--->
                                                |            |_ uwMeasuredPulseLength
                                                |_ uwMeasuredDelay

  TIM3_CH1 delay and pulse length are measured every time a pulse is generated. 
  Both can be observed through the debugger by monitoring the variables uwMeasuredDelay and
  uwMeasuredPulseLength respectively.

  - TIM2 generates a positive pulse of 3 s after a delay of 2 s. TIM2 counter start
    is controlled through the slave mode controller. TI2FP2 signals is selected as
    trigger input meaning that TIM2 counter starts when a rising edge is detected on
    TI2FP2. The TIM2 output channel is mapped on the pin PA.05 (connected to LED2 on board
    NUCLEO-F411RE). Thus LED2 status (on/off) mirrors the timer output level (active v.s. inactive).
                                  ___
                                 |   |
    TI2 _________________________|   |_________________________________________
    (TIM2_CH2)
                                               ___________________________
                                              |                           |
    OC1 ______________________________________|                           |____
    (TIM2_CH1)                   <-----2s-----><----------3 s------------->
   


Both TIM3 and TIM2 are configured to generate a single pulse (timer counter 
stops automatically at the next update event (UEV).

Connecting TIM3 OC1 to TIM2 TI2 allows to trigger TIM2 counter by pressing
the User push-button.

@par Keywords

Timers, Output, signals, One Pulse, PWM, Oscilloscope, External signal, Autoreload, Waveform

@par Directory contents 

  - TIM/TIM_OnePulse/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_OnePulse/Inc/main.h                  Header for main.c module
  - TIM/TIM_OnePulse/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_OnePulse/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_OnePulse/Src/main.c                  Main program
  - TIM/TIM_OnePulse/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F411RE Set-up:
      - TIM3_CH1  PA.06: connected to pin 5 of CN5 connector  
      - TIM2_CH1  PA.05: connected to pin 6 of CN5 connector 
      - TIM2_CH2  PA.01: connected to pin 2 of CN8 connector 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
