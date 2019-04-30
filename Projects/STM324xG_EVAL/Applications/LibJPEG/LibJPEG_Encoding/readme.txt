/**
  @page LibJPEG_Encoding LibJPEG encoding application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LibJPEG/LibJPEG_Encoding/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LibJPEG encoding application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
   @endverbatim

@par Application Description

This application demonstrates how to read BMP file from micro SD, encode it, save the jpeg file
in uSD Card then decode the jpeg file and display the final BMP image on the LCD.
BMP image is read from the bottom to the top, in order to be encoded in JPG from the top to the bottom.
BMP image is in BGR format and JPG in RGB format. B and R values of the pixels are swapped by redefining
the order of pixel components.

The BMP image should be copied at the micro SD card root.

The images must have the following properties:
 - named as "image.bmp"
 - 320*240 size

The image size can be modified by changing IMAGE_WIDTH and IMAGE_HEIGHT defines 
in "main.h" file to decode other resolution than 320x240.

@note IMAGE_WIDTH and IMAGE_HEIGHT values must be lower or equal to LCD screen resolution.
 - LCD width  = 320 
 - LCD height = 240

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents

    - LibJPEG/LibJPEG_Encoding/Inc/main.h                      Main program header file
    - LibJPEG/LibJPEG_Encoding/Inc/sd_diskio.h                 FatFS sd diskio driver header file
    - LibJPEG/LibJPEG_Encoding/Inc/stm32f4xx_it.h              Interrupt handlers header file
    - LibJPEG/LibJPEG_Encoding/Inc/stm32f4xx_hal_conf.h        HAL Configuration file 
    - LibJPEG/LibJPEG_Encoding/Inc/ffconf.h                    FAT file system module configuration file
    - LibJPEG/LibJPEG_Encoding/Inc/encode.h                    Encoder header file
    - LibJPEG/LibJPEG_Encoding/Inc/decode.h                    Decoder header file
    - LibJPEG/LibJPEG_Encoding/Inc/jmorecfg.h                  Lib JPEG configuration file (advanced configuration).
    - LibJPEG/LibJPEG_Encoding/Inc/jconfig.h                   Lib JPEG configuration file.
    - LibJPEG/LibJPEG_Encoding/Inc/jdata_conf.h                Write/Read methods definition       
    - LibJPEG/LibJPEG_Encoding/Src/main.c                      Main program  
    - LibJPEG/LibJPEG_Encoding/Src/sd_diskio.c                 FatFS sd diskio driver implementation
    - LibJPEG/LibJPEG_Encoding/Src/stm32f4xx_it.c              Interrupt handlers
    - LibJPEG/LibJPEG_Encoding/Src/system_stm32f4xx.c          STM32F4xx system clock configuration file
    - LibJPEG/LibJPEG_Encoding/Src/encode.c                    Encoder file
    - LibJPEG/LibJPEG_Encoding/Src/decode.c                    Decoder file
    - LibJPEG/LibJPEG_Encoding/Src/jdata_conf.c                Write/Read methods definition


@par Hardware and Software environment

  - This application runs on STM32F407xx/417xx devices.
    
  - This application has been tested with STMicroelectronics STM324xG-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 

  - STM324xG-EVAL Set-up
   - Connect a uSD Card to the MSD connector (CN6). 
   - Please ensure that jumpers JP16 and JP22 are in position 1-2.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   
