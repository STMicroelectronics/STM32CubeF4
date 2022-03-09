/**
  @page DCMI_CaptureMode DCMI Capture Mode example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DCMI/DCMI_CaptureMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32F4xx DCMI_CaptureMode example.
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
a camera module and display in continuous mode the picture on the LCD.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

The Digital camera interface is configured to receive the capture from
the camera module mounted on STM32446E-EVAL evaluation board.
The DMA is configured to transfer the picture from DCMI peripheral
to an external RAM used by the LCD as a frame buffer.   
When the frame event callback is raised the picture is transferred to the LCD frame buffer.   

The camera module is configured to generate QVGA (320x240) image resolution
and the LCD is configured to display QVGA image resolution

@note In case of camera fail, verify that camera sensor (U866 3M JS1434) connector 
      located at the bottom side of MB1183 RevB Camera module is well connected.
 
      
      
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, DCMI, Camera, Capture, Frame Buffer, LCD, ARGB8888, DMA, RGB565, SDRAM, DMA2D, QQVGA

@par Directory contents 

  - DCMI/DCMI_CaptureMode/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - DCMI/DCMI_CaptureMode/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - DCMI/DCMI_CaptureMode/Inc/main.h                  Header for main.c module  
  - DCMI/DCMI_CaptureMode/Src/stm32f4xx_it.c          Interrupt handlers
  - DCMI/DCMI_CaptureMode/Src/main.c                  Main program
  - DCMI/DCMI_CaptureMode/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with STM32446E-EVAL board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 



 */
