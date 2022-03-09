/**
  @page LCD_PicturesFromSDCard LCD pictures from SD card application

  @verbatim
  ******************************************************************************
  * @file    Display/LCD_PicturesFromSDCard/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD pictures from SD card application.
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

This application describes how to display on LCD pictures saved under SD card.

The image copied from SD card into internal SRAM and then displayed on LCD.
By pressing User button, the application displays the next image.

Once the LCD, SD card and file systems are initialized and configured,
a content check of the "Media" directory is done and the number of ".BMP" files
is retained.

User has to create a "Media" directory under the micro SD card root, where he
copies images with the following properties:
     file type    : *.bmp
     resolution   : 240x240
     pixel format : RGB565 (16-bits)

@note : The user can add pictures of his choice but the file type, resolution
        and pixel format described below must be respected.

@note : Maximum accepted length of image file name is 11. (characters including the extension .bmp)

@note : An associate LCD discovery driver is used in this application.

@note : The maximum number of BMP file is fixed at 25. It can be raised
        until reaching the maximum of SD card memory space.

@note
  => If the "Media" directory is empty, a warning message is displayed on 
     the LCD : "No Bitmap files..."

  => If the SD card is not inserted, a warning message is displayed on the LCD :
     "Please insert SD Card."

  => If the SD card is unplugged while the program is running, after User button is pressed a warning message is displayed on the LCD:
     "Please insert SD Card"
     "Reset the board"
     
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

  -  LED4 is ON when the uSD disk I/O driver is not Linked or an error has occurred.

@par Keywords

Graphic, Display, LCD, Pictures, SD Card, BMP, RGB565, RGB888, ARGB8888

@par Directory contents

    - Display/LCD_PicturesFromSDCard/Inc/main.h                             Main configuration file
    - Display/LCD_PicturesFromSDCard/Inc/sd_diskio.h                        FatFS sd diskio driver header file
    - Display/LCD_PicturesFromSDCard/Inc/stm32f4xx_it.h                     Interrupt handlers header file
    - Display/LCD_PicturesFromSDCard/Inc/stm32f4xx_hal_conf.h               HAL Configuration file 
    - Display/LCD_PicturesFromSDCard/Inc/ffconf.h                           FAT file system module configuration file
    - Display/LCD_PicturesFromSDCard/Inc/fatfs_storage.h                    Header for fatfs_storage.c
    - Display/LCD_PicturesFromSDCard/Src/main.c                             Main program 
    - Display/LCD_PicturesFromSDCard/Src/sd_diskio.c                        FatFS sd diskio driver implementation
    - Display/LCD_PicturesFromSDCard/Src/fatfs_storage.c                    Storage (FatFs) driver
    - Display/LCD_PicturesFromSDCard/Src/stm32f4xx_it.c                     Interrupt handlers
    - Display/LCD_PicturesFromSDCard/Src/system_stm32f4xx.c                 STM32F4xx system clock configuration file

@par Hardware and Software environment

  - This application runs on and STM32F413xx devices.
    
  - This application has been tested with STMicroelectronics STM32F413H-DISCOVERY
    boards and can be easily tailored to any other supported device
    and development board.

  - STM32F413H-Discovery Set-up
    - Connect the STM32F413H-Discovery board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector
    - Connect a uSD Card to the MSD connector (CN1).

@par How to use it ?

In order to make the program work, you must do the following :
 - The bitmap images, available under "/BMP_240x240" folder 
   provided under "/Utilities/Media/Pictures" might be copied inside a 
   dedicated directory named "Media" at the micro SD card root.
   The images should have the following properties:
     file type    : *.bmp
     resolution   : 240x240
     pixel format : RGB565 (16-bits)
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application


 */
