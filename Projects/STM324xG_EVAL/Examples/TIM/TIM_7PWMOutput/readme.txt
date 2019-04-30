/**
  @page TIM_7PWMOutput TIM 7 PWM Output example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_7PWMOutput/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWM signals generation using Timer3 example.
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

This example shows how to configure the TIM1 peripheral to generate 7 PWM signals 
with 4 different duty cycles (50%, 37.5%, 25% and 12.5%).
  
TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
SystemCoreClock is set to 168 MHz.
  
The objective is to generate 7 PWM signal at 17.57 KHz:
  - TIM1_Period = (SystemCoreClock / 17570) - 1
The channel 1 and channel 1N duty cycle is set to 50%
The channel 2 and channel 2N duty cycle is set to 37.5%
The channel 3 and channel 3N duty cycle is set to 25%
The channel 4 duty cycle is set to 12.5%
The Timer pulse is calculated as follows:
  
  - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
  
The TIM1 waveforms can be displayed using an oscilloscope.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - TIM/TIM_7PWMOutput/Inc/stm32f4xx_hal_conf.h     HAL configuration file
  - TIM/TIM_7PWMOutput/Inc/stm32f4xx_it.h           Interrupt handlers header file
  - TIM/TIM_7PWMOutput/Inc/main.h                   Main program header file  
  - TIM/TIM_7PWMOutput/Src/stm32f4xx_it.c           Interrupt handlers
  - TIM/TIM_7PWMOutput/Src/main.c                   Main program
  - TIM/TIM_7PWMOutput/Src/stm32f4xx_hal_msp.c      HAL MSP module
  - TIM/TIM_7PWMOutput/Src/system_stm32f4xx.c       STM32F4xx system clock configuration file

     
@par Hardware and Software environment 

  - This example runs on STM32F407xx/417xx devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device and development board

  - STM324xG-EVAL RevC Set-up 
    - Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1  pin (PA.08)
      - TIM1_CH1N pin (PB.13)
      - TIM1_CH2  pin (PE.11)
      - TIM1_CH2N pin (PB.14)
      - TIM1_CH3  pin (PA.10)
      - TIM1_CH3N pin (PB.15)
      - TIM1_CH4  pin (PE.14)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
