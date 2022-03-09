/**
  @page STemWin_Acceleration  STemWin acceleration Readme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    STemWin/STemWin_Acceleration/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STemWin acceleration application. 
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

This directory contains a set of source files that implement a simple "acceleration" 
application based on STemWin for STM32F4xx devices.

The application shows the capability of STemWin to do animations using the DMA2D HW for the 
acceleration of different draws (Bitmaps, Memdevs, ColorFill...).

 How to interact with the application
 ------------------------------------
 - The application is an automatic run of a FadeIn/FadeOut between different plans.
 - After the run of these plans, the performance of the operations done is displayed on the screen
 - It will loop infinetly.  

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, STemWin, Acceleration, DMA2D, GUI, Demonstration

@par Directory contents 

   - STemWin/STemWin_Acceleration/Core/Inc/main.h			Main program header file
   - STemWin/STemWin_Acceleration/Core/Inc/stm32f4xx_hal_conf.h		Library Configuration file
   - STemWin/STemWin_Acceleration/Core/Inc/stm32f4xx_it.h		Interrupt handlers header file
   - STemWin/STemWin_Acceleration/Core/Src/main.c			Main program file
   - STemWin/STemWin_Acceleration/Core/Src/stm32f4xx_it.c		STM32F4xx Interrupt handlers
   - STemWin/STemWin_Acceleration/Core/Src/system_stm32f4xx.c		STM32F4xx system file
   - STemWin/STemWin_Acceleration/STemWin/App/acceleration_app.c	acceleration application
   - STemWin/STemWin_Acceleration/STemWin/App/generated/moon.c		moon picture
   - STemWin/STemWin_Acceleration/STemWin/App/generated/sun.c		sun picture
   - STemWin/STemWin_Acceleration/STemWin/App/generated/sun_rain.c	sun_rain picture
   - STemWin/STemWin_Acceleration/STemWin/App/generated/village.c	village picture
   - STemWin/STemWin_Acceleration/STemWin/Target/GUIConf.c		Display controller initialization
   - STemWin/STemWin_Acceleration/STemWin/Target/GUIConf.h		Header for GUIConf.c
   - STemWin/STemWin_Acceleration/STemWin/Target/LCDConf.c		Configuration file for the GUI library
   - STemWin/STemWin_Acceleration/STemWin/Target/LCDConf.h		Header for LCDConf.c
  
@par Hardware and Software environment  

  - This application runs on STM32F469xx devices.

  - This application has been tested with STMicroelectronics STM32469I-Discovery
    discovery boards and can be easily tailored to any other supported device 
    and development board.

  - This application is configured to run by default on STM32469I-DISCO RevC board.
  - In order to run this application on RevA or RevB boards, replace the flag 
    USE_STM32469I_DISCO_REVC, which is defined in the pre-processor options, by 
    either USE_STM32469I_DISCO_REVA or USE_STM32469I_DISCO_REVB respectively.

@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 

 */
