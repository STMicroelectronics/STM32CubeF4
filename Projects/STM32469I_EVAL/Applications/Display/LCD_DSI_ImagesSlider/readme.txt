/**
  @page LCD_DSI_ImagesSlider : LCD DSI images slider application  

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Display\LCD_DSI_ImagesSlider/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD DSI images slider application.
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

@par Application Description

This application aims to show the outstanding capability of Display Serial Interface (DSI)
 periphiral to display images with high resolution (800x480). With a simple movement of finger,
the content of GRAM is directly updated and displayed on DSI LCD. 
Thanks to Tearing Effect and Partial refresh features that allow user to provoke
a refresh whenever he wants and on the specified region he desires, the refresh 
of LCD is more rapid than any time before.

At the beginning, the quadspi is initialized to put images in. Then TS and LCD DSI 
are initialized then a first image is displayed.
In this example, the refresh of LCD is made in partial mode (left then right). 

Two mode of images draw are possible through this application; vertical draw or
horizontal draw. The default mode is horizontal. User can change the mode by 
pressing Tamper button.

By moving the finger on TS, a new image will progressively displayed on LCD while
the old one disappears progressively. When HORIZONTAL_DRAW mode is active, moving 
finger left or right causes the display of next or previous image. 
When VERTICAL_DRAW mode is active, moving finger up or down causes the display of 
next or previous image.

The program runs in infinite loop.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Graphic, Display, DSI, LCD, Image slider, GRAM, Touch screen, 

@par Directory contents

  - Display/LCD_DSI_ImagesSlider/Inc/stm32f4xx_hal_conf.h          HAL configuration file
  - Display/LCD_DSI_ImagesSlider/Inc/stm32f4xx_it.h                Interrupt handlers header file
  - Display/LCD_DSI_ImagesSlider/Inc/main.h                        Header for main.c module
  - Display/LCD_DSI_ImagesSlider/Inc/image*.h                      Images to be displayed
  - Display/LCD_DSI_ImagesSlider/Src/stm32f4xx_it.c                Interrupt handlers
  - Display/LCD_DSI_ImagesSlider/Src/main.c                        Main program
  - Display/LCD_DSI_ImagesSlider/Src/stm32f4xx_hal_msp.c           HAL MSP file
  - Display/LCD_DSI_ImagesSlider/Src/system_stm32f4xx.c            STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F469xx/STM32F479xx devices.

  - This example has been tested with STMicroelectronics STM32469I-EVAL
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Open STM32 ST-Link Utility V4.1.0, click on "External Loader" from the bar menu then check "MT25Q512A_STM32469I-EVAL" box 
 - Connect the STM32469I-EVAL board to PC with USB cable through CN22
 - Use generated hex file with STM32 ST-Link Utility to program both internal Flash and external QSPI memory
 - Run the application 



 */
