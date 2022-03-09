/**
  @page LTDC_Display_2Layers LTDC Display 2 layers example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LTDC/LTDC_Display_2Layers/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LTDC Display 2 layers example.
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

  This example describes how to configure the LTDC peripheral to display two Layers  
  at the same time.
  The goal of this example is to explain how to blend two layers and how to display 
  an image with L8 (8bits per pixels indexed) as pixel format. 

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  Then the SystemClock_Config() function is used to configure the system
  clock (SYSCLK) to run at 180 MHz.
 
  After LCD initialization, the LCD layer 1 and Layer 2 are configured as following :
  - Layer 1 is configured to display an image loaded from flash memory with direct
    color (RGB565) as pixel format and 320x240 size.
  - Layer 2 is configured to display an image loaded from flash memory with indirect
    color (L8) as pixel format and 320x240 size.
    To display an image with an indirect color as pixel format a color lookup table (CLUT) 
    is loaded, then every byte from image data (L8_320x240) is considered as 
    a position in color lookup table (CLUT).(see example below)  

 @note : 
 The C files of the images used in this example are generated with 
 STemWin bitmap converter released with this package.
 \Middlewares\ST\STemWin\Software\BmpCvtST.exe
 Use the bitmap files under resources repository

  The blending is always active and the two layers can be blended following 
  the configured blending factors and the constant alpha.
  In this example the constant alpha for layer 2 is decreased to see the layer 1
  in the intersection zone.

  Example :
  ------------------------------------------------------------------------------
  |Image data   |  position   CLUT(RGB Value)  |   Alpha  | Output (ARGB value)|
  |-------------|------------------------------|----------|------------------- |
  |00 10 FF 25  |  0   -----> 0xFD10EA         | 0xFF     | 0xFFFD10EA         |
  |             |  .                           |          | 0xFF1548AD         |
  |             |  .                           |          | 0xFFAE6547         |
  |             |  .                           |          | 0xFFDA14EA         |
  |             |  .                           |          | .                  |
  |             |  16  -----> 0x1548AD         |          | .                  |
  |             |  .                           |          | .                  |
  |             |  .                           |          | .                  |
  |             |  37  -----> 0xDA14EA         |          | .                  |
  |             |  .                           |          |                    |
  |             |  .                           |          |                    |
  |             |  255 -----> 0xAE6547         |          |                    |
  ------------------------------------------------------------------------------       

  The images change unlike their positions on LCD in an infinite loop.

STM32 Eval boards LED can be used to monitor the transfer status:
 - LED3 is ON when there is an error in Init process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, LTDC, Display Controller, TFT, LCD, Graphic, RGB888, 2 Layers, BMP

@par Directory contents

    - LTDC/LTDC_Display_2Layers/Inc/main.h                Main configuration file
    - LTDC/LTDC_Display_2Layers/Inc/stm32f4xx_it.h        Interrupt handlers header file
    - LTDC/LTDC_Display_2Layers/Inc/stm32f4xx_hal_conf.h  HAL configuration file
    - LTDC/LTDC_Display_2Layers/Inc/RGB565_320x240.h      Image used for LTDC Validation
    - LTDC/LTDC_Display_2Layers/Inc/L8_320x240.h          Image used for LTDC Validation 
    - LTDC/LTDC_Display_2Layers/Src/main.c                Main program  
    - LTDC/LTDC_Display_2Layers/Src/stm32f4xx_it.c        Interrupt handlers
    - LTDC/LTDC_Display_2Layers/Src/stm32f4xx_hal_msp.c   HAL MSP module    
    - LTDC/LTDC_Display_2Layers/Src/system_stm32f4xx.c    STM32F4xx system clock configuration file 


@par Hardware and Software environment  

  - This example runs on STM32F429xx/STM32F439xx devices.
  
  - This example has been tested with STMicroelectronics STM324x9I-EVAL RevB 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
                           

                                    */
