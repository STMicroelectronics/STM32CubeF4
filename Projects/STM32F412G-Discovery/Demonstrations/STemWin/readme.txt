/**
  @page STemWin   STM32412G-DISCO Demonstration Firmware
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Demonstrations/STemWin/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STM32412G-DISCO Demonstration
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

@par Demo Description

The STM32Cube Demonstration platform comes on top of the STM32CubeTM as a firmware
package that offers a full set of software components based on a modules architecture
allowing re-using them separately in standalone applications. All these modules are
managed by the STM32Cube Demonstration kernel allowing to dynamically adding new
modules and access to common resources (storage, graphical components and widgets,
memory management, Real-Time operating system)

The STM32Cube Demonstration platform is built around the powerful graphical library
STemWin and the FreeRTOS real time operating system and uses almost the whole STM32
capability to offer a large scope of usage based on the STM32Cube HAL BSP and several
middleware components.

@par Demo Description

The STM32 F4 demonstration is running on STM32412G-DISCO boards RevC. 

  
Below you find an overview of the different offered module in the demonstration:

 + Video player
 --------------
 The video player module provides a video solution based on the STM32F4xx and STemWin
 movie API. It supports playing movie in AVI format.
 
 + Audio player
 --------------
 The audio player module provides a complete audio solution based on the STM32F4xx and
 delivers a high-quality music experience. It supports playing music in WAV format but may
 be extended to support other compressed formats such as MP3 and WMA audio formats.
 The Module supports background mode feature.
 
  + Audio recorder
 -----------------
 The audio recorder module allows recording audio and playing it back.
 
 + Analog Clock
 ------------
 The analog clock module shows the the time and gives the possibility to set the time.
  
 + USB Devices
 ------------ 
 The USB device module includes mass storage device application using the MicroSD
 memory.
 
 + System Info
 --------------  
 The system info module provides information about the core, supported boards, 
 CPU speed and demonstration version.  
 
@par Keywords

Demonstration, STemWin, FreeRTOS, RTOS, Graphics, Game, System, CPU, OSPI 
     
@par Hardware and Software environment

  - This application runs on STM32F412xx devices.  

  - This example has been tested with STMicroelectronics STM32412G_DISCO
    boards RevC and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files
 - Open STM32 ST-Link Utility V4.1.0, click on "External Loader" from the bar menu then check "N25Q128A_STM32412G-DISCO" box 
 - Connect the STM32412G-DISCO board to PC with USB cable through CN6
 - Use "STM32CubeDemo_STM32412G-DISCO_VX.Y.Z.hex" file with STM32 ST-Link Utility to program both internal Flash and external QSPI memory
 - Run the demonstration
 - copy the audio and video files provided under "Utilities/Media/" in the SD Card
 - Plug SD Card on CN13 connector 
     

 */
