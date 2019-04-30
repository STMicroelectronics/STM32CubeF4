/**
  @page TIM_DMABurst TIM DMA Burst example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_DMABurst/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM DMA example.
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

This example shows how to update the TIM1 channel1 period and the duty cycle 
using the TIM1 DMA burst feature.

Every update DMA request, the DMA will do 3 transfers of half words into Timer 
registers beginning from ARR register.
On the DMA update request, 0x0FFF will be transferred into ARR, 0x0000 
will be transferred into RCR, 0x0555 will be transferred into CCR1. 

The TIM1CLK frequency is set to SystemCoreClock (Hz), to get TIM1 counter
clock at 24 MHz the Prescaler is computed as following:
- Prescaler = (TIM1CLK / TIM1 counter clock) - 1

SystemCoreClock is set to 168 MHz.

The TIM1 period is 5.8 ms: TIM1 Frequency = TIM1 counter clock/(ARR + 1)
                                          = 24 MHz / 4096 = 5.8 KHz

The TIM1 CCR1 register value is equal to 0x555, so the TIM1 Channel 1 generates a 
PWM signal with a frequency equal to 5.8 KHz and a duty cycle equal to 33.33%:
TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR + 1)* 100 = 33.33%

The PWM waveform can be displayed using an oscilloscope. 


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - TIM/TIM_DMABurst/system_stm32f4xx.c         STM32F4xx system clock configuration file
  - TIM/TIM_DMABurst/stm32f4xx_hal_conf.h       HAL configuration file
  - TIM/TIM_DMABurst/stm32f4xx_it.h             Interrupt handlers header file
  - TIM/TIM_DMA/stm32f4xx_it.c                  Interrupt handlers
  - TIM/TIM_DMABurst/main.c                     Main program
  - TIM/TIM_DMABurst/main.h                     Main program header file


@par Hardware and Software environment

  - This example runs on STM32F407xx/417xx devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL RevC  
    evaluation boards and can be easily tailored to any other supported device and development board

  - STM324xG-EVAL RevC Set-up
    - Connect the following pins to an oscilloscope to monitor the different
      waveforms:  
        - TIM1 CH1 (PA.08) 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
