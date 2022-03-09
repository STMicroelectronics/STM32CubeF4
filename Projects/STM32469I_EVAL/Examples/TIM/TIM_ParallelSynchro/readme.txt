/**
  @page TIM_ParallelSynchro Timers Synchronization example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_ParallelSynchro/readme.txt 
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


@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

Timers, PWM, Parallel synchronization, Parallel mode, Master, Slave, Duty Cycle, Waveform,
Oscilloscope, Output, Signal

@par Directory contents 

  - TIM/TIM_ParallelSynchro/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_ParallelSynchro/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_ParallelSynchro/Inc/main.h                  Header for main.c module  
  - TIM/TIM_ParallelSynchro/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_ParallelSynchro/Src/main.c                  Main program
  - TIM/TIM_ParallelSynchro/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_ParallelSynchro/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F469xx/STM32F479xx devices.

  - This example has been tested and validated with STMicroelectronics STM32469I-EVAL RevC board and can be
    easily tailored to any other supported device and development board.      

  - STM32469I-EVAL Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM2 CH1 (PA.00, pin 15 in CN5 connector)
      - TIM3 CH1 (PC.06, pin 58 of CN6 connector)
      - TIM4 CH1 (PB.06, pin 18 in CN6 connector)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
