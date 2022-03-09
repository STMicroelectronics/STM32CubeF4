/**
  @page LibJPEG_Encoding LibJPEG encoding application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics  *******************
  * @file    LibJPEG/LibJPEG_Encoding/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LibJPEG encoding application.
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

This application demonstrates how to read BMP file from USB disk, encode it, save the jpeg file
in USB disk then decode the jpeg file and display the final BMP image on the LCD.
BMP image is read from the bottom to the top, in order to be encoded in JPG from the top to the bottom.
BMP image is in BGR format and JPG in RGB format. B and R values of the pixels are swapped by redefining
the order of pixel components.

The BMP image should be copied at USB disk root.

The images must have the following properties:
 - named as "image.bmp"
 - 240*320 size

The image size can be modified by changing IMAGE_WIDTH and IMAGE_HEIGHT defines 
in "main.h" file to decode other resolution than 240x320.

@note IMAGE_WIDTH and IMAGE_HEIGHT values must be lower or equal to LCD screen resolution.
 - LCD width  = 240 
 - LCD height = 320 
 
Uncomment "SWAP_RB" define in "main.h" file if the displayed image color 
are inverted

The image transferring to the LCD_FRAME_BUFFER can be done with two ways; 
with CPU or with DMA2D. Uncomment "USE_DMA2D" define for DMA2D transfer and 
"DONT_USE_DMA2D" for CPU transfer in main.h file. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, Graphic, LibJPEG, Flash memory, LCD, DMA2D, SD Card, Encoding, ARGB8888, RGB, BMP, JPG

@par Directory contents

    - LibJPEG/LibJPEG_Encoding/Inc/main.h                      Main program header file
    - LibJPEG/LibJPEG_Encoding/Inc/usbh_diskio_dma.h           FatFS usbh diskio driver header file
    - LibJPEG/LibJPEG_Encoding/Inc/stm32f4xx_it.h              Interrupt handlers header file
    - LibJPEG/LibJPEG_Encoding/Inc/stm32f4xx_hal_conf.h        HAL Configuration file 
    - LibJPEG/LibJPEG_Encoding/Inc/ffconf.h                    FAT file system module configuration file
    - LibJPEG/LibJPEG_Encoding/Inc/encode.h                    Encoder header file
    - LibJPEG/LibJPEG_Encoding/Inc/decode.h                    Decoder header file
    - LibJPEG/LibJPEG_Encoding/Inc/jmorecfg.h                  Lib JPEG configuration file (advanced configuration)
    - LibJPEG/LibJPEG_Encoding/Inc/jconfig.h                   Lib JPEG configuration file 
    - LibJPEG/LibJPEG_Encoding/Inc/jdata_conf.h                Write/Read methods definition     
    - LibJPEG/LibJPEG_Encoding/Src/main.c                      Main program  
    - LibJPEG/LibJPEG_Encoding/Src/usbh_diskio_dma.c           FatFS usbh diskio driver implementation
    - LibJPEG/LibJPEG_Encoding/Src/stm32f4xx_it.c              Interrupt handlers
    - LibJPEG/LibJPEG_Encoding/Src/system_stm32f4xx.c          STM32F4xx system clock configuration file
    - LibJPEG/LibJPEG_Encoding/Src/encode.c                    Encoder file
    - LibJPEG/LibJPEG_Encoding/Src/decode.c                    Decoder file
    - LibJPEG/LibJPEG_Encoding/Src/jdata_conf.c                Write/Read methods definition


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
