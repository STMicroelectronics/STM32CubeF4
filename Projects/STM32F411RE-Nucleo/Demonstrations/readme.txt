/**
  @page Demonstrations   Demonstrations STM32F411RE-Nucleo
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Demonstrations/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STM32F411RE-Nucleo Demo
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

@par Demo Description

Demonstration firmware based on STM32Cube. This example helps you to discover
STM32 Cortex-M devices that are plugged onto your STM32 Nucleo board.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 32 MHz.
 
 
Below you find the sequence to discover the demonstration :

 - Check the availability of adafruit 1.8" TFT shield on top of STM32 Nucleo 
   board. This is done by reading the state of IO PB.00 pin (mapped to JoyStick 
   available on adafruit 1.8" TFT shield). If the state of PB.00 is high then 
   the adafruit 1.8" TFT shield is available.
    
   - Adafruit 1.8" TFT shield is not available:
     LED2 is toggling with a first frequency equal to ~1Hz.
     Pressing User button lets LED2 toggling with a second frequency equal to ~5Hz.
     Pressing User button again, changes LED2 toggling frequency to ~10Hz.
     This is done in an infinite loop.      

   - Adafruit 1.8" TFT shield is available:
     LED2 is turned ON, because it's sharing the same pin with the SPI CLK signal
     used to communicate with the Adafruit 1.8" TFT shield.
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

@par Keywords

Demonstrations, Adafruit, Shield, TFT, Joystick, SD Card, Display

@par Directory contents
 
  - Demonstrations/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - Demonstrations/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - Demonstrations/Inc/main.h                  Header for main.c module
  - Demonstrations/Inc/fatfs_storage.h         Header for fatfs_storage.c module
  - Demonstrations/Inc/ffconf.h                FAT file system module configuration file   
  - Demonstrations/Src/stm32f4xx_it.c          Interrupt handlers
  - Demonstrations/Src/main.c                  Main program
  - Demonstrations/Src/fatfs_storage.c         Storage (FatFs) driver
  - Demonstrations/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This demonstration runs on STM32F411xEx devices.
    
  - This demonstration has been tested with STMicroelectronics STM32F4xx-Nucleo RevC 
    discovery boards and can be easily tailored to any other supported device 
    and development board.
    
  - A SDSC microSD card (capacity up to 4GB), must contain the .bmp files
    available within the "BMP_128x160" folder under "Utilities/Media/Pictures" 
    
  - Adafruit 1.8" TFT shield must be connected on CN7,CN8, CN9 and CN10 Arduino connectors, 
    for more details please refer to UM1726.

For more details about the adafruit 1.8" TFT shield, please visit: 
http://www.adafruit.com/blog/2012/04/26/new-product-adafruit-1-8-18-bit-color-tft-shield-wmicrosd-and-joystick/


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the demonstration


 */
