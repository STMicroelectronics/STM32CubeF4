/**
  @page FatFs_uSD_RTOS   FatFs with uSD card drive in RTOS mode application
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FatFs with uSD card drive in RTOS mode application
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
features with a microSD drive in RTOS mode configuration.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 100 MHz.
 
The application is based on writing and reading back a text file from a drive,
it creates a normal priority thread and it's performed using FatFs APIs to 
access the FAT volume as described in the following steps: 

 - Link the uSD disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the uSD drive;
 - Create a FAT file system (format) on the uSD drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Open text file object with read access;
 - Read back data from the text file;
 - Check on read data from text file;
 - Close the open text file;
 - Unlink the uSD disk I/O driver.

Notice that the application manages possible errors that might occur during the 
access to FAT volume, when using FatFs APIs. In addition, user can check if the
written text file is available on the uSD card.

It is possible to fine tune needed FatFs features by modifying defines values 
in FatFs configuration file "ffconf.h" available under the project includes 
directory, in a way to fit the application requirements. 

STM32F413H-DISCOVERY's LEDs can be used to monitor the application status:
  - LED3 is ON when the example runs successfully.
  - LED4 toggles in case of error in initialization or during FatFs_uSD process.


@note This application illustrates the usage of two independent time bases when the HAL drivers 
      are used with a RTOS:
      - Systick is used for FreeRTOS (1 millisecond time base)
      - A timer peripheral instance is dedicated to the HAL (1 millisecond time base)

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the 
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".

@par Keywords

FatFS, SD Card, File system, FAT Volume, Format, Mount, Read, Write,FreeRTOS, RTOS

@par Directory contents
 
  - FatFs/FatFs_uSD_RTOS/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - FatFs/FatFs_uSD_RTOS/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - FatFs/FatFs_uSD_RTOS/Inc/main.h                  Header for main.c module
  - FatFs/FatFs_uSD_RTOS/Inc/sd_diskio_dma_rtos.h    FatFS sd diskio driver header file
  - FatFs/FatFs_uSD_RTOS/Inc/ffconf.h                FAT file system module configuration file   
  - FatFs/FatFs_uSD_RTOS/Src/stm32f4xx_it.c          Interrupt handlers
  - FatFs/FatFs_uSD_RTOS/Src/main.c                  Main program
  - FatFs/FatFs_uSD_RTOS/Src/sd_diskio_dma_rtos.c    FatFS sd diskio driver implementation
  - FatFs/FatFs_uSD_RTOS/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment

  - This application runs on STM32F413xx devices.
    
  - This application has been tested with STM32F413H-DISCOVERY board and can be
    easily tailored to any other supported device and development board.        
    

@par How to use it ? 

In order to make the program work, you must do the following:

 - Insert a uSD Card into the uSD connector (CN1).
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
