/**
  @page DMA2D_MemToMemWithBlending DMA2D Memory to Memory with blending example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics  *******************
  * @file    DMA2D/DMA2D_MemToMemWithBlending/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA2D Memory to Memory with blending example.
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
  Memory_to_Memory with blending transfer mode.

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  Then the SystemClock_Config() function is used to configure the system
  clock (SYSCLK) to run at 180 MHz.
 
  In this transfer mode two input sources are fetched : foreground and background.
  In this example, the foreground and background are configured as following :
   - input memory address at Flash memory 
   - Color mode : RGB565 format
   - The constant alpha for foreground is decreased to see the background.
   - The alpha mode for foreground and background is configured to see two 
     superposed images in the resulting image.
        
  The DMA2D blends the two sources pixels to compute the resulting pixel. 
  The transferred data to the output memory address is the result of the blending 
  operation.
 
  In this example, the LCD layer 1 is configured to display the DMA2D output buffer. 
  (the result of the blending of the foreground and background)      

 @note : 
 The C files of the images used in this example are generated with 
 STemWin bitmap converter released with this package.
 \Middlewares\ST\STemWin\Software\BmpCvtST.exe
 Use the bitmap files under resources repository
       
STM32 Discovery boards LEDs can be used to monitor the transfer status:
 - LED3 is ON when the transfer is complete.
 - LED4 is ON when there is a transfer error.
 - LED3/LED4 are ON when there is an error in transfer/Init process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, Graphic, DMA2D, LCD, SRAM, ARGB4444, Blending, Memory to memory, Foreground,
Background, RGB565, LTDC, Pixel

@par Directory contents

    - DMA2D/DMA2D_MemToMemWithBlending/Inc/main.h                Main configuration file
    - DMA2D/DMA2D_MemToMemWithBlending/Inc/stm32f4xx_it.h        Interrupt handlers header file
    - DMA2D/DMA2D_MemToMemWithBlending/Inc/stm32f4xx_hal_conf.h  HAL configuration file 
    - DMA2D/DMA2D_MemToMemWithBlending/Inc/RGB565_230x170_1.h    Image used for DMAD2D validation
    - DMA2D/DMA2D_MemToMemWithBlending/Inc/RGB565_230x170_2.h    Image used for DMAD2D validation
    - DMA2D/DMA2D_MemToMemWithBlending/Src/main.c                Main program  
    - DMA2D/DMA2D_MemToMemWithBlending/Src/stm32f4xx_it.c        Interrupt handlers
    - DMA2D/DMA2D_MemToMemWithBlending/Src/stm32f4xx_hal_msp.c   HAL MSP module
    - DMA2D/DMA2D_MemToMemWithBlending/Src/system_stm32f4xx.c    STM32F4xx system clock configuration file


@par Hardware and Software environment

  - This example runs on STM32F429xx devices.
    
  - This example has been tested with STMicroelectronics STM32F429I-Discovery RevC
    boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
