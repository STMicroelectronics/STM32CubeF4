/**
  @page TIM_ParallelSynchro Timers Synchronization example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_ParallelSynchro/readme.txt 
  * @author  MCD Application Team
  * @brief   How to command 2 Timers as slaves (TIM3 & TIM4) using a Timer
  *          as master (TIM2)

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

This example shows how to synchronize TIM2 and Timers (TIM3 and TIM4) in parallel mode.

Timers synchronisation in parallel mode:


                                                       ___________ 
                                                 ITR1 |  SLAVE 1  |
                                     _________________|    TIM3   |
   ___________                      |                 |___________|
  |   MASTER  |TRGO_Update          |                 
  |    TIM2   |---------------------|               
  |___________|                     |                  ___________ 
                                    |_________________|  SLAVE 2  |
                                                 ITR1 |    TIM4   |
                                                      |___________|
                         

1/ TIM2 is configured as Master Timer:
   - PWM Mode is used
   - The TIM2 Update event is used as Trigger Output 
 
2/ TIM3 and TIM4 are slaves for TIM2,
   - PWM Mode is used
   - The ITR1(TIM2) is used as input trigger for both slaves
   - Gated mode is used, so starts and stops of slaves counters
     are controlled by the Master trigger output signal(update event).

The TIM2 counter clock is 180 MHz.

  The Master Timer TIM2 is running at:
  TIM2 frequency = TIM2 counter clock / (TIM2_Period + 1) = 351.562 KHz and
  a the duty cycle equal to: TIM2_CCR1/(TIM2_ARR + 1) = 25%

  The TIM3 is running at: 
  (TIM2 frequency)/ (TIM3 period +1) = 35.156 KHz and
  a duty cycle equal to TIM3_CCR1/(TIM3_ARR + 1) = 30%

  The TIM4 is running at:
  (TIM2 frequency)/ (TIM4 period +1) = 70.312 KHz and
  a duty cycle equal to TIM4_CCR1/(TIM4_ARR + 1) = 60%


The PWM waveform can be displayed using an oscilloscope.


@par Directory contents 

  - TIM/TIM_ParallelSynchro/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_ParallelSynchro/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_ParallelSynchro/Inc/main.h                  Header for main.c module  
  - TIM/TIM_ParallelSynchro/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_ParallelSynchro/Src/main.c                  Main program
  - TIM/TIM_ParallelSynchro/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_ParallelSynchro/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with STM32446E-EVAL board and can be
    easily tailored to any other supported device and development board.      

  - STM32446E-EVAL Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM2 CH1 (PA.00)
      - TIM3 CH1 (PC.06)
      - TIM4 CH3 (PB.08)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
