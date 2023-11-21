/**
  @page FatFs_RAMDisk   FatFs with RAM disk drive example
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FatFs/FatFs_RAMDisk/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FatFs with RAM disk drive application
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
file system module. This example develops an application that exploits FatFs
features to configure a RAM disk (SDRAM) drive.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 180 MHz.
 
The application is based on writing and reading back a text file from a drive,
and it's performed using FatFs APIs to access the FAT volume as described
in the following steps: 

 - Link the SDRAM disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the SDRAM drive;
 - Create a FAT file system (format) on the SDRAM drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Open text file object with read access;
 - Read back data from the text file;
 - Close the open text file;
 - Check on read data from text file;
 - Unlink the SDRAM disk I/O driver.

Notice that the application manages possible errors that might occur during the 
access to FAT volume, when using FatFs APIs.

It is possible to fine tune needed FatFs features by modifying defines values 
in FatFs configuration file "ffconf.h" available under the project includes 
directory, in a way to fit the application requirements. 
 
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


@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

FatFS, RAM disk, SRAM, File system, FAT Volume, Format, Mount, Read, Write

@par Directory contents
 
  - FatFs/FatFs_RAMDisk/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - FatFs/FatFs_RAMDisk/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - FatFs/FatFs_RAMDisk/Inc/main.h                  Main program header file
  - FatFs/FatFs_RAMDisk/Inc/sdram_diskio.h          FatFS sdram diskio driver header file
  - FatFs/FatFs_RAMDisk/Inc/ffconf.h                FAT file system module configuration file   
  - FatFs/FatFs_RAMDisk/Src/stm32f4xx_it.c          Interrupt handlers
  - FatFs/FatFs_RAMDisk/Src/main.c                  Main program
  - FatFs/FatFs_RAMDisk/Src/sdram_diskio.c          FatFS sdram diskio driver implementation
  - FatFs/FatFs_RAMDisk/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file

 
@par Hardware and Software environment

  - This application runs on STM32F469xx and STM32F479xx devices.
    
  - This application has been tested with STMicroelectronics STM32469I-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application

 */
