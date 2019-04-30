/**
  @page STemWin_Fonts  STemWin fonts Readme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    STemWin/STemWin_Fonts/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STemWin fonts application. 
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

This directory contains a set of source files that implement a simple "fonts" 
application based on STemWin for STM32F4xx devices.

The application shows the capability of STemWin to display different languages 
with different fonts styles and sizes.

The used fonts are not part of the fonts shipped with STemWin. 
The fonts are generated with Font converter tool.

To display Unicode characters, we need to save the text with the needed language in UTF-8 format, 
convert UTF-8 text into C code and generate the adueqate font that to be used in the main application.

Detailed steps:

Saving the text in UTF-8 format
-------------------------------
- Save the text with the needed language in "UTF-8" format. (you can use Notepad.exe)
- Using U2C software tool, converts "UTF-8" text into C code 
  (U2C software available under "Middlewares/ST/STemWin/Software") 
- copy the generated C code in the main application

pattern generation
------------------
To optimize as possible the generated fonts sizes we can generate only the used characters, 
for this, we have to save the text file (containing the text to be displayed) in "unicode" format 

Font generation
---------------
 - Opens font converter tool (available under "Middlewares/ST/STemWin/Software")
 - choose the adequate font (depending on the choosen language)
 - disable all characters ranges (Edit/Disable all characters)
 - load the generated pattern (text.txt saved as "unicode" format)
 - save the font
 ==> only used characters in text.txt file are generated.
 - include the font

 @note for arabic fonts the following ranges needs to be activated:
  - 600 -> 6FF
  - FE80 -> FEFC
  
 How to interact with the application
 ------------------------------------
 - Press on top right button to select the language and highlight the displayed text.
 - Press on bottom button to shows more information about the application.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - STemWin/STemWin_Fonts/Core/Inc/main.h                             Main program header file
  - STemWin/STemWin_Fonts/Core/Inc/stm32f4xx_hal_conf.h               Library Configuration file
  - STemWin/STemWin_Fonts/Core/Inc/stm32f4xx_it.h                     Interrupt handlers header file
  - STemWin/STemWin_Fonts/Core/Src/main.c                             Main program file
  - STemWin/STemWin_Fonts/Core/Src/stm32f4xx_it.c                     STM32F4xx Interrupt handlers
  - STemWin/STemWin_Fonts/Core/system_stm32f4xx.c                     STM32F4xx system file
  - STemWin/STemWin_Fonts/STemWin/App/font_app.c                      fonts application
  - STemWin/STemWin_Fonts/STemWin/Target/GUIConf.c                    Display controller initialization
  - STemWin/STemWin_Fonts/STemWin/Target/LCDConf_.c                   Configuration file for the GUI library  
  - STemWin/STemWin_Fonts/STemWin/Target/GUIConf.h                    Header for GUIConf.c
  - STemWin/STemWin_Fonts/STemWin/Target/LCDConf.h                    Header for LCDConf.c
  - STemWin/STemWin_Fonts/STemWin/App/generated/arabic.c   	      arabic flag picture     
  - STemWin/STemWin_Fonts/STemWin/App/generated/chinese.c  	      chinese flag picture
  - STemWin/STemWin_Fonts/STemWin/App/generated/english.c  	      english flag picture
  - STemWin/STemWin_Fonts/STemWin/App/generated/info.c                info logo picture
  - STemWin/STemWin_Fonts/STemWin/App/generated/italian.c  	      italian flag picture
  - STemWin/STemWin_Fonts/STemWin/App/generated/STLogo.c   	      STLogo logo picture
  - STemWin/STemWin_Fonts/STemWin/App/generated/world.c    	      world flag picture
  - STemWin/STemWin_Fonts/STemWin/App/generated/french.c   	      french flag picture
  - STemWin/STemWin_Fonts/STemWin/App/generated/fonts/arabic25.c      arabic font with 25 pixels as width
  - STemWin/STemWin_Fonts/STemWin/App/generated/fonts/arabic40.c      arabic font with 40 pixels as width
  - STemWin/STemWin_Fonts/STemWin/App/generated/fonts/arabic60.c      arabic font with 60 pixels as width
  - STemWin/STemWin_Fonts/STemWin/App/generated/fonts/chinese25B.c    chinese font with 25 pixels as width
  - STemWin/STemWin_Fonts/STemWin/App/generated/fonts/chinese40.c     chinese font with 40 pixels as width
  - STemWin/STemWin_Fonts/STemWin/App/generated/fonts/chineseI60.c    chinese font with 60 pixels as width
  - STemWin/STemWin_Fonts/STemWin/App/generated/fonts/eng25.c         new generated font with 25 pixels as width
  - STemWin/STemWin_Fonts/STemWin/App/generated/fonts/eng40.c         new generated font with 40 pixels as width
  - STemWin/STemWin_Fonts/STemWin/App/generated/fonts/eng60.c         new generated font with 60 pixels as width
  
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
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
