/**
  @page LTDC_AnimatedPictureFromUSB LTDC Animated Picture From USB application 
  
  @verbatim
  ******************************************************************************
  * @file    Display/LTDC_AnimatedPictureFromUSB/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LTDC Animated Picture From USB application.
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

  This application describes how to display on LCD pictures saved under USB mass storage.
   
  The user has to put the bitmap pictures available within the "BMP_240x320" folder
  under "/Utilities/Media/Pictures" that don't exceed LCD size(width: 240, high: 320)
  under root of USB mass storage.
          
  Once the LCD, USB OTG HS and file system are initialized and configured, a check
  of the existence and the content of the USB mass storage directory is done.
 
  A list of USB mass storage content will be displayed followed by a message 
  indicating that a press of user button is needed to start the display of
  existing images.

@note : Once all images are displayed, the process is repeated.
      
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Graphic, Display, LTDC, Pictures, SD Card, BMP, Background, Foreground, Layers,

@par Directory contents

    - Display/LTDC_AnimatedPictureFromUSB/Inc/main.h                 Main configuration file
    - Display/LTDC_AnimatedPictureFromUSB/Inc/usbh_diskio_dma.h      FatFS usbh diskio driver header file
    - Display/LTDC_AnimatedPictureFromUSB/Inc/stm32f4xx_it.h         Interrupt handlers header file
    - Display/LTDC_AnimatedPictureFromUSB/Inc/stm32f4xx_hal_conf.h   HAL Configuration file 
    - Display/LTDC_AnimatedPictureFromUSB/Inc/ffconf.h               FAT file system module configuration file
    - Display/LTDC_AnimatedPictureFromUSB/Inc/lcd_log_conf.h         Configuration file for LCD_Log module
    - Display/LTDC_AnimatedPictureFromUSB/Inc/usbh_conf.h            Configuration file for USB module
    - Display/LTDC_AnimatedPictureFromUSB/Src/main.c                 Main program 
    - Display/LTDC_AnimatedPictureFromUSB/Src/usbh_diskio_dma.c      FatFS usbh diskio driver implementation
    - Display/LTDC_AnimatedPictureFromUSB/Src/stm32f4xx_it.c         Interrupt handlers
    - Display/LTDC_AnimatedPictureFromUSB/Src/usbh_conf.c            Main function to configure USB
    - Display/LTDC_AnimatedPictureFromUSB/Src/system_stm32f4xx.c     STM32F4xx system clock configuration file


@par Hardware and Software environment

  - This application runs on STM32F429xx Devices.
    
  - This application has been tested with STM32F429I-Discovery RevC board and can be
    easily tailored to any other supported device and development board.  

  - STM32F429I-DISCO RevB Set-up
    - Plug the USB key into the STM32F429I-DISCO board through 'USB micro A-Male 
      to A-Female' cable(CN6). 
 
 
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 

                                    */
