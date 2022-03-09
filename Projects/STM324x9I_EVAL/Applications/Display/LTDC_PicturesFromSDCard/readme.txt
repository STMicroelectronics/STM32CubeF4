/**
  @page LTDC_PicturesFromSDCard LTDC pictures from SD card application
  
  @verbatim
  ******************************************************************************
  * @file    Display/LTDC_PicturesFromSDCard/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LTDC pictures from SD card application.
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
 
The user has to create a "Media" directory under the micro SD card root, where he
copies inside the content of "/BMP_320x240" and "/BMP_480x272" folders provided under 
"/Utilities/Media/Pictures"
   
 
@note : The user can add pictures of his choice but the file type, resolution
        and pixel format described below must be respected.
           
  Once the LCD, SD card and file systems are initialized and configured, 
  a content check of the "Media" directory is done and the number of ".BMP" files 
  is retained.
  
@note : An associate LCD eval driver is used in this application 
  
@note : The maximum number of BMP file is fixed at 25. It can be raised 
        until reaching the maximum of SD card memory space.

@note : When the SDcard is used the Camera module must be unplugged, this is due to
        the shared pins between the two devices. 
		
  The following steps are performed to scroll all the images stored in the 
  SD Card :
 
  Step1 :
  ------
  The foreground layer is set, the image copied from SD card to intermediate
  SDRAM memory and then copied to LCD frame buffer. The image is fully visible when 
  the level of transparency is increased until it becomes totally opaque (reaches 255).
 
  Press tamper button to display next image.
 
  Step2 :
  ------
  When the Tamper button is pressed the transparency of the foreground layer
  begins to decrease until become totally transparent and the background layer 
  is set, the image copied from SD card to intermediate SDRAM memory and then 
  copied to LCD frame buffer and finally the level of transparency of the 
  background layer increase until it reaches 255 (totally opaque). 
            
  Press tamper button to display next image.
 
  Step3 :
  ------
  When the Tamper button is pressed the transparency of the background layer
  begins to decrease until become totally transparent and return to step1.
  
@note the system clock (SYSCLK) is configured to run at 175 MHz and 50 MHz is provided 
      at the output PLL divided by PLL_Q. This frequency permit to reach 25 MHz clock 
      needed for SD operation and in line with microSD specification. 
      
@note 
  => If the "Media" directory is empty, a warning message is displayed on 
     the LCD : "  No Bitmap files...  "
    
  => If the file type stored in the "Media" directory is not supported,
     a warning message is displayed on the LCD : " file type not supported. "
    
  => If the SD card is not inserted, a warning message is displayed on the LCD :
     " Please insert SD Card. "
   
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

  -  LED3 is ON when the uSD disk I/O driver is not Linked;

@par Keywords

Graphic, Display, LTDC, LCD, Pictures, SD Card, BMP, RGB565, RGB888, ARGB8888

@par Directory contents

    - Display/LTDC_PicturesFromSDCard/Inc/main.h                             Main configuration file
    - Display/LTDC_PicturesFromSDCard/Inc/sd_diskio.h                        FatFS sd diskio driver header file
    - Display/LTDC_PicturesFromSDCard/Inc/stm32f4xx_it.h                     Interrupt handlers header file
    - Display/LTDC_PicturesFromSDCard/Inc/stm32f4xx_hal_conf.h               HAL Configuration file 
    - Display/LTDC_PicturesFromSDCard/Inc/ffconf.h                           FAT file system module configuration file
    - Display/LTDC_PicturesFromSDCard/Inc/fatfs_storage.h                    Header for fatfs_storage.c
    - Display/LTDC_PicturesFromSDCard/Src/main.c                             Main program 
    - Display/LTDC_PicturesFromSDCard/Src/sd_diskio.c                        FatFS sd diskio driver implementation
    - Display/LTDC_PicturesFromSDCard/Src/fatfs_storage.c                    Storage (FatFs) driver
    - Display/LTDC_PicturesFromSDCard/Src/stm32f4xx_it.c                     Interrupt handlers
    - Display/LTDC_PicturesFromSDCard/Src/system_stm32f4xx.c                 STM32F4xx system clock configuration file


@par Hardware and Software environment  

  - This application runs on and STM32F429xx/439xx devices.
    
  - This application has been tested with STMicroelectronics STM324x9I-EVAL RevB
    evaluation boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - The bitmap images, available under "/BMP_320x240" and "/BMP_480x272" folders 
   provided under "/Utilities/Media/Pictures" should be copied inside a 
   dedicated directory named "Media" at the micro SD card root.
   The images should have the following properties:
     file type    : *.bmp
     resolution   : up to 480x272 with AM480272H3TMQW LCD and up to 640x480 with 
                    AM-640480G5TNQW-T00H LCD
     pixel format : RGB565, RGB888 or ARGB8888     
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application 
                

                                    */
