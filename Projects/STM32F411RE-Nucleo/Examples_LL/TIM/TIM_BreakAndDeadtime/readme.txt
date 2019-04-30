/**
  @page TIM_BreakAndDeadtime TIM example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/TIM/TIM_BreakAndDeadtime/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_BreakAndDeadtime example.
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This example shows how to configure the TIMER to perform the following:
  – generate three center-aligned PWM and complementary PWM signals
  – insert a defined dead time value
  – use the break feature
  – lock the desired parameters
This example is based on the STM32F4xx TIM LL API;
peripheral initialization is done using LL unitary services
functions for optimization purpose (performance and size).

TIM1CLK is fixed to 100 MHz, the TIM1 Prescaler is set to have
TIM1 counter clock = 10 MHz.

The TIM1 auto-reload is set to generate PWM signal at 10 KHz:
               
The Three Duty cycles are computed as the following description: 
The channel 1 duty cycle is set to 50% so channel 1N is set to 50%.
The channel 2 duty cycle is set to 25% so channel 2N is set to 75%.
The channel 3 duty cycle is set to 12.5% so channel 3N is set to 87.5%.

A dead time equal to 4 us is inserted between 
the different complementary signals, and the Lock level 1 is selected.
  - The OCx output signal is the same as the reference signal except for the rising edge,
    which is delayed relative to the reference rising edge.
  - The OCxN output signal is the opposite of the reference signal except for the rising
    edge, which is delayed relative to the reference falling edge

Note that calculated duty cycles apply to the reference signal (OCxREF) from
which outputs OCx and OCxN are generated. As dead time insertion is enabled the
duty cycle measured on OCx will be slightly lower.
  
The break Polarity is used at High level.

The TIM1 waveforms can be displayed using an oscilloscope.

@par Directory contents 

  - TIM/TIM_BreakAndDeadtime/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_BreakAndDeadtime/Inc/main.h                  Header for main.c module
  - TIM/TIM_BreakAndDeadtime/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_BreakAndDeadtime/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_BreakAndDeadtime/Src/main.c                  Main program
  - TIM/TIM_BreakAndDeadtime/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F411RE Set-up
    - Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1  PA.08: connected to pin 8 of CN9 connector  
      - TIM1_CH1N PA.07: connected to pin 4 of CN5 connector 
      - TIM1_CH2  PA.09: connected to pin 1 of CN5 connector 
      - TIM1_CH2N PB.00: connected to pin 4 of CN8 connector 
      - TIM1_CH3  PA.10: connected to pin 3 of CN9 connector 
      - TIM1_CH3N PB.01: connected to pin 24 of CN10 connector 

    - Connect the TIM1 break to the GND. To generate a break event, switch this
      pin level from 0V to 3.3V.  
      - TIM1_BKIN  PA.06: connected to pin 5 of CN5 connector 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 