/**
  @page STemWin_HelloWorld Readme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    STemWin/STemWin_HelloWorld/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STemWin Hello World application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, Core/Including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, Core/IncLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * Core/IncIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (Core/IncLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (Core/IncLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
   @endverbatim

@par Application Description

Simple "Hello World" example based on STemWin for STM32F4xx devices.

The application allows also to run the different Segger samples that can be
downloaded from here:
http://www.segger.com/emwin-samples.html
To do this, user has only to replace the file "Basic_HelloWorld.c" into the
project workspace by the downloaded one.

Note that the following user files may need to be updated:
  LCDConf_stm3240g_eval.c
  GUIConf_stm3240g_eval.c
(if for example more GUI allocated memory is needed)

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable Core/Incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note If the application is not running normally as mentionned above , you can accordingly modify either
      the Heap and Stack of the application or the GUI_NUMBYTES define in the GUIConf.c file

@par Directory contents 

  - STemWin/STemWin_HelloWorld/STemWin/Target/GUIConf.h                Header for GUIConf_stm3240g_eval.c
  - STemWin/STemWin_HelloWorld/STemWin/Target/LCDConf_stm3240g_eval.h  Header for LCDConf_stm3240g_eval.c
  - STemWin/STemWin_HelloWorld/Core/Inc/main.h                         Main program header file
  - STemWin/STemWin_HelloWorld/Core/Inc/stm32f4xx_hal_conf.h           Library Configuration file
  - STemWin/STemWin_HelloWorld/Core/Inc/stm32f4xx_it.h                 Interrupt handlers header file
  - STemWin/STemWin_HelloWorld/STemWin/App/BASIC_HelloWorld.c          Simple demo drawing "Hello world"
  - STemWin/STemWin_HelloWorld/STemWin/Target/GUIConf_stm3240g_eval.c  Display controller initialization
  - STemWin/STemWin_HelloWorld/STemWin/Target/LCDConf_stm3240g_eval.c  Configuration file for the GUI library
  - STemWin/STemWin_HelloWorld/Core/Src/main.c                         Main program file
  - STemWin/STemWin_HelloWorld/Core/Src/stm32f4xx_it.c                 STM32F4xx Interrupt handlers
  - STemWin/STemWin_HelloWorld/Core/Src/system_stm32f4xx.c             STM32F4xx system file
 
@par Hardware and Software environment 

  - This application runs on STM32F407xx/417xx devices.
    
  - This application has been tested with STM324xG-EVAL evaluation board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
