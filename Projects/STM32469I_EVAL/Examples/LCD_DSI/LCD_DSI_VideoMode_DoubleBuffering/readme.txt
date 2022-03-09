/**
  @page LCD_DSI_VideoMode_DoubleBuffering : LCD DSI examples in DSI mode Video Burst

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD DSI in mode video burst example.
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

This example provides a description of how to use the embedded LCD DSI controller 
(using IPs LTDC and DSI Host) to drive the KoD LCD mounted on board.

The goal of this example is to display a QVGA landscape (320x240) images on LCD glass
in continuous mode (video burst) using a two buffer one for display and one for draw.

Layer0 is initialized to display a brief description of the example. It will be
used for images display also.

Line Event callback (LTDC) is used to swap front and back buffer upon drawing 
complete refresh (V-SYNC). Each image is displayed for two secondes. 
'back_buffer' pointer is used to select image to draw in back buffer,
while 'front_buffer' pointer is used to select image to display in front buffer.


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Graphic, Display, LCD, DSI, MIPI Alliance, Video mode, Double buffering, LTDC, QVGA, ARGB8888, SDRAM

@par Directory contents

  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Inc/stm32f4xx_hal_conf.h          HAL configuration file
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Inc/stm32f4xx_it.h                Interrupt handlers header file
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Inc/main.h                        Header for main.c module
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Inc/life_augmented_argb8888.h     Image 320x240 in ARGB8888 to display on LCD
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Inc/image_320x240_argb8888.h      Image 320x240 in ARGB8888 to display on LCD   
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Src/stm32f4xx_it.c                Interrupt handlers
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Src/main.c                        Main program
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Src/stm32f4xx_hal_msp.c           HAL MSP file
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Src/system_stm32f4xx.c            STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F469xx/STM32F479xx devices.

  - This example has been tested with STMicroelectronics STM32469I-EVAL
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example



 */
