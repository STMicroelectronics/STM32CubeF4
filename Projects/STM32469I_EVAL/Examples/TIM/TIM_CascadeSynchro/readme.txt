/**
  @page TIM_CascadeSynchro Timers Synchronization example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_CascadeSynchro/readme.txt 
  * @author  MCD Application Team
  * @brief   How to command 2 Timers as slaves (TIM3 & TIM4) using a Timer
  *          as master (TIM2)

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

This example shows how to synchronize TIM2 and Timers (TIM3 and TIM4) in cascade mode.

Timers synchronisation in cascade mode:
   ___________                     ___________              ___________                 
  |   MASTER  |TRGO_Update   ITR2 |  SLAVE 1  |       ITR1 |  SLAVE 2  |                 
  |    TIM3   |-------------------|    TIM2   |------------|    TIM4   |               
  |___________|                   |___________|            |___________| 
                                 

1/ TIM3 is configured as Master Timer:
   - PWM Mode is used
   - The TIM3 Update event is used as Trigger Output 
 
2)TIM2 is slave for TIM3 and Master for TIM4,
  - PWM Mode is used
  - The ITR2(TIM3) is used as input trigger 
  - Gated mode is used, so start and stop of slave counter
    are controlled by the Master trigger output signal(TIM3 update event).
  - The TIM2 Update event is used as Trigger Output.

3)TIM4 is slave for TIM2,
  - PWM Mode is used
  - The ITR1(TIM2) is used as input trigger
  - Gated mode is used, so start and stop of slave counter  are controlled by the
    Master trigger output signal(TIM2 update event).

The TIM2 counter clock is 180 MHz.

  The Master Timer TIM3 is running at:
  TIM3 frequency = TIM3 counter clock / (TIM3_Period + 1) = 351.562 KHz and
  a the duty cycle equal to: TIM3_CCR1/(TIM3_ARR + 1) = 25%

  The TIM2 is running at: 
  (TIM3 frequency)/ (TIM2 period +1) = 87.891 KHz and
  a duty cycle equal to TIM2_CCR1/(TIM2_ARR + 1) = 25%

  The TIM4 is running at:
  (TIM2 frequency)/ (TIM4 period +1) = 21.973 KHz and
  a duty cycle equal to TIM4_CCR1/(TIM4_ARR + 1) = 25%


The PWM waveform can be displayed using an oscilloscope.


@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

Timers, PWM, Cascade Synchronization, Master, Slave, Duty Cycle, Waveform, Oscilloscope, Output, Signal

@par Directory contents 

  - TIM/TIM_CascadeSynchro/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_CascadeSynchro/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_CascadeSynchro/Inc/main.h                  Header for main.c module  
  - TIM/TIM_CascadeSynchro/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_CascadeSynchro/Src/main.c                  Main program
  - TIM/TIM_CascadeSynchro/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_CascadeSynchro/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F469xx/STM32F479xx devices.

  - This example has been tested and validated with STMicroelectronics STM32469I-EVAL RevC board and can be
    easily tailored to any other supported device and development board.      

  - STM32469I-EVAL Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM2 CH4 (PA.03, pin 18 of CN5 connector)
      - TIM3 CH1 (PC.06, pin 58 of CN6 connector)
      - TIM4 CH3 (PD.14, pin 39 of CN10 connector)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
