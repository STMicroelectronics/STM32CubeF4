/**
  @page FatFs_MultiDrives   FatFs with multi drives application
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FatFs/FatFs_MultiDrives/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FatFs with multi drives application
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

How to use STM32Cube firmware with FatFs middleware component as a generic FAT
file system module. This example develops an application exploiting FatFs
features with multidrives (RAMDisk, uSD) configuration.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 180 MHz. The SD module uses internally a 48-MHz clock 
which is coming from PLL SAI. This frequency permit to reach 24 MHz clock 
needed for SD operation and in line  with microSD specification. 
           
The application is based on writing and reading back a text file from two drives,
and it's performed using FatFs APIs to access the FAT volume as described
in the following steps:

 - Link the SDRAM and uSD disk I/O drivers;
 - Register the file system object (mount) to the FatFs module for both drives;
 - Create a FAT file system (format) on both logical drives;
 - Create and Open new text file objects with write access;
 - Write data to the text files;
 - Close the open text files;
 - Open text file objects with read access;
 - Read back data from the text files;
 - Check on read data from text file;
 - Close the open text files;
 - Unlink the SDRAM and uSD disk I/O drivers.
 
It is worth noting that the application manages any error occurred during the 
access to FAT volume, when using FatFs APIs. Otherwise, user can check if the
written text file is available on the uSD card.

It is possible to fine tune needed FatFs features by modifying defines values 
in FatFs configuration file "ffconf.h" available under the project includes 
directory, in a way to fit the application requirements. In this case, two drives
are used, so the Max drive number is set to: _VOLUMES = 2 in "ffconf.h" file.

STM32 Eval boards LEDs can be used to monitor the application status:
  - LED1 is ON when the application runs successfully.
  - LED3 is ON when any error occurs.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about FatFs implementation on STM32Cube, please refer to UM1721 "Developing Applications 
on STM32Cube with FatFs".

@par Keywords

FatFS, SD Card, SDRAM, File system, FAT Volume, Format, Mount, Read, Write, multidrives

@par Directory contents
 
  - FatFs/FatFs_MultiDrives/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - FatFs/FatFs_MultiDrives/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - FatFs/FatFs_MultiDrives/Inc/main.h                  Main program header file
  - FatFs/FatFs_MultiDrives/Inc/sdram_diskio.h          FatFS sdram diskio driver header file
  - FatFs/FatFs_MultiDrives/Inc/sd_diskio.h             FatFS sd diskio driver header file
  - FatFs/FatFs_MultiDrives/Inc/ffconf.h                FAT file system module configuration file   
  - FatFs/FatFs_MultiDrives/Src/stm32f4xx_it.c          Interrupt handlers
  - FatFs/FatFs_MultiDrives/Src/main.c                  Main program
  - FatFs/FatFs_MultiDrives/Src/sdram_diskio.c          FatFS sdram diskio driver implementation
  - FatFs/FatFs_MultiDrives/Src/sd_diskio.c             FatFS sd diskio driver implementation
  - FatFs/FatFs_MultiDrives/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
         
 
@par Hardware and Software environment

  - This application runs on STM32F446xx devices.
    
  - This application has been tested with STMicroelectronics STM32446E-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32446E-EVAL Set-up
   - Insert a uSD Card to the MSD connector (CN4).

@note  When the uSD Card is used; the Camera module must be unplugged, 
       SD card removed and re-inserted after camera removal, board reset;
       this is due to the shared pins between the two devices.
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
