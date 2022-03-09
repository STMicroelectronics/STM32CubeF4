/**
  @page EEPROM_Emulation application to show an eeprom emulation

  @verbatim
  ******************* (C) COPYRIGHT 2016 STMicroelectronics ********************
  * @file    EEPROM/EEPROM_Emulation/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the EEPROM_Emulation application.
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

This application describes the software solution for substituting standalone EEPROM
by emulating the EEPROM mechanism using the on-chip Flash of STM32F4xx devices.

Please refer to AN3969 for further details regarding this application.

The application requires two Flash memory sectors of identical size allocated to non-volatile data:
one that is initially erased, and offers byte-by-byte programmability; the other that is ready
to take over when the former sector needs to be garbage-collected. A header field that occupies
the first half word (16-bit) of each sector indicates the sector status. Each of these sectors is
considered as a page.
The sectors used are Sector 2 and Sector 3 with a size of 16KB each one.

STM32F4xx-Nucleo's LED can be used to monitor the application status:
  - LED2 is On when the application runs successfully.
  - LED2 is toggle in case of error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

EEPROM, Emulation, Flash, Program, Erase, Sectors, OPCODE

@par Directory contents 

 - EEPROM/EEPROM_Emulation/Inc/stm32f4xx_hal_conf.h          HAL Configuration file
 - EEPROM/EEPROM_Emulation/Inc/main.h                        Header for main.c module
 - EEPROM/EEPROM_Emulation/Inc/eeprom.h                      Header for eeprom.c module 
 - EEPROM/EEPROM_Emulation/Inc/stm32f4xx_it.h                Header for stm32f4xx_it.c
 - EEPROM/EEPROM_Emulation/Src/main.c                        Main program
 - EEPROM/EEPROM_Emulation/Src/eeprom.c                      EEPROM program
 - EEPROM/EEPROM_Emulation/Src/stm32f4xx_it.c                Interrupt handlers
 - EEPROM/EEPROM_Emulation/Src/system_stm32f4xx.c            STM32F4xx system clock configuration file 
    

@par Hardware and Software environment

  - This application runs on STM32F446xx devices.
    
  - This application has been tested with STMicroelectronics STM32F4xx-Nucleo rev C
    boards and can be easily tailored to any other supported device and development board.

Table 1. EEPROM application implementation on STM32F4xx-Nucleo
/*** Platform ***|************* Implementation **************************|***** Configuration *****\
****************************************************************************************************
|    Firmware    | The EEPROM program is located at 0x08000000. The Flash|                         |
|                | routines (program/erase) are executed from the Flash  |                         |
|                | memory.                                               |                         |
|                | The size of this program is about 6 Kbytes and        |                         |
|                | programmed on:                                        | Sector 0                |  
|                | ------------------------------------------------------|-------------------------|
|                |                                                       |                         | 
|                | The flash zone used for the EEPROM emulation is       | Sector 2 and Sector 3   |
|                | located at 0x08008000                                 | each one has a size of  |
|                |                                                       |           16KB          | 
\**************************************************************************************************/

@note
Before using the application, ensure that dedicated sectors for EEPROM (FLASH_SECTOR_2 and FLASH_SECTOR_3) 
are erased to avoid a match with a valid page OPCODE.

Following picture illustrates the situation in program memory:

 Top Flash Memory address /-------------------------------------------\  0x08080000
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |          Sector 4 - Sector 7              |
                          |                                           |
                          |-------------------------------------------|  0x08010000
    Sector 3 (16KB)       |                                           |
                          |                                           |
                          | Flash used for EEPROM emulation mechanism |  0x0800C000
    Sector 2 (16KB)       |                                           |
                          |                                           |
                          |-------------------------------------------|  0x08008000
    Sector 1 (16KB)       |                                           |
                          |                                           |
                          |-------------------------------------------|  0x08004000
                          |                                           |
    Sector 0 (32KB)       |   flash used for implement the EEPROM     |
                          |          emulation mechanism(6KB)         |
                          \-------------------------------------------/	 0x08000000

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
