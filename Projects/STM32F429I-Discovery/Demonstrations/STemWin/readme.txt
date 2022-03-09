/**
  @page Demo   Demo STM32F429I-Discovery
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics  *******************
  * @file    Demonstrations/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STM32F429I-Discovery Demonstration
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
of usage based on the STM32Cube HAL, BSP and several Middleware components.
  
Below you find an overview of the different offred module in the demonstration:

 + System
 --------
 The system module provides three control tabs: 
  - system information 
  - general settings
  - date settings
  - clock settings 

 To set the global demonstration settings. The system module retrieves
 demonstration information from internal kernel settings data structures and acts 
 on the several kernel services to changes settings.
 
 + File browser
 --------------
 The File browser module is a system module that allows to explore the connected
 storage unit(s), to delete or to open a selected file. The file list structure 
 is built during the media connection and updated after a connection status change of one
 of the used media.
 
 + Game
 -------
 The game coming in the STM32Cube demonstration is based on the Reversi game. It is a
 strategy board game for two players, played on an 8×8 board. The goal of the game is to
 have the majority of disks turned to display your color when the last playable empty square
 is filled.

 + Benchmark
 -----------
 The Benchmark module is a system module that allows measure the graphical performance
 by measuring the time needed to draw several colored rectangles in random position with
 random size during a specific period. The result is given in pixel per second.
 
 + Video
 -------
 The video player module provides a video solution based on the STM32F4xx and STemWin
 movie API. It supports playing movie in emf format.
 
 + Image viewer
 --------------
 The Image viewer module allows displaying bmp and jpg pictures. It is possible to load the
 full images list from a folder or to add the images manually to the playlist. Once the playlist is
 created, navigation between pictures can be done either via Next and previous buttons or by
 enabling the slide show mode. The slide show timer can be changed on the fly (there is no
 need to restart the module).
 
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

  - This example runs on STM32F429xx devices.
    
  - This example has been tested with STMicroelectronics STM32F429I-Discovery RevC
    boards and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 
     - Copy the picture and video files provided under "Utilities/Media/" in the USB key
       USB key must contain the files listed below:
         - .jpg and .bmp files available within the "BMP_240x320" and "BMP_320x240" folder under "Utilities/Media/Pictures" 
         - .emf files available "Utilities/Media/Video" 
     - Plug the USB key into the STM32F429I-DISCO board through 'USB micro A-Male to A-Female' cable(CN6).

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
