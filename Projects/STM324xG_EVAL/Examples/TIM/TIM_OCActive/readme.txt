/**
  @page TIM_OCActive Output Compare Active example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_OCActive/readme.txt 
  * @author  MCD Application Team
  * @brief   This example shows how to configure the Timer to generate four 
  *          delayed signals.
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

  This example shows how to configure the TIM peripheral to generate four different 
  signals with four different delays.

  The TIM3CLK frequency is set to SystemCoreClock / 2 (Hz), and the objective is
  to get TIM3 counter clock at 2 KHz so the Prescaler is computed as following:
     - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
   
  SystemCoreClock is set to 168 MHz.

  The TIM3 CCR1 register value is equal to 1000:
  TIM3_CH1 delay = CCR1_Val/TIM3 counter clock  = 500 ms
  so the TIM3 Channel 1 generates a signal with a delay equal to 500 ms.

  The TIM3 CCR2 register value is equal to 500:
  TIM3_CH2 delay = CCR2_Val/TIM3 counter clock = 250 ms
  so the TIM3 Channel 2 generates a signal with a delay equal to 250 ms.

  The TIM3 CCR3 register value is equal to 250:
  TIM3_CH3 delay = CCR3_Val/TIM3 counter clock = 125 ms
  so the TIM3 Channel 3 generates a signal with a delay equal to 125 ms.

  The TIM3 CCR4 register value is equal to 125:
  TIM3_CH4 delay = CCR4_Val/TIM3 counter clock = 62.5 ms
  so the TIM3 Channel 4 generates a signal with a delay equal to 62.5 ms.

  The delay correspond to the time difference between PG.06 falling edge and
  TIM3_CHx signal rising edges.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note LED4 is turned ON, because it's sharing the same GPIO pin with the TIM3 channel 2.

@par Keywords

Timers, Output, Compare, Active, Signals, 

@par Directory contents 

  - TIM/TIM_OCActive/Inc/stm32f4xx_hal_conf.h         HAL configuration file
  - TIM/TIM_OCActive/Inc/stm32f4xx_it.h               Interrupt handlers header file
  - TIM/TIM_OCActive/Inc/main.h                       Main program header file  
  - TIM/TIM_OCActive/Src/stm32f4xx_it.c               Interrupt handlers
  - TIM/TIM_OCActive/Src/main.c                       Main program
  - TIM/TIM_OCActive/Src/stm32f4xx_hal_msp.c          HAL MSP module
  - TIM/TIM_OCActive/Src/system_stm32f4xx.c           STM32F4xx system clock configuration file

                                  
@par Hardware and Software environment 

  - This example runs on STM32F407xx/417xx devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL RevC  
    evaluation boards and can be easily tailored to any other supported device and development board

  - STM324xG-EVAL RevC Set-up 
    - Connect the following pins to an oscilloscope to monitor the different 
      waveforms:
       - Use LED1 connected to PG.06 (Reference)
       - PC.06 (TIM3_CH1)
       - PC.07 (TIM3_CH2)
       - PC.08 (TIM3_CH3)
       - PC.09 (TIM3_CH4)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
