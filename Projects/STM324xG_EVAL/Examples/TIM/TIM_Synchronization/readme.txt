/**
  @page TIM_Synchronization Timers Synchronization example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_Synchronization/readme.txt 
  * @author  MCD Application Team
  * @brief   How to command 2 Timers as slaves (TIM3 & TIM4) using a Timer
  *          as master (TIM1)
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

This example shows how to synchronize TIM1 and Timers (TIM3 and TIM4) in parallel mode.

Timers synchronization in parallel mode:


                                                       ___________ 
                                                 ITR0 |  SLAVE 1  |
                                     _________________|    TIM3   |
   ___________                      |                 |___________|
  |   MASTER  |TRGO_Update          |                 
  |    TIM1   |---------------------|               
  |___________|                     |                  ___________ 
                                    |_________________|  SLAVE 2  |
                                                 ITR0 |    TIM4   |
                                                      |___________|
                         

1/ TIM1 is configured as Master Timer:
   - PWM Mode is used
   - The TIM1 Update event is used as Trigger Output 
 
2/ TIM3 and TIM4 are slaves for TIM1,
   - PWM Mode is used
   - The ITR0(TIM1) is used as input trigger for both slaves
   - Gated mode is used, so starts and stops of slaves counters
     are controlled by the Master trigger output signal(update event).

The TIM1 counter clock is 168 MHz.

  The Master Timer TIM1 is running at:
  TIM1 frequency = TIM1 counter clock / (TIM1_Period + 1) = 656 KHz
  and the duty cycle is equal to: TIM1_CCR1/(TIM1_ARR + 1) = 50%

  The TIM3 is running at: 
  (TIM1 frequency)/ ((TIM3 period +1)* (TIM1_Repetition_Counter+1)) = 43.730 KHz and
  a duty cycle equal to TIM3_CCR1/(TIM3_ARR + 1) = 33.3%

  The TIM4 is running at:
  (TIM1 frequency)/ ((TIM4 period +1)* (Repetition_Counter+1)) = 65.600 KHz and
  a duty cycle equal to TIM4_CCR1/(TIM4_ARR + 1) = 50%


The PWM waveform can be displayed using an oscilloscope.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - TIM/TIM_Synchronization/Inc/stm32f4xx_hal_conf.h      HAL configuration file
  - TIM/TIM_Synchronization/Inc/stm32f4xx_it.h            Interrupt handlers header file
  - TIM/TIM_Synchronization/Inc/main.h                    Main program header file
  - TIM/TIM_Synchronization/Src/stm32f4xx_it.c            Interrupt handlers
  - TIM/TIM_Synchronization/Src/main.c                    Main program
  - TIM/TIM_Synchronization/Src/stm32f4xx_hal_msp.c       HAL MSP module
  - TIM/TIM_Synchronization/Src/system_stm32f4xx.c        STM32F4xx system clock configuration file  


@par Hardware and Software environment

  - This example runs on STM32F407xx/417xx devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device and development board

  - STM324xG-EVAL RevC Set-up 
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM1 CH1 (PA.08)
      - TIM3 CH1 (PC.06)
      - TIM4 CH1 (PB.06)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
