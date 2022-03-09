/**
  @page TIM_PrescalerSelection TIM Prescaler Selection example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_PrescalerSelection/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWM signals generation using TIM3
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

This example shows how to configure the TIM peripheral in PWM (Pulse Width Modulation) 
mode with clock prescaler selection feature activated using __HAL_RCC_TIMCLKPRESCALER()
which allows to double the output frequency.

SystemCoreClock is set to 180 MHz for STM32F4xx Devices.

  In this example TIM3 input clock (TIM3CLK) is set to 4 * APB1 clock (PCLK1), since 
  Timer clock prescalers selection activated (TIMPRE bit from RCC_DCKCFGR register is set).   
  TIM3CLK = 4 * PCLK1  
  PCLK1 = HCLK / 4 
  => TIM3CLK = HCLK = SystemCoreClock

  For TIM3CLK equal to SystemCoreClock and prescaler equal to (5 - 1), TIM3 counter clock 
  is computed as follows:
  TIM3 counter clock = TIM3CLK / (Prescaler + 1)
                     = SystemCoreClock / (Prescaler + 1)
                     = 36MHz

  For ARR equal to (1800 - 1), the TIM3 output clock is computed as follows:
  TIM3 output clock = TIM3 counter clock / (ARR + 1)
                    = 20KHZ
                     
  The TIM3 CCR1 register value is equal to 900, so the TIM3 Channel 1 generates a 
  PWM signal with a frequency equal to 20 KHz and a duty cycle equal to 50%:

  TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR + 1)* 100 = 50%


The PWM waveforms can be displayed using an oscilloscope.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

Timers, PWM, Prescaler, Duty Cycle, Waveform, Oscilloscope, Output, Signal

@par Directory contents 

  - TIM/TIM_PrescalerSelection/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_PrescalerSelection/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - TIM/TIM_PrescalerSelection/Inc/main.h                  Header for main.c module  
  - TIM/TIM_PrescalerSelection/Src/stm32f4xx_it.c          Interrupt handlers
  - TIM/TIM_PrescalerSelection/Src/main.c                  Main program
  - TIM/TIM_PrescalerSelection/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_PrescalerSelection/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F469xx/STM32F479xx devices.

  - This example has been tested and validated with STMicroelectronics STM32469I-EVAL RevC 
    board and can be easily tailored to any other supported device 
    and development board.      

  - STM32469I-EVAL Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
        - TIM3_CH1 : PB.04, pin 24 in CN6 connector



@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
