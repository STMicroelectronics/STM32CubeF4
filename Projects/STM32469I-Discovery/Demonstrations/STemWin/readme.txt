/**
  @page STemWin   STM32469I-DISCO Demonstration Firmware
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Demonstrations/STemWin/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STM32469I-DISCO Demonstration
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

The demonstration firmware is built around the graphical library STemWin and the FreeRTOS 
real time operating system and uses almost the whole STM32 capability to offer a large scope
of usage based on the STM3Cube HAL, BSP and several Middleware components.

@par Demo Description

The STM32 F4 demonstration is running on STM32469I-DISCOVERY boards RevB.
Tow flavours of the Demonstration binaries are available:
   - 'STM32469I-Disco_Demo_V1.2.0.hex': based on StemWin and which source code
     is provided within the STM32Cube_FW_F4 package. Its modules are listed below.
   - 'STM32469I-Disco_Demo_V1.2.0_FULL.hex' : an out of the box Demo integrating the StemWin
     demo in addition to third parties graphical Demo modules:
   - TouchGFX demonstration module based on Draupner Graphics’ commercial graphic library.
     Free evaluation version is available at www.touchgfx.com/stmicroelectronics.
  

Below you find an overview of the different offered module in the demonstration:

 + Video player
 --------------
 The video player module provides a video solution based on the STM32F4xx and STemWin
 movie API. It supports playing movie in emf format.
 
 + Audio player
 --------------
 The audio player module provides a complete audio solution based on the STM32F4xx and
 delivers a high-quality music experience. It supports playing music in WAV format but may
 be extended to support other compressed formats such as MP3 and WMA audio formats.
 The Module supports background mode feature.
 
 + Audio Recorder
 ----------------
 The Audio record module allows to record an audio file. The audio format supported is 
 WAV format but may be extended to support other compressed formats such as MP3.
 The recorded files are stored in USB Disk flash(USB High Speed).
  
 + Home alarm
 ------------ 
 Control of Home alarm system, equipped with cameras.
 Static picture shown when a room is selected and then the camera icon pressed
 General room alarm activation/deactivation when pressed.
 
 + Gardening control
 -------------------
 The gardening control module provides a graphic garden watering system behaviour
 
 + Game
 ------
 The game coming in the STM32Cube demonstration is based on the Reversi game. It is a
 strategy board game for two players, played on an 8×8 board. The goal of the game is to
 have the majority of disks turned to display your color when the last playable empty square
 is filled.
 
 + System Info
 --------------  
 The system info module provides information about the core, supported eval boards, 
 CPU speed and demonstration version.
 
 For more details about the demonstration modules please refers to STM32CubeF4 demonstration (UM1743)
 
@note Demonstration Firmware doesn't embed TouchGFX demonstration module. 
      Free evaluation version of the TouchGFX demonstration, based on Draupner Graphics’ 
	  commercial graphic library, is available at www.touchgfx.com/stmicroelectronics

@par Keywords

Demonstration, STemWin, FreeRTOS, RTOS, Graphics, Game, System, CPU, OSPI 
      
@par Hardware and Software environment

  - This application runs on STM32F469xx devices.  

  - This application has been tested with STMicroelectronics STM32469I_DISCOVERY
    boards and can be easily tailored to any other supported device 
    and development board.
  - This application is configured to run by default on STM32469I-DISCO RevC board.
  - In order to run this application on RevA or RevB boards, replace the flag 
    USE_STM32469I_DISCO_REVC, which is defined in the pre-processor options, by 
    either USE_STM32469I_DISCO_REVA or USE_STM32469I_DISCO_REVB respectively.
  - Jumpers configuration:
    - JP5	Fitted (Power on MCU)

@par How to use it ? 

The QSPI external flash loader is not integrated with supported toolchains, it’s only supported with
STM32 ST-Link Utility V3.7 or later.
To load the demonstration, use STM32 ST-Link Utility to program both internal Flash and external QSPI memory.
To edit and debug the demonstration you need first to program the external QSPI memory using STLink utility
and then use your preferred toolchain to update and debug the internal flash content.

Below the detailed steps:

In order to program the demonstration you must do the following
1- Open STM32CubeProgrammer, click on "External Loader" icon then check 
   "N25Q128A_STM32469I-DISCO" box for STM32469I_DISCOVERY board RevB or
   "MT25QL128A_STM32469I-DK" box for STM32469I_DISCOVERY board RevC
2- Connect the STM32F469I-DISCO board to PC with USB cable through CN1
3- Use "STM32469I-Disco_Demo_VX.Y.Z.hex" file provided under “Binary” with STM32 ST-Link Utility
   to program both internal Flash and external NOR memory
   The "STM32469I-Disco_Demo_V1.2.0_FULL.hex" file provided under 'Binary', can also be used to take benefit
   from Touch-GFX third party demonstrations modules replacing Gardening control module.

4- copy the audio and video files provided under "Utilities/Media/" in the USB key
5- Plug a USB micro A-Male to A-Female cable on CN13 connector
-> The internal Flash and the external QSPI are now programmed and the demonstration is shown on the board.

In order to Edit and debug the program, you must do the following
- if not done, perform step 1, 2, 3, 4 and 5 described above.
- Open your preferred toolchain
- Use the IDE to update and load the internal flash content 
- Run the demonstration 
 
     

 */
