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
a camera module to capture and display in continuous mode the picture on LCD.

This example captures continuously camera images in a Camera Frame Buffer 
(320x240 in RGB565) into external SDRAM. 
Each time a full frame camera image is captured display it on the LCD DSI in ARGB8888 format by a partial refresh in DSI adapted
command mode with Tearing Effect by DSI Link in RGB888 format inside DSI packets.
This use example implements a camera preview mode.

There are two stages : identified by the stageNb variable
- StageNb == 1 : refresh the title and black screen of the LCD in 800x480 landscape, this is the full LCD frame
buffer refresh in adapted command mode. 
- StageNb == 2 : to display in streaming the camera frame buffer, the LTDC is reprogrammed to partial refresh
the LCD in the center of the WVGA glass on an area QVGA (320x240) to only refresh the camera image preview and
reduce the bandwidth consumed and power consumption on DSI link.
In stageNb==2, the camera capture is started in a continuous mode in Camera Frame buffer in SDRAM 
to receive a 320x240 RGB565 image at speed of camera module. 
Each time a new DCMI Frame Event is received,
a new camera buffer is ready for display. It is copied/color converted from RGB565 to ARGB8888 in the center of
the LCD frame buffer. The LTDC is programmed at entry in stageNb == 2 to refresh only the QVGA center of the LCD 
800x480 frame buffer. Also a DSI command is sent to LCD to notify to partial refresh only the
area QVGA in the center of the screen.
Once DMA2D have finished copy, the LCD frame buffer center is ready for partial refresh by calling LCD_Refresh()
function which does refresh of GRAM in LCD synchronized with tearing effect DSI packet sent by LCD on request.

The Digital camera interface is configured to receive the capture from
the camera module mounted on STM32469I-EVAL evaluation board.
A DMA Peripheral to Memory is programmed between DCMI and Camera Frame buffer 
to receive a 320x240 RGB565 in camera frame buffer 
in SDRAM or SRAM in continuous mode.


The camera module is configured to generate QVGA (320x240) image resolution in RGB565
and the LCD is configured to display QVGA image resolution in ARGB8888.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

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

Display, DCMI, Camera, Capture, Frame Buffer, LCD, ARGB8888, DMA, RGB565, SDRAM, DMA2D, QQVGA

@par Directory contents 

  - DCMI/DCMI_CaptureMode/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - DCMI/DCMI_CaptureMode/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - DCMI/DCMI_CaptureMode/Inc/main.h                  Header for main.c module  
  - DCMI/DCMI_CaptureMode/Src/stm32f4xx_it.c          Interrupt handlers
  - DCMI/DCMI_CaptureMode/Src/main.c                  Main program
  - DCMI/DCMI_CaptureMode/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F469xx/STM32F479xx devices.
    
  - This example has been tested and validated with STM32469I-EVALRevC  board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 



 */
