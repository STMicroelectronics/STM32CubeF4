/**
  @page DCMI_SnapshotMode DCMI Capture Mode example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DCMI/DCMI_SnapshotMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32F4xx DCMI_SnapshotMode example.
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
a camera module and display in snapshot mode the picture on LCD.

This example captures a single image in Camera Frame Buffer (320x240 in RGB565) 
and once full frame camera is captured display it on the LCD in ARGB8888 format.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

The Digital camera interface (DCMI) is configured to receive the capture from
the camera module mounted on STM32469I-EVAL evaluation board.
A DMA Peripheral to Memory is programmed between DCMI and Camera Frame buffer to receive a 320x240 RGB565
in camera frame buffer in SDRAM or SRAM.
When the IT DCMI End of frame occurs, the DMA2D is used to copy and convert the RGB565 frame from Camera into an ARGB8888 LCD Frame buffer
and therefore the obtained buffer is displayed on LCD.

The camera module is configured to generate QVGA (320x240) image resolution
and the LCD is configured to display QVGA image resolution

LED1 is ON to indicate the test is OK. 
LED3 is ON in case of initialization error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note Make sure jumper JP3 of STM32469I-EVAL board is on position PC0, because PA4 is shared with the DCMI_HSYNC signal.

@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

Display, DCMI, Camera, Snapshot, Frame Buffer, LCD, ARGB8888, DMA, RGB565, SDRAM, DMA2D, QQVGA

@par Directory contents 

  - DCMI/DCMI_SnapshotMode/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - DCMI/DCMI_SnapshotMode/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - DCMI/DCMI_SnapshotMode/Inc/main.h                  Header for main.c module  
  - DCMI/DCMI_SnapshotMode/Src/stm32f4xx_it.c          Interrupt handlers
  - DCMI/DCMI_SnapshotMode/Src/main.c                  Main program
  - DCMI/DCMI_SnapshotMode/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F469xx/STM32F479xx devices.
    
  - This example has been tested and validated with STM32469I-EVAL RevC board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 



 */
