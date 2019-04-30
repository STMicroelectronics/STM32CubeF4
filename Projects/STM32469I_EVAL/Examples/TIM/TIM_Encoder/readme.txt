/**
  @page TIM_Encoder TIM Encoder example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_Encoder/readme.txt 
  * @author  MCD Application Team
  * @brief   This example shows how to configure the Timer in Encoder interface 
  *          to determinate the rotation direction. 
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    |             |CH2:PC7             CH2:PE11 |             |
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
LED1 is configured to toggle accordingly every 1s. 
               
SystemCoreClock is set to 180 MHz for STM32F4xx Devices.

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

@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the board's power off/power on or by software calling the BSP_LCD_Reset() function.

@par Directory contents 

  - TIM/TIM_Encoder/Inc/stm32f4xx_hal_conf.h HAL configuration file
  - TIM/TIM_Encoder/Inc/stm32f4xx_it.h       Interrupt handlers header file
  - TIM/TIM_Encoder/Inc/main.h               Header for main.c module  
  - TIM/TIM_Encoder/Src/stm32f4xx_it.c       Interrupt handlers
  - TIM/TIM_Encoder/Src/main.c               Main program
  - TIM/TIM_Encoder/Src/stm32f4xx_hal_msp.c  HAL MSP file
  - TIM/TIM_Encoder/Src/system_stm32f4xx.c   STM32F4xx system source file

    
@par Hardware and Software environment

  - This example runs on STM32F469xx/STM32F479xx devices.
  - In this example, the clock is set to 180 MHz.
  - This example has been tested and validated with STMicroelectronics STM32469I-EVAL RevC 
    evaluation board and can be easily tailored to any other supported device and development board

  - STM32469I-EVAL Set-up 
    - Connect PC6, pin 58 of CN6 connector (TIM3_Channel 1) to PA8, pin 52 of CN6 connector (TIM1_Channel 1).
    - Connect PC7, pin 56 of CN6 connector (TIM3_Channel 2) to PE11, pin 16 of CN10 connector (TIM1_Channel 2).
    - Put uwDirection in the debugger live watch to detect the rotation direction
      change.
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
