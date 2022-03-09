/**
  @page BSP Example on how to use the BSP drivers
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Demonstration/readme.txt 
  * @author  MCD Application Team
  * @brief   Description STM32F4xx_Nucleo_144 Demo description.
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

This example provides a description of how to use the different BSP drivers.

The BSP example detects the presence of Adafruit 1.8" TFT shield with joystick and uSD.
If the Adafruit shield is NOT connected, then:
 - Blue LED (LED2) blinks waiting the user button is pushed.
 - Then blue led switches off and green & red LEDs start blinking at high frequency.
 - Pushing user button again and again blinking frequency decreases until looping 
   to high frequency.

If the Adafruit shield is connected, then : this example shows how to use the different 
functionalities of LCD, SD card and joystick by switching between tests using key button. 
  - The Blue LED (LED2) toggles until the user button is pressed.
  - Firstly, use the joystick button to move a pointer inside a rectangle 
    (up/down/right/left) and change the pointer color(select).
  - Secondly, this example shows how to use the different LCD features to display 
    string with different fonts, to display different shapes and to draw a bitmap.
  - Thirdly, this example shows how to erase, write and read the SD card.
At the end of the nine examples when pushing the user button the application loops 
to the beginning (first examples). pushing the user button the application loops 
to the beginning (first examples).

@par Keywords

Board Support Package, BSP, Adafruit, TFT, LCD, Joystick, SD Card

@par Directory contents 

  - BSP/Src/main.c                     Main program
  - BSP/Src/system_stm32f4xx.c         STM32F4xx system clock configuration file
  - BSP/Src/stm32f4xx_it.c             Interrupt handlers 
  - BSP/Src/joystick.c                 Joystick feature
  - BSP/Src/lcd.c                      LCD drawing features
  - BSP/Src/log.c                      LCD Log firmware functions
  - BSP/Src/sd.c                       SD features
  - BSP/Inc/main.h                     Main program header file  
  - BSP/Inc/stm32f4xx_hal_conf.h       HAL configuration file
  - BSP/Inc/stm32f4xx_it.h             Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h             lcd_log configuration template file
  - BSP/Inc/stlogo.h                   Image used for BSP example


@par Hardware and Software environment

 - This Demo runs on STM32F4xx Devices 
  
 - This example has been tested with STMicroelectronics NUCLEO-F446ZE Rev.B (MB1137) 
   and can be easily tailored to any other supported device and development board.

 - NUCLEO-F446ZE Set-up
   - Connect the Adafruit 1.8" TFT shield (https://www.adafruit.com/products/802)
   - Make sure the SB146, SB121 are closed and the SB174, SB165, SB122 are opened on the NUCLEO-F446ZE board (bottom side)
   
@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
      

 */
