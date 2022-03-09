/**
  @page TIM_OCActive TIM_OCActive example
  
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

  This example shows how to configure the TIM peripheral in Output Compare Active 
  mode (when the counter matches the capture/compare register, the concerned output
  pin is set to its active state).

  The TIM4 frequency is set to SystemCoreClock, and the objective is
  to get TIM4 counter clock at 10 kHz so the Prescaler is computed as following:
     - Prescaler = (TIM4CLK /TIM4 counter clock) - 1
   
  SystemCoreClock is set to 100 MHz for STM32F4xx Devices.

  The TIM4 CCR1 register value is equal to 10000:
  TIM4_CH1 delay = CCR1_Val/TIM4 counter clock  = 1s
  so the TIM4 Channel 1 generates a signal with a delay equal to 1s.

  The TIM4 CCR2 register value is equal to 5000:
  TIM4_CH2 delay = CCR2_Val/TIM4 counter clock = 500 ms
  so the TIM4 Channel 2 generates a signal with a delay equal to 500 ms.

  The TIM4 CCR3 register value is equal to 2500:
  TIM4_CH3 delay = CCR3_Val/TIM4 counter clock = 250 ms
  so the TIM4 Channel 3 generates a signal with a delay equal to 250 ms.

  The TIM4 CCR4 register value is equal to 1250:
  TIM4_CH4 delay = CCR4_Val/TIM4 counter clock = 125 ms
  so the TIM4 Channel 4 generates a signal with a delay equal to 125 ms.

  The delay correspond to the time difference between PE.00 rising edge and
  TIM4_CHx signal rising edges.

@note Delay values mentioned above are theoretical (obtained when the system clock frequency 
      is exactly 100 MHz). Since the generated system clock frequency may vary from one board to another observed
      delay might be slightly different.
	  
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timers, Output, Compare, Active, Signals

@par Directory contents 

  - TIM/TIM_OCActive/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_OCActive/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_OCActive/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OCActive/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_OCActive/Src/main.c                  Main program
  - TIM/TIM_OCActive/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_OCActive/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F412xG devices.
    
  - This example has been tested with STMicroelectronics STM32412G-DISCOVERY 
    board and can be easily tailored to any other supported device 
    and development board.      

  - STM32412G-DISCOVERY Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - Use LED1 connected to PE.00 (Reference)
      - PB.06: (TIM4_CH1) (connected to PB.06 (pin 23 in P2 connector))
      - PB.07: (TIM4_CH2) (connected to PB.07 (pin 24 in P2 connector))
      - PB.08: (TIM4_CH3) (connected to PB.08 (pin 19 in P2 connector))
      - PB.09: (TIM4_CH4) (connected to PB.09 (pin 20 in P2 connector))

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



 */
