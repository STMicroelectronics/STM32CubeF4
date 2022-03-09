/**
  @page Audio_playback_and_record  Audio Playback and Record application

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Audio/Audio_playback_and_record/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Audio Playback and Record application
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

@par Application Description 

This application shows how to use the different functionalities of Audio device 
and ST MEMS microphones.

Three different menu are available to switch between them use the joystick button:
  + Explorer Audio File menu
  + Start Audio Player menu
  + Start Audio Recorder menu

1) Explorer Audio File menu.
In this menu, the explorer wave files list stored in the USB Key will be displayed on the LCD.

2) Start Audio Play menu (Need headphone).
In Start Audio Play menu, any wave file stored under the USB Key can be opened using the FatFs 
file system and transferred to the internal SRAM using the file system. All the wave
files properties are read from the Wave File Header.
Plug a headphone to hear the sound  /!\ Take care of yours ears. Default volume is 70%.

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

This menu also manages information display and control interface through push buttons: 
 - When the application is Playing audio file:
     [UP]:   Volume+
     [DOWN]: Volume-
     [LEFT]: Previous wave file
     [RIGHT]: Next wave file
     [SEL]: Pause/Resume
     [User Key]: Stop

Note: The audio files provided under "/Utilities/Media/Audio" are based on a free 
music download from www.DanoSongs.com website and user can load his own audio 
(*.wav) files in the USB Key root.
 
3) Start Audio Recorder menu:
Two MEMS microphone MP34DT01 mounted on STM324x9I-EVAL are connected
to the Inter-IC Sound (I2S) peripheral. The I2S is configured in master
receiver mode. In this mode, the I2S peripheral provides the clock to an embedded
timer (TIM3 in this application) which divides the I2S clock. This clock is delivered
to the MEMS microphone through CLK. Data acquired (Audio samples) from the MEMS 
microphone through DOUT.

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
in USB key using the free file system FatFS.

This menu also manages information display and control interface through push buttons: 
 - When the application is recording audio file:
     [UP]: Volume+
     [DOWN]: Volume-
     [SEL]: Pause/Resume
     [User Key]: Stop

Note that a PDM Audio software decoding library provided in binary is used in 
this application. For IAR EWARM toolchain, the library is labeled 
"libPDMFilter_IAR.a".

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Audio_PDM, Playback, Record, MEMS, Microphone, Touch screen, USB, WAV, FatFS, SRAM, audio codec,
DMA, SAI

@par Directory contents 

  - Audio_playback_and_record/Src/main.c               Main program
  - Audio_playback_and_record/Src/usbh_diskio_dma.c    FatFS usbh diskio driver implementation
  - Audio_playback_and_record/Src/stm32f4xx_it.c       Interrupt handlers
  - Audio_playback_and_record/Src/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - Audio_playback_and_record/Src/usbh_conf.c          Board support package for the USB host library 	
  - Audio_playback_and_record/Src/waveplayer.c         Audio play file
  - Audio_playback_and_record/Src/waverecorder.c       Audio record file
  - Audio_playback_and_record/Inc/ffconf.h             FAT file system module configuration file   
  - Audio_playback_and_record/Inc/main.h               Main program header file
  - Audio_playback_and_record/Inc/usbh_diskio_dma.h    FatFS usbh diskio driver header file
  - Audio_playback_and_record/Inc/stm32f4xx_hal_conf.h HAL configuration file
  - Audio_playback_and_record/Inc/stm32f4xx_it.h       Interrupt handlers header file
  - Audio_playback_and_record/Inc/usbh_conf.h          USB driver Configuration file
  - Audio_playback_and_record/Inc/waveplayer.h         Audio play header file
  - Audio_playback_and_record/Inc/waverecorder.h       Audio record header file
      
      
@par Hardware and Software environment

  - This application runs on STM32F42xxx/STM32F43xxx devices.
    
  - This application has been tested with STM324x9I-EVAL RevB evaluation board and can be
    easily tailored to any other supported device and development board.   

  - STM324x9I-EVAL Set-up
    - Please ensure that jumpers JP4 and JP5 are fitted in position 2-3
    - Please ensure that jumpers JP13, JP14 and JP15 are fitted in position 2-3
    - Plug the USB key into the STM324x9I-EVAL board through 'USB micro A-Male 
      to A-Female' cable (FS mode: connector CN14).
    - Load .wav audio file (audio_sample.wav) available under "/Utilities/Media/Audio" to the root of USB mass storage


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 

 */
