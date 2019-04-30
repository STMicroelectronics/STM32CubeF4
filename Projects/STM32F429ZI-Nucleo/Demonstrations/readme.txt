/**
  @page Demo   Demo NUCLEO-F429ZI
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Demonstrations/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of NUCLEO-F429ZI Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Demo Description

The provided demonstration firmware based on STM32Cube helps you to discover STM32 
Cortex-M devices that can be plugged on a STM32NUCLEO board. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 180 MHz.
 
 
Below you find the sequence to discover the demonstration :

 - Check the availability of adafruit 1.8" TFT shield on top of STM32 Nucleo 
   board. This is done by reading the state of IO PF.03 pin (mapped to JoyStick 
   available on adafruit 1.8" TFT shield). If the state of PF.03 is high then 
   the adafruit 1.8" TFT shield is available.
    
   - Adafruit 1.8" TFT shield is not available:
     LED2 is toggling with a first frequency equal to ~1Hz.
     Pressing User button lets LED2 toggling with a second frequency equal to ~5Hz.
     Pressing User button again, changes LED2 toggling frequency to ~10Hz.
     This is done in an infinite loop.      

   - Adafruit 1.8" TFT shield is available:
     A menu will be displayed on TFT. Follow instructions below:
     
     - Joystick DOWN to continue menu display
     - Choose the desired display mode: Press Joystick DOWN for automatic mode 
       or Joystick UP for manual mode.
       
       - Manual Mode selected:
         Images available on SD Card, are displayed by pressing Joystick RIGHT 
         to display next image or Joystick LEFT to display previous one. 
         Pressing long (~1s) Joystick SEL, switches display mode to automatic one.
       
       - Automatic Mode selected:
         Images available on SD Card are displayed sequentially in a forever loop.


It is worth noting that the application manages some errors occurred during the 
access to uSD card to parse bmp images:

 - If SD Card is not FAT formatted, a message will be displayed on TFT. 
   In this case, format the SD card and put into its root directory the .bmp 
   files available within the "BMP_128x160" folder under "Utilities/Media/Pictures"

 - If the content of the SD card is other than a bitmap file, a message will 
   be displayed on TFT mentioning that it is not supported.
   In this case, put into the SD card's root directory the .bmp
   files available within the "BMP_128x160" folder under "Utilities/Media/Pictures" 
   which are respecting the following criteria:
    o Dimensions: 128x160
    o Width: 128 pixels
    o Height: 160 pixels
    o Bit depth: 16
    o Item type: BMP file
    o The name of the bmp image file must not exceed 11 characters (including
      .bmp extension).


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, 
      then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

@note The clock setting is configured to have the max product performance (max clock frequency) 
      so not optimized in term of power consumption.

@par Directory contents
 
  - Demonstrations/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - Demonstrations/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - Demonstrations/Inc/main.h                  Header for main.c module
  - Demonstrations/Inc/sd_diskio.h             FatFS sd diskio driver header file
  - Demonstrations/Inc/fatfs_storage.h         Header for fatfs_storage.c module
  - Demonstrations/Inc/ffconf.h                FAT file system module configuration file   
  - Demonstrations/Src/stm32f4xx_it.c          Interrupt handlers
  - Demonstrations/Src/main.c                  Main program
  - Demonstrations/Src/sd_diskio.c             FatFS sd diskio driver implementation
  - Demonstrations/Src/fatfs_storage.c         Storage (FatFs) driver
  - Demonstrations/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This demonstration runs on STM32F429ZI devices.
    
  - This demonstration has been tested with STMicroelectronics NUCLEO-F429ZI Rev.B 
    boards and can be easily tailored to any other supported device and development board.
    
  - A SDSC microSD card (capacity up to 8GB), must contain the .bmp files
    available within the "BMP_128x160" folder under "Utilities/Media/Pictures" 
    
  - Adafruit 1.8" TFT shield must be connected on CN7, CN8, CN9 and CN10 Arduino connectors, 
    for more details please refer to UM1726.

For more details about the adafruit 1.8" TFT shield, please visit: 
http://www.adafruit.com/blog/2012/04/26/new-product-adafruit-1-8-18-bit-color-tft-shield-wmicrosd-and-joystick/


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the demonstration

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 
