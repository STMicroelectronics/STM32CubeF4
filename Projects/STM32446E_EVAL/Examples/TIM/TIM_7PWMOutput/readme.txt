/**
  @page TIM_7PWMOutput TIM Complementary Signals example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_7PWMOutput/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWM signals generation using Timer Example.
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

This example shows how to configure the TIM1 peripheral to generate 7 PWM signals 
with 4 different duty cycles (50%, 37.5%, 25% and 12.5%).

TIM1CLK is fixed to SystemCoreClock, the TIM1 Prescaler is set to 0 to have
TIM1 counter clock = SystemCoreClock.

The objective is to generate 7 PWM signals at 18kHz:
  - TIM1_Period = (TIM1 counter clock / 18000) - 1
               
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
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timers, PWM, Duty Cycle, Waveform, Oscilloscope, Output, Signal, commutation, timing mode

@par Directory contents 

  - TIM/TIM_7PWMOutput/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_7PWMOutput/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_7PWMOutput/Inc/main.h                  Header for main.c module  
  - TIM/TIM_7PWMOutput/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_7PWMOutput/Src/main.c                  Main program
  - TIM/TIM_7PWMOutput/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_7PWMOutput/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with STMicroelectronics STM32446E-EVAL
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32446E-EVAL Set-up
    - Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1  pin (PA.08)(pin 1 in CN6 connector)  
      - TIM1_CH1N pin (PB.13)  
      - TIM1_CH2  pin (PA.09)  
      - TIM1_CH2N pin (PB.14)(Pin 7 in CN6 connector)  
      - TIM1_CH3  pin (PA.10)  
      - TIM1_CH3N pin (PB.15)(pin 5 in CN6 connector)
      - TIM1_CH4  pin (PA.11) 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
