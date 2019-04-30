/**
  @page TIM_OCActive TIM_OCActive example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_OCActive/readme.txt 
  * @author  MCD Application Team
  * @brief   This example shows how to configure the Timer to generate four 
  *          delayed signals.
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

  This example shows how to configure the TIM peripheral to generate four different signals with four
  different delays.

  The TIM3 frequency is set to SystemCoreClock, and the objective is
  to get TIM3 counter clock at 10 kHz so the Prescaler is computed as following:
     - Prescaler = (TIM3CLK /TIM3 counter clock) - 1
   
  SystemCoreClock is set to 180 MHz for STM32F4xx Devices.

  The TIM3 CCR1 register value is equal to 10000:
  TIM3_CH1 delay = CCR1_Val/TIM3 counter clock  = 1s
  so the TIM3 Channel 1 generates a signal with a delay equal to 1s.

  The TIM3 CCR2 register value is equal to 5000:
  TIM3_CH2 delay = CCR2_Val/TIM3 counter clock = 500 ms
  so the TIM3 Channel 2 generates a signal with a delay equal to 500 ms.

  The TIM3 CCR3 register value is equal to 2500:
  TIM3_CH3 delay = CCR3_Val/TIM3 counter clock = 250 ms
  so the TIM3 Channel 3 generates a signal with a delay equal to 250 ms.

  The TIM3 CCR4 register value is equal to 1250:
  TIM3_CH4 delay = CCR4_Val/TIM3 counter clock = 125 ms
  so the TIM3 Channel 4 generates a signal with a delay equal to 125 ms.

  The delay correspond to the time difference between LED2 connected to PB.07 (Reference) and
  TIM3_CHx signal rising edges.

@note Delay values mentioned above are theoretical (obtained when the system clock frequency 
      is exactly 180 MHz). Since the generated system clock frequency may vary from one board to another observed
      delay might be slightly different.
	  
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - TIM/TIM_OCActive/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_OCActive/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_OCActive/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OCActive/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_OCActive/Src/main.c                  Main program
  - TIM/TIM_OCActive/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_OCActive/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F429xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-F429ZI  Rev.B 
    board and can be easily tailored to any other supported device 
    and development board.      

  - NUCLEO-F429ZI  Rev.B Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - Use LED2 connected to PB.07 (Reference)
      - PB.04: (TIM3_CH1) (connected to PB.04 (pin 19 in CN7 connector))
      - PB.05: (TIM3_CH2) (connected to PB.05 (pin 13 in CN7 connector))
      - PB.00: (TIM3_CH3) (connected to PB.00 (pin 31 in CN10 connector))
      - PB.01: (TIM3_CH4) (connected to PB.01 (pin 7 in CN10 connector))

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 
 
You should see these waveforms on oscilloscope :

 CH1                                                            ________________
 _______________________________________________________________|
           <---------------------- 1sec------------------------->
           
 CH2                                   __________________________________________
 ______________________________________|
           <------------500ms--------->
 
 CH3                        _____________________________________________________
 ___________________________|
           <----250ms------->
 
 CH4                  ____________________________________________________________
 _____________________|
           <--125ms--->

 TRIG      ______________________________________________________________________
 __________|


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
