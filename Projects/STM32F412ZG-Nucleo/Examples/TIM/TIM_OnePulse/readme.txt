/**
  @page TIM_OnePulse TIM One Pulse example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_OnePulse/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM One Pulse example.      
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

This example shows how to use the TIM peripheral to generate a single pulse when
a rising edge of an external signal is received on the timer Input pin.


Clock setup for TIM3
================================

  TIM3CLK = SystemCoreClock = 100 MHz.
  
  Prescaler = (TIM3CLK /TIM3 counter clock) - 1
  
  The prescaler value is computed in order to have TIM3 counter clock 
  set at 25000000 Hz.
  
  The Autoreload value is 65535 (TIM3->ARR), so the maximum frequency value to 
  trigger the TIM3 input is 25000000/65535 [Hz].
 
Configuration of TIM3 in One Pulse Mode
===================================================
 
  - The external signal is connected to TIM3_CH2 pin (PB.05), 
    and a rising edge on this input is used to trigger the Timer.
  - The One Pulse signal is output on TIM3_CH1 (PB.04).

  The delay value is fixed to:
   - Delay =  CCR1/TIM3 counter clock 
           = 16383 / 25000000 [sec]
           
  The pulse value is fixed to : 
   - Pulse value = (TIM_Period - TIM_Pulse)/TIM3 counter clock  
                 = (65535 - 16383) / 25000000 [sec]

  The one pulse waveform can be displayed using an oscilloscope and it looks
  like this.
  
                               ____
                               |   |
  CH2 _________________________|   |__________________________________________
 
                                             ___________________________
                                            |                           |
  CH1 ______________________________________|                           |_____
                               <---Delay----><------Pulse--------------->
  


@par Keywords

Timers, Output, signals, One Pulse, PWM, Oscilloscope, External signal, Autoreload, Waveform

@par Directory contents 

  - TIM/TIM_OnePulse/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_OnePulse/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_OnePulse/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OnePulse/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_OnePulse/Src/main.c                  Main program
  - TIM/TIM_OnePulse/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_OnePulse/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

   - This example runs on STM32F412Zx devices.
   - In this example, the clock is set to 100 MHz.
    
  - This example has been tested with NUCLEO-F412ZG Rev B board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F412ZG Rev B Set-up
   - Connect the external signal to the TIM3_CH2 pin (PB.05) (CN7, pin 13)
   - Connect the TIM3_CH1 pin(PB.04) (CN7, pin 19) to an oscilloscope to monitor the waveform.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 


 */
