/**
  @page BSP  Example on how to use the BSP drivers
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

@par Example Description 

This example provides a description of how to use the different BSP drivers. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 100 MHz and provide 28 MHz at the output PLL divided by PLL_Q. 
This frequency permit to reach 14 Mhz clock needed for SD operation and in line 
with microSD specification. 

This example shows how to use the different functionalities of LCD, SD card, 
EEPROM, QUADSPI, audio playback and record by switching between all tests 
using joystick selection button. 
At the beginning, all leds are ON.

#### JOYSTICK ####
Use the joystick button to move a pointer inside a rectangle 
(up/down/right/left).

#### LCD ####
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

#### TOUCH SCREEN ####
This example shows how to use Touch Screen mounted on the LCD. The touched circle
will be filled with its color.

#### SD ####
Fourthly, this example shows how to erase, write and read the SD card and also 
how to detect the presence of the card.
This is performed in polling and DMA modes. Card detection is tested in polling 
and interrupt modes.

#### LOG ####
This example show how to use the LCD log features.

#### EEPROM ####
This example shows how to read and write data in RF EEPROM. The I2C EEPROM
memory (M24LR64) is available on separate daughter board ANT7-M24LR-A, which is not
provided with the STM32412G-DISCOVERY board. To use this driver you have to connect the 
ANT7-M24LR-A to CN10 connector of STM32412G-DISCOVERY board.

#### QSPI ####
This example shows how to use the QSPI features(read/write/erase..).

#### AUDIO PLAY ####

This example shows how to play an audio file through the I2S peripheral
using the external codec WM8994 implemented on the STM32412G-DISCOVERY board. The I2S input 
clock, provided by a dedicated PLL (PLLI2S), is configured initially to have an audio  
sampling frequency at 48 KHz. The audio data is stored in the internal flash memory 
(4 channels, 16-bit, 48 KHz). Following the instruction on the display, stream can be 
paused and resumed(Long press on joystick selection button) , volume can be changed and 
sample frequency can be changed.

#### AUDIO REC ANALOG ####
This example shows how to record an audio file through the I2S peripheral
using the external codec WM8994 implemented on the STM32412G-Discovery board. The I2S input 
clock, provided by a dedicated PLL (PLLI2S), is configured to have an audio sampling 
frequency at 16 KHz. The test records an audio sample from input analog microphone (CN9 connector)
and playback it right after to the headphones connected to CN9 audio jack connector. 
Connect a headset with built-in microphone to CN9 jack connector.

#### AUDIO REC DFSDM ####
This example shows how to record and playback audio from MEMs microphones using 
DFSDM input capture. The test uses the two MP34DT01TR microphones soldered on TOP
side of the board (U4 and U5). A 64KBytes circular buffer is used to record
data from microphones. The stream is then sent in same time to WM8994 codec via I2S communication
to ear sound from microphones in headphones connected to CN9 jack connector.

At the end of the ten examples when pushing the joystick selection button the application loops 
to the beginning (first examples). 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

BSP, LCD, Touchscreen, PSRAM, DMA, QUADSPI, Erase, Read, Write, Audio play, Record, MEMS, Audio Codec, DFSDM,
I2S, Microphone, Headphones

@par Directory contents 

  - BSP/Src/main.c                     Main program
  - BSP/Src/system_stm32f4xx.c         STM32F4xx system clock configuration file
  - BSP/Src/stm32f4xx_it.c             Interrupt handlers 
  - BSP/Src/joystick.c                 Joystick feature
  - BSP/Src/lcd.c                      LCD drawing features
  - BSP/Src/touchscreen.c              Touch Screen features  
  - BSP/Src/log.c                      LCD Log firmware functions
  - BSP/Src/sd.c                       SD features
  - BSP/Src/eeprom.c                   EEPROM features      
  - BSP/Src/qspi.c                     QSPI features      
  - BSP/Src/audio.c                    Audio playback features      
  - BSP/Src/audio_rec_analog.c         Analog audio record features      
  - BSP/Src/audio_rec_dfsdm.c          DFSDM audio record features      
  - BSP/Inc/main.h                     Main program header file  
  - BSP/Inc/stm32f4xx_hal_conf.h       HAL configuration file
  - BSP/Inc/stm32f4xx_it.h             Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h             lcd_log configuration template file
  - BSP/Inc/stlogo.h                   Image used for BSP example
  - BSP/Inc/image_f4.h                 Image used for BSP example
        
        
@par Hardware and Software environment  

  - This example runs on STM32F412xx devices.
  
  - This example has been tested with STMicroelectronics STM32412G-DISCOVERY 
    board and can be easily tailored to any other supported device 
    and development board.
    
  
@par How to use it ? 

 - Use STLink utility, available on www.st.com or any other in system programming
   tool to load "BSP/Binary/audio_sample.bin" file to the STM32 internal flash 
   at the address 0x08040000.

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
     @note Make sure that the tool flash loader does not erase or overwrite the
        loaded audio file at address 0x08040000 by limiting the application
        end address to 0x0803FFFF. This is already done for the example project
 - Connect a headphone with a built-in microphone to the audio jack connector (CN9).


 */
