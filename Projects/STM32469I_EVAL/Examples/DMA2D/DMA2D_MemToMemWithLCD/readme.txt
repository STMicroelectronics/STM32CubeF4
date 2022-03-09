/**
  @page DMA2D_MemToMemWithLCD DMA2D Memory to Memory with LCD example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DMA2D/DMA2D_MemToMemWithLCD/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA2D Memory to Memory with LCD example.
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

  This example provides a description of how to configure DMA2D peripheral in 
  Memory_to_Memory transfer mode and display the result on LCD.
  
  The source buffer in format ARGB4444 and size 150x150 located in Flash is copied in Memory to Memory mode 
  by the DMA2D in SRAM Destination buffer of same size and format.
  This SRAM buffer is then displayed by the LTDC centered on LCD screen.

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  Then the SystemClock_Config() function is used to configure the system
  clock (SYSCLK) to run at 180 MHz.
 
  In this basic example the goal is to explain the different fields of the DMA2D 
  structure in the case of Memory_to_Memory transfer mode.

  After DMA2D configuration, the data transfer Memory to Memory is performed from Flash memory to
  SRAM. Then the LTDC is configured to display obtained DMA2D output buffer in SRAM on LCD. 
  (more details about LCD configuration in LCD examples)
  
 => Output offset is the number of pixel to be skipped after each transferred line
                      
 how to calculate the size of the transferred data ? 
 => selected color mode gives the number of bits per pixel and we have 
    (ARGB4444 => 16bits/pixel) the number of pixel per line and the number of line, 
    therefore :
    
    data_size = (bits per pixel) X (pixel per line + output offset) X (number of line) 
       
STM32469I-EVAL boards LEDs can be used to monitor the transfer status:
 - LED1 is ON when the DMA2D transfer is complete.
 - LED2 is ON when there is a DMA2D transfer error.
 - LED3 is ON when there is an error in LTDC transfer/Init process.
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, Graphic, DMA2D, LCD, SRAM, ARGB4444, Blending, Memory to memory, LTDC, Pixel

@par Directory contents

    - DMA2D/DMA2D_MemToMemWithLCD/Inc/main.h                Main configuration file
    - DMA2D/DMA2D_MemToMemWithLCD/Inc/stm32f4xx_it.h        Interrupt handlers header file
    - DMA2D/DMA2D_MemToMemWithLCD/Inc/stm32f4xx_hal_conf.h  HAL configuration file
    - DMA2D/DMA2D_MemToMemWithLCD/Inc/ARGB4444_150x150.h    Texture in Flash to be transferred by DMA2D in SRAM
    - DMA2D/DMA2D_MemToMemWithLCD/Src/main.c                Main program  
    - DMA2D/DMA2D_MemToMemWithLCD/Src/stm32f4xx_it.c        Interrupt handlers
    - DMA2D/DMA2D_MemToMemWithLCD/Src/stm32f4xx_hal_msp.c   HAL MSP module    
    - DMA2D/DMA2D_MemToMemWithLCD/Src/system_stm32f4xx.c    STM32F4xx system clock configuration file

@par Hardware and Software environment  

  - This example runs on STM32F469xx/STM32F479xx devices.

  - This example has been tested and validated with STMicroelectronics STM32469I-EVAL RevC
    evaluation board and can be easily tailored to any other supported device 
    and development board. 


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 
  

                                    */
