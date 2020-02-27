/**
  @page STemWin_Simulation Readme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    WIN32/STemWin_Simulation/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STemWin Simulation project. 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2017 STMicroelectronics International N.V. 
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

@par Description

This STemWin simulation project allows you to compile the same C source on your Windows
PC using a native (typically Microsoft) compiler and create an executable for your own
application. Doing so allows the following:
 - Design of the user interface on your PC (no hardware required!).
 - Debugging of the user interface program.
 - Creation of demos of your application in windows executable file .


The STemWin simulation requires Microsoft Visual C++ (version 6.00 or higher) and the
integrated development environment (IDE) which comes with it. You will see a simulation
of your LCD on your PC screen, which has the same resolution in X and Y and can display
the exact same colors as your LCD once it has been properly configured.
The entire graphic library API and Window Manager API of the simulation are identical
to those on your target system; all functions will behave in the very same way as
on the target hardware since the simulation uses the same C source code as the target
system. The difference lies only in the lower level of the software: the display
driver. Instead of using the actual display driver, the PC simulation uses a simulation
driver which writes into a bitmap. The bitmap is then displayed on your screen using
a second thread of the simulation. This second thread is invisible to the application;
it behaves just as if the LCD routines were writing directly to the display.

The STemWinxxx_WIN32.lib file contains a full library which allows you to evaluate all
available features of STemWin. You will not be able to view the source code of STemWin or 
the simulation, but you will still be able to become familiar with what STemWin can do.

The project allows also to run the different Segger samples that can be downloaded
from here: http://www.segger.com/emwin-samples.html
To do this, user has only to replace the file "MainTask.c" into the project workspace
by the downloaded one.


@par Software environment 
  - This Simulation project runs on VS Express for Desktop on windows platform
  - This project has been tested with Microsoft Visual Studio Express 2017 for Windows Desktop on Windows 7

@par Keywords

STemWin, Simulation, Microsoft Visual C++, LCD

@par How to use it ? 

In order to use the simulation project :
  - Open the Simulation.vcxproj file 
  - Rebuild all files
  - Start Debugging (F5)
  - A "hello world" message will be shown in the Simulation display
  - the MainTask.c could be overwritten by user code to run his own code
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
