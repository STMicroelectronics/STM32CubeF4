/**
  @page TIM_ExtTriggerSynchro TIM External Trigger Synchro example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_ExtTriggerSynchro/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM External Trigger Synchro example.        
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

  This example shows how to synchronize TIM peripherals in cascade mode with an
  external trigger.
  In this example three timers are used:

  1/TIM1 is configured as Master Timer:
      - Toggle Mode is used
      - The TIM1 Enable event is used as Trigger Output 

  2/TIM1 is configured as Slave Timer for an external Trigger connected to TIM1 
    TI2 pin (TIM1 CH2 configured as input pin):
      - The TIM1 TI2FP2 is used as Trigger Input
      - Rising edge is used to start and stop the TIM1: Gated Mode.

  3/TIM3 is slave for TIM1 and Master for TIM4,
      - Toggle Mode is used
      - The ITR1(TIM1) is used as input trigger 
      - Gated mode is used, so start and stop of slave counter
        are controlled by the Master trigger output signal(TIM1 enable event).
      - The TIM3 enable event is used as Trigger Output. 

  4/TIM4 is slave for TIM3,
      - Toggle Mode is used
      - The ITR2(TIM3) is used as input trigger
      - Gated mode is used, so start and stop of slave counter
        are controlled by the Master trigger output signal(TIM3 enable event).

   The TIM1CLK is fixed to 168 MHZ, the Prescaler is equal to 5 so the TIMx clock 
   counter is equal to 28 MHz.
   The TIM3CLK  and TIM4CLK are fixed to 84 MHZ, the Prescaler is equal to 5 
   so the TIMx clock counter is equal to 14 MHz. 
   The Three Timers are running at: 
   TIMx frequency = TIMx clock counter/ 2*(TIMx_Period + 1) = 93,3 KHz.
   
  The starts and stops of the TIM1 counters are controlled by the external trigger.
  The TIM3 starts and stops are controlled by the TIM1, and the TIM4 starts and 
  stops are controlled by the TIM3.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timers, PWM, External Trigger, Synchronization, Cascade mode, Master, Slave, Duty Cycle, Waveform,
Oscilloscope, Output, Signal,

@par Directory contents 

  - TIM/TIM_ExtTriggerSynchro/Inc/stm32f4xx_hal_conf.h      HAL configuration file
  - TIM/TIM_ExtTriggerSynchro/Inc/stm32f4xx_it.h            Interrupt handlers header file
  - TIM/TIM_ExtTriggerSynchro/Inc/main.h                    Main program header file
  - TIM/TIM_ExtTriggerSynchro/Src/stm32f4xx_it.c            Interrupt handlers
  - TIM/TIM_ExtTriggerSynchro/Src/main.c                    Main program
  - TIM/TIM_ExtTriggerSynchro/Src/stm32f4xx_hal_msp.c       HAL MSP module
  - TIM/TIM_ExtTriggerSynchro/Src/system_stm32f4xx.c        STM32F4xx system clock configuration file  


@par Hardware and Software environment

  - This example runs on STM32F407xx/417xx devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL RevC  
    evaluation boards and can be easily tailored to any other supported device and development board

  - STM324xG-EVAL RevC Set-up 
    - Connect an external trigger to the TIM1 CH2 (PE.11).

    - Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM1 CH1 (PA.08)
      - TIM3 CH1 (PC.06)
      - TIM4 CH1 (PB.06) 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 


 */
