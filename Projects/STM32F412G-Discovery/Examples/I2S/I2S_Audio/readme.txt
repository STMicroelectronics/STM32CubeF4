/**
  @page I2S_AUDIO  How to use the Audio features with I2S peripheral
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    I2S/I2S_Audio/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2S Audio Example.
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

Basic implementation of audio features. This example allows playing an audio 
file with an external codec on the STM32F412G-Discovery board through the I2S 
peripheral using DMA transfer.

In this example the I2S input clock, provided by a dedicated PLL (PLLI2S),  is
configured to have an audio sampling frequency at 8 KHz with Master clock enabled.

This example uses an audio codec driver which consists of two independent layers:
 - Codec Hardware layer: which controls and communicates with the audio codec 
   (CS43L22) implemented on the evaluation board.
 - The high layer: which implements overall control and interfacing API allowing
   to perform basic audio operations (Init, Play, Pause, Resume, Stop, Volume 
   control and audio file pointer management)
  
In this example the audio file is stored in the internal flash memory (Mono, 
16-bit, 8 KHz). It also manages information display and control interface
through push buttons: 
 - When the application is Playing audio file:
     + Joystick Sel push-button push button: Pause
     + Joystick UP push-button push button: Volume UP
     + Joystick DOWN push-button: Volume DOWN
 - When the application is Paused:
     + Joystick Sel push-button push button: Play
     + Joystick UP push-button push button: No action
     + Joystick DOWN push-button: No action
This example plays the audio file stored in internal flash memory and replays it
when it reaches end of file. But it can be tailored to used different media storage
devices (SDCard (through SDIO), external Memory (through FSMC) ...) or to play 
in streaming mode (ie. from USB port in device or host mode). In this case, Circular
DMA mode should be preferred (by enabling the define DMA_CIRCULAR in
I2Sx_MspInit() API of stm32412g_discovery_audio.c file).  

List of Known Limitations and more detailed user notes are provided in file 
stm32412g_discovery_audio.c   (under Drivers\BSP\STM32412G-DISCOVERY)

The provided sample Audio file (stored in internal Flash memory) is extracted from:
  - Title: Partita E major, Gavotte en rondeau  
  - Artist/Composer: J.-S. Bach

@par Keywords

Connectivity, I2S, DMA, Audio codec, Joystick

@par Directory contents 

  - I2S/I2S_Audio/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - I2S/I2S_Audio/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - I2S/I2S_Audio/Inc/main.h                  Header for main.c module  
  - I2S/I2S_Audio/Src/stm32f4xx_it.c          Interrupt handlers
  - I2S/I2S_Audio/Src/main.c                  Main program
  - I2S/I2S_Audio/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - I2S/I2S_Audio/Src/system_stm32f4xx.c      STM32F4xx system source file
  - I2S/I2S_Audio/Src/audio_sample.c          Audio Sample file (in tab format)


@par Hardware and Software environment

  - This example runs on STM32F412xG devices.
    
  - This example has been tested with STM32412G-DISCOVERY board and can be
    easily tailored to any other supported device and development board.

  - STM32412G-DISCOVERY Set-up

@par How to use it ? 

 - Use STLink utility, available on www.st.com or any other in system programming
   tool to load a ".wav" file to the STM32 internal flash at the address 0x08080000.
   Make sure the audio file size does not exceed 512KB. When using STLink utility change
   the audio file extension to "*.bin" in order to be able to load it directly to flash memory.
         @note Audio file present in .\Utilities\Media\AudioFile\ folder
   
In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
         @note Make sure that the tool flash loader does not erase or overwrite the
            loaded audio file at address 0x08080000 by limiting the application
            end address to 0x0807FFFF. This is already done for the example project 
 - Connect a headphone to the audio jack connector (CN9).

 */
