/**
  @page I2S_AUDIO  How to use the Audio features with I2S peripheral
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    I2S/I2S_Audio/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2S Audio Example.
  ******************************************************************************
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
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

