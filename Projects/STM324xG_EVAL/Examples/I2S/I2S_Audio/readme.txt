/**
  @page I2S_AUDIO  I2S Audio example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    I2S/I2S_Audio/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2S Audio example.
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
file with an external codec on the STM324xG_EVAL board through the I2S 
peripheral using DMA transfer.

In this example the I2S input clock, provided by a dedicated PLL (PLLI2S),  is
configured to have an audio sampling frequency at 48 KHz with Master clock enabled.

This example uses an audio codec driver which consists of three independent layers:
 - Codec Hardware layer: which controls and communicates with the audio codec 
   (CS43L22) implemented on the evaluation board.
 - MAL (Media Access Layer): which controls and interfaces with storage media 
   storing or providing the audio file/stream.
 - The high layer: which implements overall control and interfacing API allowing
   to perform basic audio operations (Init, Play, Pause, Resume, Stop, Volume 
   control and audio file pointer management)
  
In this example the audio file is stored in the internal flash memory (Stereo, 
16-bit, 48 KHz). The analog output device is automatically detected (Speaker or 
Headphone) when the Headphone is plugged/unplugged in/from the audio jack of the 
evaluation board. The example also manages information display and control interface
through push buttons: 
 - When the application is Playing audio file:
     + Key   : Pause
     + Tamper: Volume UP
     + Wakeup: Volume DOWN
 - When the application is Paused:
     + Key   : Play
     + Tamper: Switch output target to Headphone
     + Wakeup: Switch output target to Speaker

This example plays the audio file stored in internal flash memory and replay it
when it reach end of file. But it can be tailored to used different media storage
devices; SDCard (through SDIO), external Memory (through FSMC) ...) or to play 
in streaming mode (ie. from USB port in device or host mode). In this case, Circular
DMA mode should be preferred (by enabling the define AUDIO_MAL_MODE_CIRCULAR in
stm324xg_eval_audio.h file).  

List of Known Limitations and more detailed user notes are provided in file 
stm324xg_eval_audio.c   (under Drivers/BSP/STM324xG_EVAL)

The provided sample audio file, under "/Utilities/Media/Audio/audio_sample_wav.bin"
that must be stored in internal flash memory, is extracted from:
 - Title: artofgardens-instr 
 - Artist/Composer: Dan O'Connor
 - Creative Commons license: Attribution 3.0 United States
   Please read the license following this link:
   http://creativecommons.org/licenses/by/3.0/us/

@note When using the Speaker, if the audio file quality is not high enough, the speaker output
      may produce high and uncomfortable noise level. To avoid this issue, to use speaker
      output properly, try to increase audio file sampling rate (typically higher than 48KHz).
      This operation will lead to larger file size.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, I2S, DMA, Audio codec

@par Directory contents 

  - I2S/I2S_Audio/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - I2S/I2S_Audio/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - I2S/I2S_Audio/Inc/main.h                  Main program header file
  - I2S/I2S_Audio/Src/audio_if.h              Audio Out (playback) interface API header file    
  - I2S/I2S_Audio/Src/stm32f4xx_it.c          Interrupt handlers
  - I2S/I2S_Audio/Src/main.c                  Main program
  - I2S/I2S_Audio/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - I2S/I2S_Audio/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - I2S/I2S_Audio/Src/audio_if.c              Audio Out (playback) interface API file
      
      
@par Hardware and Software environment 

  - This example runs on STM32F407xx/417xx devices.
  
  - This example has been tested with STMicroelectronics STM324xG-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM324xG-EVAL RevC Set-up  
    - Please ensure that the jumper JP16 is in the position 2-3. 


@par How to use it ? 

 - Use STLink utility, available on www.st.com or any other in system programming
   tool to load "/Utilities/Media/Audio/audio_sample_wav.bin" file to the STM32 
   internal flash at the address 0x08080000.
   Make sure the audio file size does not exceed 512KB. When using STLink utility change
   the audio file extension to "*.bin" in order to be able to load it directly to flash memory.
   
In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
         @note Make sure that the tool flash loader does not erase or overwrite the
            loaded audio file at address 0x08080000 by limiting the application
            end address to 0x0807FFFF. This is already done for the example project 
 - Connect a headphone or a speaker to the audio jack connector (CN11).
 

  */
