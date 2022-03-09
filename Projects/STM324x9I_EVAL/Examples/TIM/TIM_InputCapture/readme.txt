/**
  @page TIM_InputCapture Input Capture example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_InputCapture/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_InputCapture example.
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

  This example shows how to use the TIM peripheral to measure the frequency of
  an external signal.
  
  The TIM1CLK frequency is set to SystemCoreClock /2 (Hz), the Prescaler is 0,
  so the TIM1 counter clock is SystemCoreClock (Hz)/2.
  SystemCoreClock is set to 180 MHz for STM32F4xx Devices.

  TIM1 is configured in Input Capture Mode: the external signal is connected to 
  TIM1 Channel2 used as input pin.
  To measure the frequency we use the TIM1 CC2 interrupt request, so in the 
  TIM1_CC_IRQHandler routine, the frequency of the external signal is computed.
  
  The "uwFrequency" variable contains the external signal frequency:
  uwFrequency = TIM1 counter clock / uwDiffCapture (Hz),
  where "uwDiffCapture" is the difference between two consecutive TIM1 captures.


 The minimum frequency value to measure is 2x (TIM1 counter clock / CCR MAX)
                                              = 2x(180MHz/2)/ 65535
                                              = 2747 Hz  

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timers, Frequency, Input, Capture, External Signal, Measurement

@par Directory contents 

  - TIM/TIM_InputCapture/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_InputCapture/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_InputCapture/Inc/main.h                  Main program header file  
  - TIM/TIM_InputCapture/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_InputCapture/Src/main.c                  Main program
  - TIM/TIM_InputCapture/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - TIM/TIM_InputCapture/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file

@par Hardware and Software environment 

  - This example runs on STM32F429xx/STM32F439xx devices. 
    
  - This example has been tested with STMicroelectronics STM324x9I-EVAL RevB(STM32F42xxx/43xxx devices) 
    evaluation boards and can be easily tailored to any other supported device and development board

  - STM324x9I-EVAL RevB Set-up 
    - Connect the external signal to measure to the TIM1 CH2 pin (PE.11).


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
   

 */
