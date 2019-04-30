/**
  @page BSP  Example on how to use the BSP drivers
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics  *******************
  * @file    BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

This example provides a description of how to use the different BSP drivers. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

This example shows how to use the different functionalities of LCD, MEMS, 
touchscreen and RF EEPROM (only with daughter board ANT7-M24LR-A) by switching 
between all tests using USER button. 

Touchscreen example: after the touchscreen calibration, use the touchscreen 
functionality to activate all colored circles inside a rectangle.

LCD example: this example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

LCD log example: this example shows how to use the LCD log features.

MEMS example: this example shows how to use accelerometer MEMS. You need to color all the
circles in moving boards: Right(Y+)/Left(Y-)/Right rotate(Z+)/Left rotate(Z-)/Back(X+)/Front(X-)
 
EEPROM example: this example shows how to read and write data in RF EEPROM.
  *             The I2C EEPROM memory (M24LR64) is available on separate daughter 
  *             board ANT7-M24LR-A, which is not provided with the STM32F429I 
  *             DISCOVERY board.
  *             To use this driver you have to connect the ANT7-M24LR-A to CN3 
  *             connector of STM32F429I DISCOVERY board.
Note: select Workspace STM32F429I-Discovery+EEPROM if you want to test RF EEPROM
ANT7-M24LR-A
      
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - BSP/Src/main.c                     Main program
  - BSP/Src/system_stm32f4xx.c         STM32F4xx system clock configuration file
  - BSP/Src/stm32f4xx_it.c             Interrupt handlers 
  - BSP/Src/lcd.c                      LCD drawing features
  - BSP/Src/touchscreen.c              Touchscreen feature
  - BSP/Src/ts_calibration.c           Touchscreen calibration
  - BSP/Src/mems.c                     MEMS features
  - BSP/Src/eeprom.c                   EEPROM features (only use in workspace STM32F429I-Discovery+EEPROM)
  - BSP/Src/log.c                      LCD Log features
  - BSP/Inc/main.h                     Main program header file  
  - BSP/Inc/stm32f4xx_hal_conf.h       HAL configuration file
  - BSP/Inc/stm32f4xx_it.h             Interrupt handlers header file
  - BSP/Inc/stlogo.h                   Image used for BSP example
  - BSP/Inc/lcd_log_conf.h             LCD Log configuration file
        
        
@par Hardware and Software environment  

  - This example runs on STM32F429xx devices.
    
  - This example has been tested with STMicroelectronics STM32F429I-Discovery RevC
    boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
