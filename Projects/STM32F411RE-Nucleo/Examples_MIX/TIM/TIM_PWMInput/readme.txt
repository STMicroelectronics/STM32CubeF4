/**
  @page TIM_PWMInput TIM PWM Input example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_PWMInput/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM PWM_Input example.
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

This example shows how to use the TIM peripheral to measure the frequency and 
duty cycle of an external signal.

The TIM3CLK frequency is set to SystemCoreClock/1 (Hz), the Prescaler is 0 so the 
counter clock is SystemCoreClock/1 (Hz).
SystemCoreClock is set to 100 MHz for STM32F411xx Devices.

TIM3 is configured in PWM Input Mode: the external signal is connected to 
TIM3 Channel2 used as input pin.
To measure the frequency and the duty cycle, we use the TIM3 CC2 interrupt request,
so in the TIM3_IRQHandler routine, the frequency and the duty cycle of the external 
signal are computed.

"uwFrequency" variable contains the external signal frequency:
TIM3 counter clock = SystemCoreClock/1,
uwFrequency = TIM3 counter clock / TIM3_CCR2 in Hz, 

"uwDutyCycle" variable contains the external signal duty cycle:
uwDutyCycle = (TIM3_CCR1*100)/(TIM3_CCR2) in %.

The minimum frequency value to measure is (TIM3 counter clock / CCR MAX)
                                         = (100 MHz/1)/ 65535
                                         = 1525 Hz

TIM2 can be used to generate the external signal in case a function generator 
is not available. TIM2 is configured in PWM Output Mode to produce a square wave on PA.00.
Frequency and duty cycles can be changed by pressing the User Button (PC13).
Six combinations are available (see thables aFrequency[] and aDutyCycle[]).

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timers, Input, signals, PWM, External signal, Frequency, Duty cycle, Measure

@par Directory contents 

  - TIM/TIM_PWMInput/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_PWMInput/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_PWMInput/Inc/main.h                  Header for main.c module  
  - TIM/TIM_PWMInput/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_PWMInput/Src/main.c                  Main program
  - TIM/TIM_PWMInput/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_PWMInput/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-F411RE 
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-F411RE Set-up
    - You can either Connect the external signal to measure to the TIM3 CH2 pin (PA.07) (pin 4 in CN5 connector).
    - Or connect TIM3 CH1 pin PA.00 (pin 1 in CN8 connector) to the TIM3 CH2 pin (PA.07) instead of an external signal.

	
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
