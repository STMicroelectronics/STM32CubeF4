/**
  @page LCD_DSI_CmdMode_TearingEffect : LCD DSI examples in DSI command mode 

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LCD_DSI/LCD_DSI_CmdMode_TearingEffect/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD DSI in command mode example.
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
in Command mode using Tearing Effect, through DSI LINK, and partial 
refresh method. One buffer is used for display and for draw.

Layer0 is initialized to display a brief description of the example. It will be
used for images display also.

In this example, the display area is split in tow part right and left. When the
end of refresh event occurred, left part of display area is refreshed/displayed while
the other right part is prepared to be displayed when Line Event occurs.

Partial Refresh with splitting method is based on the following steps:
- Set area of refresh by fixing Page and Column values.
- Select active area (LEFT_AREA)
- Set Scan Line at line 533, this to allow to write data to LEFT_AREA before a new
  refresh comes.
- Copy image to LCD frame buffer address
- Refresh LEFT_AREA
- refresh RIGHT_AREA

Each image is displayed for two secondes.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Graphic, Display, LCD, DSI, MIPI Alliance, Command mode, Tearing Effect, Partial refresh, LTDC, QVGA, ARGB8888

@par Directory contents

  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Inc/stm32f4xx_hal_conf.h          HAL configuration file
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Inc/stm32f4xx_it.h                Interrupt handlers header file
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Inc/main.h                        Header for main.c module
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Inc/life_augmented_argb8888.h     Image 320x240 in ARGB8888 to display on LCD
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Inc/image_320x240_argb8888.h      Image 320x240 in ARGB8888 to display on LCD  
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Src/stm32f4xx_it.c                Interrupt handlers
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Src/main.c                        Main program
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Src/stm32f4xx_hal_msp.c           HAL MSP file
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Src/system_stm32f4xx.c            STM32F4xx system source file

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
