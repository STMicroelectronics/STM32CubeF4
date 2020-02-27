/**
  @page Audio_playback_and_record  Audio Playback and Record application

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Audio/Audio_playback_and_record/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Audio Playback and Record application.
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

@par Application Description 

This application shows how to use the different functionalities of Audio device 
and ST MEMS microphones.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals.
Then the SystemClock_Config() function is used to configure the systemclock (SYSCLK) 
to run at 84 MHz.

This application shows how to use the different functionalities of Audio device CS43L22 
and ST MEMS microphone (MP45DT02) by switching between play and record audio
using USER button. 

1) Play audio(Need headphone). Device: "CS43L22"
Application plays an audio file (wave.wav) which is available on USB Mass Storage.
Plug a headphone to hear a 48K sound  /!\ Take care of yours ears.
Default volume is 50%.
Click feature has been implemented only for RevB board. A click on the board
will pause the audio file play (LED6 on). Another click resumes audio
file play (LED6 toggle)

  - The player wave name can be customized through the define "WAVE_NAME" in main.h
  (default name is wave.wav) and if this file is not available on the 
  USB Key the application switch on the record.
 
To play the audio file, a buffer is used between USB and audio codec. This buffer 
is used like a ping-pong buffer to be able to read regularly data from USB to be 
sent to audio codec: 
 - Using FatFs driver, a 1st read of AUDIO_BUFFER_SIZE bytes is done
 - This buffer is sent to audio BSP which uses DMA mode to transfer data from MCU
 to audio codec (16 bits)
 - At DMA half transfer ISR, an indication is sent to application to indicate that
 half of the buffer has been sent
 - At DMA half transfer ISR exit, application can read (AUDIO_BUFFER_SIZE/2) bytes from USB to fill
 the 1st part of the buffer
 - At DMA transfer complete ISR, DMA transfer is requested to send 1st part of the
 buffer
 - At DMA transfer complete ISR exit, application can read (AUDIO_BUFFER_SIZE/2) bytes from USB to fill
 the 2nd part of the buffer
 - When file size is reached, audio codec is stopped and a new play is requested.  

2) Record audio. Device: "MP45DT02"
When the User button is pressed the application starts recording in the USB Key and LED3 will start toggling, 
the recorded wave length is (~30secs) and user can interrupt the recording using the User button. 
 - The recorded wave name can be customized through the define "REC_WAVE_NAME" in main.h.
 - Once the recording is finished or interrupted, the recorded wave will start playing
   automatically and LED6 start toggling, Plug a headphone to hear a 16K sound  /!\ Take care of yours ears.
 - If the define "PLAY_REPEAT_DISABLED" in main.h file is not enabled the play will start in infinity loop,
   on board RevB you can pause/resume the player using the MEMS click feature (LED6 on/Toggle) 
 - Default volume is 70%.

Two MEMS microphone MP34DT01 mounted on STM32F411E-Discovery is connected
to the Inter-IC Sound (I2S) peripheral. The I2S is configured in master
receiver mode. In this mode, the I2S peripheral provides the clock to the MEMS microphone.
Data acquired (Audio samples) from the MEMS microphone through DOUT.

Data acquisition is performed in 16KHz and 16-bit PDM format and DMA is used to
transfer data from I2S peripheral to internal SRAM. In order to avoid data-loss, one buffer
split into two halves is used:

When the first half of the buffer is filled (half transfer complete), the PDM
samples are converted to PCM format while the second half of the buffer is being
used to receive data from Audio samples.

When the second half of the buffer is filled (transfer complete), the PDM
samples are converted to PCM format while the first half of the buffer is being
used to receive data from Audio samples. This process is repeated as long as the 
application is running.

To avoid data corruption:
  - DMA ISR priority must be set at a Lower priority than USB interrupt sources,
  this priority order must be respected when managing other interrupts;
  - The processing time of converting/filtering samples from PDM to PCM 
  PDM_Filter_64_LSB()) should be lower than the time required to fill a
  single buffer.

In main loop of the application, when a buffer is full with PCM format it is written
in USB key using the free file system FatFs.

Note that a PDM Audio software decoding library provided in binary is used in 
this application. For IAR EWARM toolchain, the library is labeled 
"libPDMFilter_IAR.a".

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
to have correct HAL operation.

@par Keywords

Audio, Playback, Record, MEMS, Microphone, Touch screen, USB, WAV, FatFS, SRAM, audio codec,
DMA, SAI

@par Directory contents 

  - Audio_playback_and_record/Src/main.c               Main program
  - Audio_playback_and_record/Src/usbh_diskio_dma.c      FatFS usbh diskio driver implementation
  - Audio_playback_and_record/Src/stm32f4xx_it.c       Interrupt handlers
  - Audio_playback_and_record/Src/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - Audio_playback_and_record/Src/usbh_conf.c          Board support package for the USB host library 	
  - Audio_playback_and_record/Src/waveplayer.c         Audio play file
  - Audio_playback_and_record/Src/waverecorder.c       Audio record file
  - Audio_playback_and_record/Inc/ffconf.h             FAT file system module configuration file   
  - Audio_playback_and_record/Inc/main.h               Main program header file
  - Audio_playback_and_record/Inc/usbh_diskio_dma.h      FatFS usbh diskio driver header file
  - Audio_playback_and_record/Inc/stm32f4xx_hal_conf.h HAL configuration file
  - Audio_playback_and_record/Inc/stm32f4xx_it.h       Interrupt handlers header file
  - Audio_playback_and_record/Inc/usbh_conf.h          USB driver Configuration file
  - Audio_playback_and_record/Inc/waveplayer.h         Audio play header file
  - Audio_playback_and_record/Inc/waverecorder.h       Audio record header file
 
      
@par Hardware and Software environment  

  - This application runs on and STM32F411xE devices.
    
  - This application has been tested with STMicroelectronics STM32F411E-Discovery board 
    and can be easily tailored to any other supported device and development board.

  - STM32F411E-DISCO Set-up
    - Plug the USB key into the STM32F411E-DISCO board through 'USB micro A-Male 
      to A-Female' cable (FS mode: connector CN5).
    - audio_sample.wav provided under "Utilities/Media/Audio" must be saved 
      on the root of USB mass storage


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
