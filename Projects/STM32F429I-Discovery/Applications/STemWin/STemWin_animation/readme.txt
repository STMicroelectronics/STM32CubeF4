/**
  @page STemWin_Animation  STemWin animation Readme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    STemWin/STemWin_Animation/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STemWin animation application. 
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

This directory contains a set of source files that implement a simple "animation" 
application based on STemWin for STM32F4xx devices.

The application shows the capability of STemWin to do different animations of different objects independently.

 How to interact with the application
 ------------------------------------
 - The application is an automatic run.
 - It will show 3 objects animated independently: a dog, the cloud and a balloon
 - It will loop infinetly.  

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, STemWin, Animation, LCD, GUI, Demonstration

@par Directory contents 

    - STemWin/STemWin_Animation/Core/Inc/main.h					Main program header file
    - STemWin/STemWin_Animation/Core/Inc/stm32f4xx_hal_conf.h			Library Configuration file
    - STemWin/STemWin_Animation/Core/Inc/stm32f4xx_it.h				Interrupt handlers header file
    - STemWin/STemWin_Animation/Core/Src/main.c					Main program file
    - STemWin/STemWin_Animation/Core/Src/stm32f4xx_it.c				STM32F4xx Interrupt handlers
    - STemWin/STemWin_Animation/Core/Src/system_stm32f4xx.c			STM32F4xx system file
    - STemWin/STemWin_Animation/STemWin/App/animation_app.c			animation application
    - STemWin/STemWin_Animation/STemWin/App/generated/Background.c		Background picture
    - STemWin/STemWin_Animation/STemWin/App/generated/Balloon.c			Balloon picture
    - STemWin/STemWin_Animation/STemWin/App/generated/dog1_walk1.c	        Dog first picture going right
    - STemWin/STemWin_Animation/STemWin/App/generated/dog1_walk1_r.c	        Dog first picture going left
    - STemWin/STemWin_Animation/STemWin/App/generated/dog1_walk2.c	        Dog second picture going right
    - STemWin/STemWin_Animation/STemWin/App/generated/dog1_walk2_r.c	        Dog second picture going left
    - STemWin/STemWin_Animation/STemWin/App/generated/cloud.c			cloud picture
    - STemWin/STemWin_Animation/STemWin/Target/GUIConf.c			Display controller initialization
    - STemWin/STemWin_Animation/STemWin/Target/GUIConf.h			Header for GUIConf.c
    - STemWin/STemWin_Animation/STemWin/Target/LCDConf.c			Configuration file for the GUI library
    - STemWin/STemWin_Animation/STemWin/Target/LCDConf.h			Header for LCDConf.c
  
@par Hardware and Software environment  

  - This application runs on STM32F429xx devices.

  - This application has been tested with STMicroelectronics STM32F429I-Discovery
    discovery boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 

 */
