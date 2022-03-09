/**
  @page DMA2D_MemoryToMemory DMA2D : Memory to Memory transfer example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/DMA2D/DMA2D_MemoryToMemory/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA2D_MemoryToMemory example.
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

This example describes how to configure the DMA2D peripheral in Memory-to-Memory transfer
mode. The example is based on the STM32F4xx DMA2D LL API. Peripheral initialization is done 
using LL unitary services functions for optimization purpose (performance and size).

The example achieves a DMA2D transfer of an internal SRAM static texture array aBufferInput[] 
of size 16x32 and format ARGB4444 (16 bpp) to a destination buffer in internal SRAM in 
array aBufferResult[].
The number of bytes transferred is then 16x32x2=1024 bytes.

Peripheral initialization done using LL unitary services functions
for optimization purpose (performance and size).

After DMA2D configuration, the data transfer is performed in polling mode and then the transferred 
data are checked in comparing the source and destination buffers aBufferInput[] and aBufferResult[].

STM32F429ZI-Nucleo Rev B board LED1 is used to monitor the transfer status:
- LED1 is ON in case of successful transfer,
- LED1 is blinking in case of transfer error.

@par Keywords

Display, Graphic, DMA2D, SRAM, ARGB4444, Memory to Memory

@par Directory contents 

  - DMA2D/DMA2D_MemoryToMemory/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - DMA2D/DMA2D_MemoryToMemory/Inc/main.h                  Header for main.c module
  - DMA2D/DMA2D_MemoryToMemory/Inc/stm32_assert.h          Template file to include assert_failed function
  - DMA2D/DMA2D_MemoryToMemory/Src/stm32f4xx_it.c          Interrupt handlers
  - DMA2D/DMA2D_MemoryToMemory/Src/main.c                  Main program
  - DMA2D/DMA2D_MemoryToMemory/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F429xx devices.

  - This example has been tested with STM32F429ZI-Nucleo Rev B board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
