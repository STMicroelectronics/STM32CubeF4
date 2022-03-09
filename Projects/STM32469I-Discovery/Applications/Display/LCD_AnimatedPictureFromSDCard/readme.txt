/**
  @page LCD_AnimatedPictureFromSDCard LCD animated picture from SD card application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Display/LCD_AnimatedPictureFromSDCard/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LCD animated picture from SD card application.
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

  This application describes how to display an animated picture on LCD saved under
  microSD.
  
  The animated picture is the display of an images sequence with a determined 
  frequency to be seen like a animated image.
 
  The user has to copy the two directories "BACK" and "TOP" available from 
  "./Media/" to the root of micro SD card. 
 
  * Background picture display
  ----------------------------           
  Once the LCD, SD card and file system initialized and configured, a check
  of the existence and the content of the "/BACK" directory is done.

  * Foreground animated pictures display
  -------------------------------------  
  A content check of the "/TOP" directory is done and the number of ".BMP" files 
  is retained.

@note : The maximum number of BMP file is fixed at 25. It can be raised 
        until reaching the maximum of SD card memory space.

@note : the system clock (SYSCLK) is configured to run at 180 MHz and 50 MHz is provided 
        at the output PLL divided by PLL_Q. This frequency permit to reach 25 MHz clock 
        needed for SD operation and in line with microSD specification. 

  The following steps are performed to scroll all the images stored in the 
  SD Card :

  - The foreground layer is set, the image copied from SD card (from "/TOP" directory)
    to intermediate SDRAM memory space and then copied to LCD frame buffer. 
 
  - The color keying feature is applied to remove the bottom of foreground layer (transparent)
    and then replaced by the background layer.
 
  - Jump to next image  

@note : 
  => If the "/Foreground" directory is empty, a warning message is displayed on the LCD : 
     " Open directory.. fails "
    
  => If the file type stored in the "/BACK" or "/TOP" directories is not supported,
     a warning message is displayed on the LCD : " file type not supported. "
    
  => If the SD card is not inserted, a warning message is displayed on the LCD :
     " Open directory.. fails "
      
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

  -  LED3 is ON when the uSD disk I/O driver is not Linked;

@par Keywords

Graphic, Display, LCD, Pictures, SD Card, BMP, Background, Foreground, Layers,

@par Directory contents

    - Display/LCD_AnimatedPictureFromSDCard/Inc/main.h                 Main configuration file
    - Display/LCD_AnimatedPictureFromSDCard/Inc/sd_diskio.h            FatFS sd diskio driver header file
    - Display/LCD_AnimatedPictureFromSDCard/Inc/stm32f4xx_it.h         Interrupt handlers header file
    - Display/LCD_AnimatedPictureFromSDCard/Inc/stm32f4xx_hal_conf.h   HAL Configuration file 
    - Display/LCD_AnimatedPictureFromSDCard/Inc/ffconf.h               FAT file system module configuration file
    - Display/LCD_AnimatedPictureFromSDCard/Inc/fatfs_storage.h        Header for fatfs_storage.c
    - Display/LCD_AnimatedPictureFromSDCard/Src/main.c                 Main program 
    - Display/LCD_AnimatedPictureFromSDCard/Src/sd_diskio.c            FatFS sd diskio driver implementation
    - Display/LCD_AnimatedPictureFromSDCard/Src/fatfs_storage.c        Storage (FatFs) driver
    - Display/LCD_AnimatedPictureFromSDCard/Src/stm32f4xx_it.c         Interrupt handlers
    - Display/LCD_AnimatedPictureFromSDCard/Src/system_stm32f4xx.c     STM32F4xx system clock configuration file


@par Hardware and Software environment  

  - This application runs on STM32F469xx devices.
    
  - This application has been tested with STMicroelectronics STM32469I-DISCOVERY
    discovery boards and can be easily tailored to any other supported device 
    and development board.
 
  - This application is configured to run by default on STM32469I-DISCO RevC board.
  - In order to run this application on RevA or RevB boards, replace the flag 
    USE_STM32469I_DISCO_REVC, which is defined in the pre-processor options, by 
    either USE_STM32469I_DISCO_REVA or USE_STM32469I_DISCO_REVB respectively.
 
@par How to use it ?

In order to make the program work, you must do the following :
 - The two directories "BACK" and "TOP" from "./Media/Pictures/Animated" 
   folder must be copied at the micro SD card root.
 - Connect a uSD Card to the Micro SD connector (CN9).
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
  

 */
