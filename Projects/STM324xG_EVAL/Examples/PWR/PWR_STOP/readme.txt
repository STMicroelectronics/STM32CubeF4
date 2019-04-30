/**
  @page PWR_STOP PWR Stop Mode example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    PWR/PWR_STOP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR Stop Mode example.
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

How to enter the Stop mode and wake up from this mode by using the RTC wakeup 
timer event or an interrupt.

This example shows how to enters the system to STOP mode and wake-up from this
mode using RTC Wake-up Timer Event connected to EXTI Line 22 or Key push button
EXTI Line 15.

In the associated software
  - the system clock is set to 168 MHz
  - the EXTI_Line15 is configured to generate interrupt on falling edge
  - the EXTI_Line22 connected internally to the RTC Wake-up event is configured
    to generate an interrupt on rising edge each 4s
  - the SysTick is programmed to generate an interrupt each 1 ms 
In the SysTick interrupt handler, LED2 is toggled, this is used to indicate whether
the MCU is in STOP or RUN mode.

The system enters STOP mode and will wait for the RTC Wake-up event to be 
generated each 4s, or Key push button is pressed. 
 - If the RTC Wake-up event (EXTI_Line22) is the source of wake-up from STOP, LED1 is toggled.
 - If the Key button (EXTI_Line15) is the source of wake-up from STOP, LED4 is toggled.

This behavior is repeated in an infinite loop.

LEDs are used to monitor the system state as following:
 - LED1 toggled: system woken up from STOP using RTC Wake-up Interrupt
 - LED2 toggling: system in RUN mode
 - LED3 Initialization error (RTC, RCC,...)
 - LED4 toggled: system woken up from STOP using EXTI_Line15 (KEY push button)
 
 @note To measure the current consumption in STOP mode, please refer to 
      @subpage PWR_CurrentConsumption example.

 @note This example can not be used in DEBUG mode, this is due to the fact 
       that the Cortex-M4 core is no longer clocked during low power mode 
       so debugging features are disabled
       
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select the RTC clock source; in this 
       case the Backup domain will be reset in order to modify the RTC Clock source, as consequence RTC  
       registers (including the backup registers) and RCC_BDCR register are set to their reset values.


@par Directory contents 

  - PWR/PWR_STOP/Inc/stm32f4xx_hal_conf.h     HAL configuration file
  - PWR/PWR_STOP/Inc/stm32f4xx_it.h           Interrupt handlers header file
  - PWR/PWR_STOP/Inc/main.h                   Main program header file
  - PWR/PWR_STOP/Src/system_stm32f4xx.c       STM32F4xx system clock configuration file
  - PWR/PWR_STOP/Src/stm32f4xx_it.c           Interrupt handlers
  - PWR/PWR_STOP/Src/main.c                   Main program
  - PWR/PWR_STOP/Src/stm32f4xx_hal_msp.c      HAL MSP module


@par Hardware and Software environment

  - This example runs on STM32F407xx/417xx devices.  
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM324xG-EVAL RevC Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PG.06, PG.08, PI.09  and PC.07 pins
    - Use the Key push-button connected to pin PG15 (EXTI_Line15)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
