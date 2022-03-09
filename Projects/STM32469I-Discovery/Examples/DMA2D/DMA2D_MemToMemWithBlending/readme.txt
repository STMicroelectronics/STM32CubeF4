/**
  @page DMA2D_MemToMemWithBlending DMA2D Memory to Memory with blending example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DMA2D/DMA2D_MemToMemWithBlending/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32F4xx DMA2D Memory to Memory with blending example.
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
   - Foreground object 
   - input memory address at Flash memory (static image arrays coded in FLASH).
   - Format : RGB565
   - Size   : 240x130
   - Background object 
   - input memory address at Flash memory (static image arrays coded in FLASH).
   - Format : RGB565
   - Size   : 240x130
   - The constant alpha for foreground is decreased to see the background.
   - The alpha mode for foreground and background is configured to see two 
     superposed images in the resulting image out of the ChromArt DMA2D operation.
        
  The DMA2D blends the two sources pixels from FLASH to internal FIFOs to compute 
  the resulting pixel in dedicated output FIFO. 
  The transferred data to the output memory address is the result of the blending 
  operation between background and foreground.
 
  Next step in the pixel pipeline is the use of the LTDC with only 1 layer which 
  is the output of the DMA2D operation.
  This in order to show on LCD panel the result of the blending of the 
  foreground and background.      
  
  In this example, the background object represents the ST logo and the foreground
  object represents the "life.augmented" slogan.

STM32469I-DISCOVERY boards LEDs can be used to monitor the transfer status:
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

  - This example runs on STM32F469xx devices.
  
  - This example has been tested with STM32469I-DISCOVERY boards and can be easily 
    tailored to any other supported device and development board. 

  - This example is configured to run by default on STM32469I-DISCO RevC board.
  - In order to run this example on RevA or RevB boards, replace the flag 
    USE_STM32469I_DISCO_REVC, which is defined in the pre-processor options, by 
    either USE_STM32469I_DISCO_REVA or USE_STM32469I_DISCO_REVB respectively.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 
                          

 */
