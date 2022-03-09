/**
  @page LibJPEG_Decoding LibJPEG decoding application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LibJPEG/LibJPEG_Decoding/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LibJPEG decoding application.
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

This application demonstrates how to read a jpeg file from SDCard memory, decode it 
and display the final BMP image on the LCD.

The JPEG images should be copied at the micro SD card's root.
Image samples can be found in ../../../../../Utilities/Media/Pictures/JPG

The images must have the following properties:
 - named as "image.jpg"
 - 320*240 size

The image size can be modified by changing IMAGE_WIDTH and IMAGE_HEIGHT defines 
in "main.h" file to decode other resolution than 320x240.

@note IMAGE_WIDTH and IMAGE_HEIGHT values must be lower or equal to LCD screen resolution.
 - LCD width  = 320 
 - LCD height = 240

@note If the displayed image's colors are inverted, please uncomment "SWAP_RB" define in "main.h" file.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, Graphic, LibJPEG, SD Card, LCD, DMA2D, Decoding, Flash memory, DMA2D, ARGB8888, RGB, BMP, JPG

@par Directory contents

    - LibJPEG/LibJPEG_Decoding/Inc/main.h                      Main program header file
    - LibJPEG/LibJPEG_Decoding/Inc/sd_diskio.h                 FatFS sd diskio driver header file
    - LibJPEG/LibJPEG_Decoding/Inc/stm32f4xx_it.h              Interrupt handlers header file
    - LibJPEG/LibJPEG_Decoding/Inc/stm32f4xx_hal_conf.h        HAL Configuration file 
    - LibJPEG/LibJPEG_Decoding/Inc/ffconf.h                    FAT file system module configuration file
    - LibJPEG/LibJPEG_Decoding/Inc/decode.h                    Decoder header file
    - LibJPEG/LibJPEG_Decoding/Inc/jmorecfg.h                  Lib JPEG configuration file (advanced configuration)
    - LibJPEG/LibJPEG_Decoding/Inc/jconfig.h                   Lib JPEG configuration file
    - LibJPEG/LibJPEG_Decoding/Inc/jdata_conf.h                Write/Read methods definition
    - LibJPEG/LibJPEG_Decoding/Src/main.c                      Main program  
    - LibJPEG/LibJPEG_Decoding/Src/sd_diskio.c                 FatFS sd diskio driver implementation
    - LibJPEG/LibJPEG_Decoding/Src/stm32f4xx_it.c              Interrupt handlers
    - LibJPEG/LibJPEG_Decoding/Src/system_stm32f4xx.c          STM32F4xx system clock configuration file
    - LibJPEG/LibJPEG_Decoding/Src/decode.c                    Decoder file
    - LibJPEG/LibJPEG_Decoding/Src/jdata_conf.c                Write/Read methods definition


@par Hardware and Software environment

  - This application runs on STM32F469xx devices.

  - This application has been tested with STM32469I-DISCOVERY board and can be easily tailored to any 
    other supported device and development board.

  - This application is configured to run by default on STM32469I-DISCO RevC board.
  - In order to run this application on RevA or RevB boards, replace the flag 
    USE_STM32469I_DISCO_REVC, which is defined in the pre-processor options, by 
    either USE_STM32469I_DISCO_REVA or USE_STM32469I_DISCO_REVB respectively.

  - STM32469I-DISCOVERY Set-up
   - Connect a uSD Card to the Micro SD connector (CN9) 

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
