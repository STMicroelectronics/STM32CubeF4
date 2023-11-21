/**
  @page TIM_Encoder TIM Encoder example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_Encoder/readme.txt 
  * @author  MCD Application Team
  * @brief   This example shows how to configure the Timer in Encoder interface 
  *          to determinate the rotation direction. 
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

This example shows how to configure the TIM1 peripheral in encoder mode to
determinate the rotation direction.
               
                                     
     -------------                               -------------
    |             |CH1:PC6              CH1:PA8 |             |
    |             |---------------------------->|             |
    |    TIM3     |                             |    TIM1     |
    |             |CH1:PC7             CH1:PE11 |             |
    | Quadrature  |---------------------------->| Quadrature  |
    | encoder     |                             | encoder     |
    | emulator    |                             | interface   |
    |             |                             |             |
    |             |                             |             |
    |             |                             |-------------|     -----------
    |             |                             |  Direction  |--> |uwDirection|     
     -------------                               -------------      -----------
               Encoder interface example description
               
               

  To emulate a quadrature encoder, TIM3 is configured in toggle mode to generate
2 quadrature signals on (PC6 and PC7) at 10KHz. Each 1s, signals change phase
(+90°/-90°) to emulate a Forward/Backward rotation.  

  TIM1 is configured in encoder mode interface, counting on TI1 and TI2.
  The counting direction corresponds to the rotation direction of the connected 
sensor (emulated by TIM3 signals).
Rotation direction can be monitored by putting "uwDirection" variable in the 
Live Watch window.
  When uwDirection = 0, and according to the "Counting direction versus encoder
signals" table, rotation direction is Forward.
  When uwDirection = 1, and according to the "Counting direction versus encoder
signals" table, rotation direction is Backward.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timer, Encoder mode, Master, Slave, Duty Cycle, rotation direction, Waveform, Oscilloscope, Output, Signal

@par Directory contents 

  - TIM/TIM_Encoder/Inc/stm32f4xx_conf.h     Library Configuration file
  - TIM/TIM_Encoder/Inc/stm32f4xx_it.h       Interrupt handlers header file
  - TIM/TIM_Encoder/Inc/main.h               Main program header file  
  - TIM/TIM_Encoder/Src/stm32f4xx_it.c       Interrupt handlers
  - TIM/TIM_Encoder/Src/main.c               Main program
  - TIM/TIM_Encoder/Src/stm32f4xx_hal_msp.c  HAL MSP file
  - TIM/TIM_Encoder/Src/system_stm32f4xx.c   STM32F4xx system clock configuration file

    
@par Hardware and Software environment

  - This example runs on STM32F429xx/STM32F439xx devices.
    
  - This example has been tested with STMicroelectronics STM324x9I-EVAL RevB 
    evaluation boards and can be easily tailored to any other supported device and development board

  - STM324x9I-EVAL RevB Set-up 
    - Connect PC6 (TIM3_Channel 1) to PA8 (TIM1_Channel 1).
    - Connect PC7 (TIM3_Channel 2) to PE11(TIM1_Channel 2).
    - Put uwDirection in the debugger live watch to detect the rotation direction
      change.
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
