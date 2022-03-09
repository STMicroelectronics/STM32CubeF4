/**
  @page STemWin_HelloWorld Readme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    STemWin_HelloWorld/readme.txt 
  * @author  MCD Application Team
  * @brief   
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

Simple "Hello World" example based on STemWin for STM32F4xx devices.

The example allows also to run the different Segger samples that can be
downloaded from here:
http://www.segger.com/emwin-samples.html
To do this, user has only to replace the file "Basic_HelloWorld.c" into the
project workspace by the downloaded one.

Note that the following user files may need to be updated:
  LCDConf_stm32446e_eval.c
  GUIConf_stm32446e_eval.c
(if for example more GUI allocated memory is needed)

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
	  
@note If the application is not running normally as mentioned above , you can accordingly modify either
      the Heap and Stack of the application or the GUI_NUMBYTES define in the GUIConf.c file

@par Keywords

Display, STemWin, LCD, HelloWorld, GUI

@par Directory contents 

  - STemWin/STemWin_HelloWorld/STemWin/Target/GUIConf.h                 Header for GUIConf_stm32446e_eval.c
  - STemWin/STemWin_HelloWorld/STemWin/Target/LCDConf_stm32446e_eval.h  Header for LCDConf_stm32446e_eval.c
  - STemWin/STemWin_HelloWorld/Core/Inc/main.h                          Main program header file
  - STemWin/STemWin_HelloWorld/Core/Inc/stm32f4xx_hal_conf.h            Library Configuration file
  - STemWin/STemWin_HelloWorld/Core/Inc/stm32f4xx_it.h                  Interrupt handlers header file
  - STemWin/STemWin_HelloWorld/STemWin/App/BASIC_HelloWorld.c           Simple demo drawing "Hello world"
  - STemWin/STemWin_HelloWorld/STemWin/Target/GUIConf_stm32446e_eval.c  Display controller initialization
  - STemWin/STemWin_HelloWorld/STemWin/Target/LCDConf_stm32446e_eval.c  Configuration file for the GUI library
  - STemWin/STemWin_HelloWorld/Core/Src/main.c                          Main program file
  - STemWin/STemWin_HelloWorld/Core/Src/stm32f4xx_it.c                  STM32F4xx Interrupt handlers
  - STemWin/STemWin_HelloWorld/Core/Src/system_stm32f4xx.c              STM32F4xx system file
 
@par Hardware and Software environment 

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with STM32446E_EVAL evaluation board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
 

 */
