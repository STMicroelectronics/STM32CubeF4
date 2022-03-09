/**
  @page SAI_AUDIO  How to use the Audio features with SAI peripheral
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    SAI/SAI_Audio/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32F4xx SAI Audio example.
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

Basic implementation of audio features using BSP_AUDIO.
This example uses the SAI peripheral and the external WM8994 codec implemented on the STM32446E-EVAL board
to play two audio files at the same time.

In this example the SAI input clock, provided by a dedicated PLL (PLLI2S), is
configured to have an audio sampling frequency at 48 KHz on four channels:
stereo on AUDIO1 (headset) and stereo on AUDIO2 (speakers).

This example uses an audio codec driver which consists of three independent layers:
 - Codec Hardware layer: which controls and communicates with the audio codec 
   (WM8994) implemented on the evaluation board.
 - MAL (Media Access Layer): which controls and interfaces with storage media 
   storing or providing the audio file/stream.
 - The high layer: which implements overall control and interfacing API allowing
   to perform basic audio operations (Init, Play, Pause, Resume, Stop, Volume 
   control and audio file pointer management)
  
In this example the audio data is stored in the internal flash memory (Stereo, 
16-bit, 48 KHz).
The example also manages information display and control interface
through push buttons: 
 - When the application is Playing audio file:
     Short press on User push-button : Volume DOWN until looping
     Long press on User push-button : Pause/Resume

This example plays the audio data stored in internal flash memory and replays it
when it reaches end of file. But it can be tailored to use different media storage
devices; SDCard (through SDIO), external Memory (through FMC),... or to play 
in streaming mode (ie. from USB port in device or host mode). 

The WM8994 codec support the TDM format, for this the SAI peripheral is configured 
with 4 time slot (2 time slots left/right channels for headphone and 2 time slots 
left/right channels for speaker).
In the WM8994 configuration the SLOT0 (2 time slots left/right) is dedicated for 
the headphone and SLOT1 (2 time slots left/right) is dedicated for speaker.

List of Known Limitations and more detailed user notes are provided in file 
stm32446e_eval_audio.c   (under Drivers\BSP\STM32446E_EVAL)

The provided sample audio files (stored in internal flash memory) are:
- Downloaded from http://www.freesound.org
- Licensed under the Creative Commons 0 License:
  http://creativecommons.org/publicdomain/zero/1.0/


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Audio, SAI, DMA, play, pause, headphone, audio codec

@par Directory contents 

  - SAI/SAI_Audio/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - SAI/SAI_Audio/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - SAI/SAI_Audio/Inc/main.h                        Header for main.c module  
  - SAI/SAI_Audio/Src/stm32f4xx_it.c          Interrupt handlers
  - SAI/SAI_Audio/Src/main.c                        Main program
  - SAI/SAI_Audio/Src/system_stm32f4xx.c      STM32F4xx system source file

      
@par Hardware and Software environment 

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with STM32446E-EVAL board and can be
    easily tailored to any other supported device and development board.      


@par How to use it ? 
 - Use STLink utility, available on www.st.com or any other in system programming
   tool to load "SAI/SAI_Audio/Binary/audio_sample_tdm.bin" file to the STM32 internal flash at the address 0x08010000.

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
         @note Make sure that the tool flash loader does not erase or overwrite the
            loaded audio file at address 0x08010000-0x0807FFFF by limiting the application
            end address to 0x08010000-1. This is already done for the example project
         @note This example runs only with (no optimization) on stm32f4xx_hal_dma.c file. 
 - Connect a headphone and a speaker to the audio jack connectors (CN22/CN23).


 */
