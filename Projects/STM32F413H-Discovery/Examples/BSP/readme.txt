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

This example provides 2 project configurations:
- USE_STM32F413H_DISCOVERY_REVE : Board contains the FT3x67 Touch Screen component.
- USE_STM32F413H_DISCOVERY : Board contains the FT6x06 Touch Screen component.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 100 MHz and provide 50 MHz at the output PLL divided by PLL_Q. 
This frequency permit to reach 25 Mhz clock needed for SD operation and in line 
with microSD specification. 


This example shows how to use the different functionalities of LCD, SD card, QUADSPI, 
PSRAM by switching between all tests using USER selection button. 

#### LCD ####
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

#### TOUCH SCREEN ####
This example shows how to use Touch Screen mounted on the LCD. 
 - TS demo 1: Test of single touch: The touched circle will be filled with its color.
 - TS demo 2: Test of dual touch: Use two fingers to display 2 circles on the LCD,
              their positions will be displayed.
 - TS demo 3: Test of TS interruption: Each time a new interrupt is detected when touching 
              the LCD, a rectangle correspondent to the touched area will be colored.              
#### SD ####
Fourthly, this example shows how to erase, write and read the SD card and also 
how to detect the presence of the card.
This is performed in polling and DMA modes. Card detection is tested in polling 
and interrupt modes.

#### LOG ####
This example show how to use the LCD log features.

#### QSPI ####
This example shows how to use the QSPI features(read/write/erase..).

#### PSARM ####
This example shows how to use the PSARM features(read/write/erase..).
This is performed in polling and DMA modes.

#### AUDIO PLAY ####
This example shows how to play an audio file through the I2S peripheral
using the external codec WM8994 implemented on the STM32F413H-DISCOVERY board. The I2S input 
clock, provided by a dedicated PLL (PLLI2S), is configured initially to have an audio  
sampling frequency at 48 KHz. The audio data is stored in the internal flash memory 
(4 channels, 16-bit, 48 KHz). Following the instruction on the display, stream can be 
paused and resumed, volume can be changed and sample frequency can be changed.


#### AUDIO REC/PLAY DFSDM ####
This example shows how to record and playback audio from MEMs microphones using 
DFSDM input capture. The test uses the two MP34DT01TR microphones soldered on TOP
side of the board (U16 and U17). A 64KBytes circular buffer is used to record
data from microphones. The stream is then sent in same time to WM8994 codec via I2S communication
to ear sound from microphones in headphones connected to CN5 jack connector.

At the end of the ten examples when pushing the USER button the application loops 
to the beginning (first examples). 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

BSP, Board support package, DMA, Audio play, Headphone, Audio record, microphone, Touchscreen, Joystick,
LCD, SD Card, EEPROM, NOR, SRAM, SDRAM, LOG, QSPI

@par Directory contents 

  - BSP/Src/main.c                     Main program
  - BSP/Src/system_stm32f4xx.c         STM32F4xx system clock configuration file
  - BSP/Src/stm32f4xx_it.c             Interrupt handlers 
  - BSP/Src/lcd.c                      LCD drawing features
  - BSP/Src/touchscreen.c              Touch Screen features  
  - BSP/Src/log.c                      LCD Log firmware functions
  - BSP/Src/sd.c                       SD features
  - BSP/Src/psram.c                    PSRAM features   
  - BSP/Src/qspi.c                     QSPI features  
  - BSP/Src/audio.c                    Audio playback features    
  - BSP/Src/audio_rec_dfsdm.c          DFSDM audio record features  
  - BSP/Inc/main.h                     Main program header file  
  - BSP/Inc/stm32f4xx_hal_conf.h       HAL configuration file
  - BSP/Inc/stm32f4xx_it.h             Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h             lcd_log configuration template file
  - BSP/Inc/stlogo.h                   Image used for BSP example
        
        
@par Hardware and Software environment  

  - This example runs on STM32F413xx devices.
  
  - This example has been tested with STMicroelectronics STM32F413H-DISCOVERY 
    board and can be easily tailored to any other supported device 
    and development board.
    
  
@par How to use it ? 

 - Use STLink utility, available on www.st.com or any other in system programming
   tool to load "Utilities/Media/Audio/audio_sample_tdm.bin" file to the STM32 internal flash 
   at the address 0x08040000.

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
      @note Make sure that the tool flash loader does not erase or overwrite the
        loaded audio file at address 0x08040000 by limiting the application
        end address to 0x0803FFFF. This is already done for the example project
 - Connect a headphone with a built-in microphone to the audio jack connector (CN5).
 

 */
