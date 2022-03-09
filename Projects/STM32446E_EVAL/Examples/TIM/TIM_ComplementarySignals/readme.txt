/**
  @page TIM_ComplementarySignals TIM Complementary Signals example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_ComplementarySignals/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM Complementary Signals example.
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

This example shows how to configure the TIM1 peripheral to generate three 
complementary TIM1 signals, to insert a defined dead time value, to use the break 
feature and to lock the desired parameters.

TIM1CLK is fixed to SystemCoreClock, the TIM1 Prescaler is set to have
TIM1 counter clock = 18MHz.

The objective is to generate PWM signal at 10 KHz:
  - TIM1_Period = (TIM1 counter clock / 10000) - 1
               
The Three Duty cycles are computed as the following description: 
The channel 1 duty cycle is set to 50% so channel 1N is set to 50%.
The channel 2 duty cycle is set to 25% so channel 2N is set to 75%.
The channel 3 duty cycle is set to 12.5% so channel 3N is set to 87.5%.
The Timer pulse is calculated as follows:
  - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100

A dead time equal to 100/SystemCoreClock (around 0.5us) is inserted between 
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

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timers, PWM, Complementary signals, Master, Slave, Duty Cycle, Waveform, Oscilloscope, Output, Signal,
dead time, break lock

@par Directory contents 

  - TIM/TIM_ComplementarySignals/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_ComplementarySignals/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_ComplementarySignals/Inc/main.h                  Header for main.c module  
  - TIM/TIM_ComplementarySignals/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_ComplementarySignals/Src/main.c                  Main program
  - TIM/TIM_ComplementarySignals/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_ComplementarySignals/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with STMicroelectronics STM32446E-EVAL
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32446E-EVAL Set-up
    - Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1  pin  PA.08(Pin 1 in CN6 connector)  
      - TIM1_CH1N pin  PB.13  
      - TIM1_CH2  pin  PA.09  
      - TIM1_CH2N pin  PB.14(Pin 7 in CN6 connector)  
      - TIM1_CH3  pin  PA.10  
      - TIM1_CH3N pin  PB.15 (Pin 5 in CN6 connector)

    - Connect the TIM1 break pin TIM1_BKIN pin (PB.12) to the GND. To generate a 
      break event, switch this pin level from 0V to 3.3V.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
