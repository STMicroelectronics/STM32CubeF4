/**
  @page DFSDM_AudioRecord  Description of the DFSDM audio record example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DFSDM/DFSDM_AudioRecord/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DFSDM audio record example.
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

This example shows how to use the DFSDM HAL API to perform stereo audio recording.
For this audio record we will use two MP34DT01 digital microphones mounted on the board. 

MP34DT01 runs with a clock frequency from 1MHz to 3,25MHz.
This clock frequency has to be delivered by DFSDM. So first channel of DFSDM will be
configurated to enable output clock generation, to choose audio clock as output clock source,
and output clock divider will be set to generate output clock on MP34DT01 frequency clock range.
Audio clock will be configurated to 49.142MHz, so DFSDM output clock divider will be set to 24.

The digital audio outputs from the microphones are coded in PDM (Pulse Density Modulation) and 
interlaced to use the same data wire connected to PB1. When CLK = 0, the left channel is sent
on PCB1 and when CLK = 1, the right channel is sent on PC0.

PC0 can be configurated as DFSDM_DATIN0 (DFSDM channel 0 external data input).
We will use DFSDM channel 0 to get left channel. DFSDM channel 0 will be configurated to take
inputs from external serial inputs and from pins of channel 0. DFSDM channel 0 will also be 
configurated to operate in SPI mode with rising edge and with internal clock.
We will use also DFSDM channel 3 to get right channel. DFSDM channel 3 will be configurated to take
inputs from external serial inputs and from pins of channel 0. DFSDM channel 3 will also be 
configurated to operate in SPI mode with falling edge and with internal clock.

DFSDM filter 0 will be used to perform left channels conversions.
We will use a SINC3 filter with oversampling at 128 and integrator oversampling at 1.
Oversampling is set to 128 in order to have audio sample frequency at 16KHz (49.142MHz/(24*128)).
Regular conversions in fast mode will be performed in DMA mode on DFSDM filter 0 in order to fill
a buffer containing left audio samples at 16KHz.
DFSDM filter 1 will be used to perform right channels conversions.
We will use a SINC3 filter with oversampling at 128 and integrator oversampling at 1.
Oversampling is set to 128 in order to have audio sample frequency at 16KHz (49.142MHz/(24*128)).
Regular conversions in fast mode and synchronized with DFSDM filter 0 regular conversions will be 
performed in DMA mode on DFSDM filter 1 in order to fill a buffer containing right audio samples at 16KHz.

Playback of the recorded data will be performed on headset using HAL_I2S and WM8994 audio codec.
A circular playback buffer will be filled as soon as recorded data are available. 
When half left and right buffers will be filled, we put first parts of left and right channels data 
on first half of playback buffer. 
When left and right buffers will be full filled, we put second parts of left and right channels data 
on second half of playback buffer.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Audio, DFSDM, Audio record, Digital Microphone, Stereo, MP34DT01, DMA, PDM, Filter, Playback, Audio codec, SAI

@par Directory contents  

  - DFSDM/DFSDM_AudioRecord/Src/main.c                  Main program
  - DFSDM/DFSDM_AudioRecord/Src/system_stm32f4xx.c      STM32F4xx system source file
  - DFSDM/DFSDM_AudioRecord/Src/stm32f4xx_it.c          Interrupt handlers
  - DFSDM/DFSDM_AudioRecord/Inc/main.h                  Main program header file
  - DFSDM/DFSDM_AudioRecord/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - DFSDM/DFSDM_AudioRecord/Inc/stm32f4xx_it.h          Interrupt handlers header file


@par Hardware and Software environment

  - This example runs on STM32412xx devices.
    
  - This example has been tested with STMicroelectronics STM32412G-Discovery
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32F412G-Discovery Set-up :
    - Plug headset on AUDIO JACK connector.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 


 */
