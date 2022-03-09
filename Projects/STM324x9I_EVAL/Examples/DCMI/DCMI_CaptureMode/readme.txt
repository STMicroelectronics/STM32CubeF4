/**
  @page DCMI_CaptureMode DCMI Capture Mode example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DCMI/DCMI_CaptureMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DCMI Capture Mode example.
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

This example provides a short description of how to use the DCMI to interface with
camera module and display in continuous mode the picture on LCD.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

The Digital camera interface is configured to receive the capture from
the camera module mounted on STM324x9I-EVAL RevB evaluation board.
DMA2 Stream1 channel1 is configured to transfer the picture from DCMI peripheral
to an external RAM.

At each camera line event, the line is converted to ARGB8888 pixel format 
and transferred to LCD_FRAME_BUFFER using DMA2D.   

The camera module is configured to generate (480x272) image resolution
and the LCD is configured to display (480x272) image resolution

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, DCMI, Camera, Capture, Frame Buffer, LCD, ARGB8888, DMA, RGB565, SDRAM, DMA2D, QQVGA

@par Directory contents

    - DCMI/DCMI_CaptureMode/Inc/stm32f4xx_hal_conf.h    HAL configuration file
    - DCMI/DCMI_CaptureMode/Inc/main.h                  Main program header file
    - DCMI/DCMI_CaptureMode/Inc/stm32f4xx_it.h          Interrupt handlers header file
    - DCMI/DCMI_CaptureMode/Src/main.c                  Main program  
    - DCMI/DCMI_CaptureMode/Src/stm32f4xx_it.c          Interrupt handlers   
    - DCMI/DCMI_CaptureMode/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment 

  - This example runs on STM32F429xx/STM32F439xx devices.
  
  - This example has been tested with STMicroelectronics STM324x9I-EVAL RevB 
    evaluation boards, which includes the AMPIRE480272 LCD and OV2640 camera module, 
    and can be easily tailored to any other supported device and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  

                                    */
