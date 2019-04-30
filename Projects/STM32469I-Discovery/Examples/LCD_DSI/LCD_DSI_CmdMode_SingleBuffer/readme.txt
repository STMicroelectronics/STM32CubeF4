/**
  @page LCD_DSI_CmdMode_SingleBuffer : LCD DSI examples in DSI mode Video Burst

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LCD_DSI/LCD_DSI_CmdMode_SingleBuffer/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD DSI in mode video burst example.
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

This example provides a description of how to use the embedded LCD DSI controller 
(using IPs LTDC and DSI Host) to drive the KoD LCD mounted on board.

The goal of this example is to display a QVGA landscape (320x240) images on LCD glass
in Command mode using a single buffer for display and for draw.

Layer0 is initialized to display a brief description of the example. It will be
used for images display also.

Line Event callback (LTDC) is used to swap image to be draw to display buffer upon  
drawing complete refresh (V-SYNC). Each image is displayed for two secondes. The same
buffer is used to draw and display image. 

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - LCD_DSI/LCD_DSI_CmdMode_SingleBuffer/Inc/stm32f4xx_hal_conf.h          HAL configuration file
  - LCD_DSI/LCD_DSI_CmdMode_SingleBuffer/Inc/stm32f4xx_it.h                Interrupt handlers header file
  - LCD_DSI/LCD_DSI_CmdMode_SingleBuffer/Inc/main.h                        Header for main.c module
  - LCD_DSI/LCD_DSI_CmdMode_SingleBuffer/Inc/life_augmented_argb8888.h     Image 320x240 in ARGB8888 to display on LCD
  - LCD_DSI/LCD_DSI_CmdMode_SingleBuffer/Inc/image_320x240_argb8888.h      Image 320x240 in ARGB8888 to display on LCD  
  - LCD_DSI/LCD_DSI_CmdMode_SingleBuffer/Src/stm32f4xx_it.c                Interrupt handlers
  - LCD_DSI/LCD_DSI_CmdMode_SingleBuffer/Src/main.c                        Main program
  - LCD_DSI/LCD_DSI_CmdMode_SingleBuffer/Src/stm32f4xx_hal_msp.c           HAL MSP file
  - LCD_DSI/LCD_DSI_CmdMode_SingleBuffer/Src/system_stm32f4xx.c            STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F469xx devices.

  - This example has been tested with STMicroelectronics STM32469I-EVAL
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
