/**
  @page TIM_OnePulse TIM One Pulse example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_OnePulse/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM One Pulse example.      
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

  This example shows how to use the TIM peripheral to generate a single pulse when
  a rising edge of an external signal is received on the timer Input pin.

  TIM2CLK = SystemCoreClock/2, we want to get TIM2 counter clock at 42 MHz:
             - Prescaler = (TIM2CLK / TIM2 counter clock) - 1

  SystemCoreClock is set to 168 MHz.

  The Autoreload value is 65535 (TIM4->ARR), so the maximum frequency value to 
  trigger the TIM4 input is 42000000/65535 = 641 Hz.

  The TIM4 is configured as follows: 
  The One Pulse mode is used, the external signal is connected to TIM4 CH2 pin (PB.07), 
  the rising edge is used as active edge, the One Pulse signal is output 
  on TIM4_CH1 (PB.06).

  The TIM_Pulse defines the delay value, the delay value is fixed to:
  delay =  CCR1/TIM4 counter clock = 16383 / 42000000 = 390 us. 
  The (TIM_Period - TIM_Pulse) defines the One Pulse value, the pulse value is fixed to:
  One Pulse value = (TIM_Period - TIM_Pulse)/TIM4 counter clock 
                  = (65535 - 16383) / 42000000 = 1.170 ms.

The one pulse waveform can be displayed using an oscilloscope.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - TIM/TIM_OnePulse/system_stm32f4xx.c             STM32F4xx system clock configuration file
  - TIM/TIM_OnePulse/stm32f4xx_hal_conf.h           HAL configuration file
  - TIM/TIM_OnePulse/stm32f4xx_it.h                 Interrupt handlers header file
  - TIM/TIM_OnePulse/stm32f4xx_it.c                 Interrupt handlers
  - TIM/TIM_OnePulse/main.c                         Main program
  - TIM/TIM_OnePulse/main.h                         Main program header file


@par Hardware and Software environment

  - This example runs on STM32F407xx/417xx Devices.
    
  - This example has been tested with STM324xG-EVAL RevC evaluation board and can be
    easily tailored to any other supported device and development board. 

   - STM324xG-EVAL RevC Set-up 
    - Connect the external signal to the TIM4_CH2 pin (PB.07)
    - Connect the TIM4_CH1 (PB.06) pin to an oscilloscope to monitor the waveform.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
