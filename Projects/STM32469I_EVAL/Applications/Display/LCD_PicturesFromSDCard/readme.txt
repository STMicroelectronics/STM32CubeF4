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

This application describes how to display pictures, saved on SD card, on LCD DSI, .
 
Prior to starting the application, the user has to create a "/Media" directory under the micro SD card root and
copy the pictures stored in PC directory "./Media/".
   
 
@note : The user can add pictures of his choice but the file type, resolution
        and pixel format described below must be respected.
           
  Once the LCD, SD card and file systems initialized and configured, 
  a content check of the "Media" directory is done and the number of ".BMP" files 
  is retained.
  
@note : An associate LCD eval driver is used in this application 
  
@note : The maximum number of BMP file is fixed to 25. It can be raised 
        until reaching the maximum of SD card memory space.

  The following steps are performed to scroll all the images stored in the 
  SD Card :
 
  Step1 :
  ------
  The foreground layer is set, the image is copied from SD card to intermediate
  SDRAM memory and then copied to LCD frame buffer. The image is fully visible when 
  the level of transparency is increased until it becomes totally opaque (reaches 255).
  
  Step2 :
  ------
  In a second step, the transparency of the foreground layer
  begins to decrease until become totally transparent and the background layer 
  is set, the image copied from SD card to intermediate SDRAM memory and then 
  copied to LCD frame buffer and finally the level of transparency of the 
  background layer increases until it reaches 255 (totally opaque). 
             
  Step3 :
  ------
  In this step, the transparency of the background layer
  begins to decrease until becoming totally transparent and returns to step 1.
  
@note the system clock (SYSCLK) is configured to run at 180 MHz and 50 MHz is provided 
      at the output PLL divided by PLL_Q. This frequency permits to reach 25 MHz clock 
      needed for SD operation and in line with microSD specification. 
      
@note 
  => If the "Media" directory is empty, a warning message is displayed on 
     the LCD : "  No Bitmap files...  "
    
  => If the file type stored in the "Media" directory is not supported,
     a warning message is displayed on the LCD : " file type not supported. "
    
  => If the SD card is not inserted, a warning message is displayed on the LCD :
     "  No Bitmap files...  "
   
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

  -  LED3 is ON when the uSD disk I/O driver is not Linked;

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

  - This application runs on and STM32F469xx or STM32F479xx devices.
    
  - This application has been tested with STMicroelectronics STM32469I-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - The bitmap images, available under ".Media/" 
   should be copied inside a  dedicated directory named "Media" at the micro SD card root.
   The images should have the following properties:
     file type    : *.bmp
     resolution   : up to 800x480
     pixel format : RGB565, RGB888 or ARGB8888     
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application 
                

                                    */
