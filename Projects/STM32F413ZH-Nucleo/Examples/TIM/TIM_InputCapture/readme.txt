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

  The TIM3CLK frequency is set to SystemCoreClock (Hz), the Prescaler is 0,
  so the TIM3 counter clock is SystemCoreClock (Hz).
  SystemCoreClock is set to 100 MHz for STM32F413xx/STM32F423xx Devices.

  TIM3 is configured in Input Capture Mode: the external signal is connected to 
  TIM3 Channel2 used as input pin.
  To measure the frequency we use the TIM3 CC2 interrupt request, so in the 
  TIM3_IRQHandler routine, the frequency of the external signal is computed.

  The "uwFrequency" variable contains the external signal frequency:
  uwFrequency = TIM3 counter clock / uwDiffCapture (Hz),
  where "uwDiffCapture" is the difference between two consecutive TIM3 captures.


  The minimum frequency value to measure is TIM3 counter clock / CCR MAX
                                              = 100 MHz / 65535

  Due to TIM3_IRQHandler processing time (around 2.5us), the maximum
  frequency value to measure is around 400kHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timers, DMA, Frequency, Input, Capture, External Signal, Measurement

@par Directory contents 

  - TIM/TIM_InputCapture/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_InputCapture/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_InputCapture/Inc/main.h                  Header for main.c module  
  - TIM/TIM_InputCapture/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_InputCapture/Src/main.c                  Main program
  - TIM/TIM_InputCapture/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_InputCapture/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F413xx/STM32F423xx devices.
  - In this example, the clock is set to 100 MHz.
    
  - This example has been tested with STMicroelectronics STM32F413ZH-Nucleo
    board and can be easily tailored to any other supported device 
    and development board.      

  - STM32F413ZH-Nucleo Set-up
    - Connect the external signal to measure to the TIM3 CH2 pin (PB.05). 
      PB05 is connected to pin13 in CN7 connector.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
