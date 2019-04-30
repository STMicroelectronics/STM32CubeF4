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
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
   @endverbatim

@par Application Description

This directory contains a set of source files that implement a simple "animation" 
application based on STemWin for STM32F4xx devices.

The application shows the capability of STemWin to do different animations of different objects independently.

Note that the following user files may need to be updated:
  LCDConf_stm324x9i_eval_MB1046.c
  LCDConf_stm324x9i_eval_MB1063.c
  GUIConf.c
(if for example more GUI allocated memory is needed)

 How to interact with the application
 ------------------------------------
 - The application is an automatic run.
 - It will show 3 objects animated independently: the dog, the cloud and a balloon
 - It will loop infinetly.  

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

    - STemWin/STemWin_Animation/Core/Inc/main.h						   Main program header file
    - STemWin/STemWin_Animation/Core/Inc/stm32f4xx_hal_conf.h			           Library Configuration file
    - STemWin/STemWin_Animation/Core/Inc/stm32f4xx_it.h					   Interrupt handlers header file
    - STemWin/STemWin_Animation/Core/Src/main.c						   Main program file
    - STemWin/STemWin_Animation/Core/Src/stm32f4xx_it.c					   STM32F4xx Interrupt handlers
    - STemWin/STemWin_Animation/Core/Src/system_stm32f4xx.c				   STM32F4xx system file
    - STemWin/STemWin_Animation/STemWin/App/animation_app.c				   Animation application
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1046/Background.c   Background picture (MB1046 LCD)
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1046/Balloon.c	   Balloon picture (MB1046 LCD)
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1046/dog1_walk1.c   Dog first picture going right (MB1046 LCD)
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1046/dog1_walk1_r.c Dog first picture going left (MB1046 LCD)
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1046/dog1_walk2.c   Dog second picture going right (MB1046 LCD)
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1046/dog1_walk2_r.c Dog second picture going left (MB1046 LCD)
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1046/cloud.c	   Cloud picture (MB1046 LCD)
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1063/Background.c   Background picture (MB1063 LCD)
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1063/Balloon.c	   Balloon picture (MB1063 LCD)
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1063/dog1_walk1.c   Dog first picture going right (MB1063 LCD)
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1063/dog1_walk1_r.c Dog first picture going left (MB1063 LCD)
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1063/dog1_walk2.c   Dog second picture going right (MB1063 LCD)
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1063/dog1_walk2_r.c Dog second picture going left (MB1063 LCD)
    - STemWin/STemWin_Animation/STemWin/App/generated_stm324x9i_eval_MB1063/cloud.c	   Cloud picture (MB1063 LCD)
    - STemWin/STemWin_Animation/STemWin/Target/GUIConf.c				   Display controller initialization
    - STemWin/STemWin_Animation/STemWin/Target/GUIConf.h				   Header for GUIConf.c
    - STemWin/STemWin_Animation/STemWin/Target/LCDConf_stm324x9i_eval_MB1046.c             Configuration file for the GUI library (MB1046 LCD)
    - STemWin/STemWin_Animation/STemWin/Target/LCDConf_stm324x9i_eval_MB1063.c             Configuration file for the GUI library (MB1063 LCD)
    - STemWin/STemWin_Animation/STemWin/Target/LCDConf.h				   Header for LCDConf.c
    
  
@par Hardware and Software environment  

  - This application runs on STM32F4x9xx devices.

  - This application has been tested with STMicroelectronics STM324x9I-EVAL
    evaluation boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
