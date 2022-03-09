/**
  @page UTILS_ReadDeviceInfo UTILS example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/UTILS/UTILS_ReadDeviceInfo/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the UTILS example.
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

@par Example Description

This example describes how to read UID, Device ID and Revision ID and save 
them into a global information buffer.

Then UID, Device ID and Revision ID will be saved in following global variables:
- aShowDeviceID: Device identifier
- aShowRevisionID: Revision identifier (This field indicates the revision of the device. 
  For example, it is read as 0x1000 for Revision 1.0)
- aShowCoordinate: X and Y coordinates on the wafer expressed in BCD format
- aShowWaferNumber: WAF_NUM[7:0], Wafer number (8-bit unsigned number)
- aShowLotNumber: LOT_NUM[55:0], Lot number (ASCII encoded)

@par Keywords

System, Utils, Device info, UID, Device ID, Revision ID, Wafer, Lot number, 

@par Directory contents 

  - UTILS/UTILS_ReadDeviceInfo/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - UTILS/UTILS_ReadDeviceInfo/Inc/main.h                  Header for main.c module  
  - UTILS/UTILS_ReadDeviceInfo/Src/stm32f4xx_it.c          Interrupt handlers
  - UTILS/UTILS_ReadDeviceInfo/Src/main.c                  Main program
  - UTILS/UTILS_ReadDeviceInfo/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
