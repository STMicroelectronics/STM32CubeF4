/**
  @page STemWin   STM32446E-EVAL Demonstration Firmware
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Demonstrations/STemWin/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STM32446E-EVAL Demonstration
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

@par Demonstration Overview

When the STM32446E-EVAL demonstration is launched for the first time, the touch screen 
need to be calibrated.
You have to follow the displayed calibration instructions by touching the screen 
at the indicated positions.
Once the touch screen calibrated, the supported demos will be viewed per icon. 

 @note All icons are stored into external QSPI Flash memory. 
  
Below you find an overview of the different offered modules in the demonstration:

 + System
 --------
 The system module provides three control tabs: system information, general settings
 and clock settings 
 
 + File browser
 --------------
 The File browser module allows to explore the connected storage unit(s), to delete 
 or to open a selected file. 
  
 + Game
 -------
 The game coming in the STM32Cube demonstration is based on the Reversi game. 

 + Benchmark
 -----------
 The Benchmark module allows measure the graphical performance by measuring the 
 time needed to draw several coloured rectangles in random position with random 
 size during a specific period.
 
 + Audio
 -------
 The audio player module supports playing music in WAV format.
 You can use the *.wav audio provided under "Utilities/Media/Audio" or any other ones.
 
 @note: Supported audio format: the audio player supports all wav PCM audio files with the following configurations:
          + Sample rate: 8 to 96 kHz
          + Channel number: stereo/mono
          + Audio data format: 16 bits
 @note: you can copy these file at the root any directory of the uSD or USB key
 
 + USB Mass storage Device
 -------------------------
 The USB device module includes mass storage device application using the MicroSD
 memory. The USB FS is used for the USB disk Flash storage unit.
    
 + Camera
 --------
 The camera application allows to directly and permanently display on the LCD the image
 captured using the camera module. 
 
 + Image viewer
 --------------
 The Image viewer module allows displaying bmp and jpg pictures. 
 User can use .jpg and .bmp files available within the "BMP_240x320", "BMP_320x240", "BMP_480x272", folder under "Utilities/Media/Pictures".
 
 @note: you can copy these file at the root any directory of the uSD or USB key
  
 For more details about the demonstration modules please refers to  STM32CubeF4 demonstration (UM1743)

 @note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
       based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
       a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
       than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
       To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
 @note The application needs to ensure that the SysTick time base is always set to 1 millisecond
       to have correct HAL operation.

@par Keywords

Demonstration, STemWin, FreeRTOS, RTOS, Graphics, Game, System, CPU, OSPI 

@par Hardware and Software environment

  - This demonstration runs on STM32F446xx devices.    
  - This demonstration has been tested with STM32446E-EVAL RevB evaluation board.
  - Jumpers configuration:
      - JP4: <2-3> (used for USB device module)
      - JP19: <1-2> (used for audio player module)


@par How to use it ? 

Below the detailed steps:

In order to make the program work, you must do the following :
 1 - Open your preferred toolchain 
 2 - Rebuild all files
 3 - Open STM32 ST-Link Utility V4.1.0 or later , click on "External Loader" from the bar menu then check "N25Q256A_STM32446E-EVAL" box 
 4 - Connect the STM32446E-EVAL board to PC with USB cable through CN16
 5 - Set BOOT0 pin (SW1) to 1 to select RAM Boot
 6 - Use "STM32CubeDemo_STM32446E-EVAL_X.Y.Z.hex" file with STM32 ST-Link Utility to program both internal Flash and external QSPI memory
 7 - Change BOOT0 pin (SW1) to 0, and reset the Board 
 8 - copy the pictures and audio files provided under "Utilities/Media/" in the USB key
 9 - Plug a USB micro A-Male to A-Female cable on CN9 connector
10 - Connect a uSD Card to the MSD connector (CN4).
11 - Run the demonstration
-> The internal Flash and the external QSPI memory are now programmed and the demonstration is shown on the board.

In order to Edit and debug the program, you must do the following
- if not done, perform step 1, 2, 3, 4 , 5 and 6 described above,
- Open your preferred toolchain,
- Use the IDE to update and load the internal flash content, 
- Run the demonstration.




 */
