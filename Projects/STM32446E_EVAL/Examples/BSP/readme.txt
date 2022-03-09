/**
  @page BSP  Example on how to use the BSP drivers
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 180 MHz and provide 50 MHz at the output PLL divided by PLL_Q. 
This frequency permit to reach 25 Mhz clock needed for SD operation and in line 
with microSD specification. 


This example shows how to use the different functionalities of LCD, SD card, 
touchscreen, joystick and SDRAM external memory, audio playback by switching 
between all tests using key button. 

Firstly, use the joystick button to move a pointer inside a rectangle 
(up/down/right/left) and change the pointer color(select).

Secondly, and after the touchscreen calibration, use the touchscreen 
functionality to select or activate colored circle inside a rectangle.

Thirdly, this example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

Fourthly, this example shows how to erase, write and read the SD card and also 
how to detect the presence of the card.

Fifthly, this example shows how to use the LCD log features.

Sixthly, this example provides of how to write, read and buffers compare 
for different SDRAM external memory.

Seventhly, this example shows how to read and write data in RF EEPROM. The I2C EEPROM
memory (M24LR64) is available on separate daughter board ANT7-M24LR-A, which is not
provided with the STM32446E-EVAL board. To use this driver you have to connect the 
ANT7-M24LR-A to CN1 connector of STM32446E-EVAL board.

Eightly, this example shows how to use the QSPI features.

Ninthly, this example shows how to play an audio file through the SAI peripheral
using the external codec WM8994 implemented on the STM32446E-EVAL board. The SAI input 
clock, provided by a dedicated PLL (PLLI2S), is configured initially to have an audio  
sampling frequency at 48 KHz. The audio data is stored in the internal flash memory 
(4 channels, 16-bit, 48 KHz). Following the instruction on the display, stream can be 
paused and resumed, volume can be changed and sample frequency can be changed.
Notice that changing sampling frequency might switch the channels on speakers with
the channels on headset, resulting in a different music. This effect is kept to highlight 
that there are 4 channels and is out of the scope of this example to correct it.

Tenthly, this example shows a camera sensor stream on screen.
It is compatible with only Samsung S5K5CAG camera sensor. It displays streams for two 
resolutions : QQVGA (160x120) and QVGA (320x240). 

At the end of the nine examples when pushing the user button the application loops 
to the beginning (first examples). 

@note In case of camera fail, verify that camera sensor (U866 3M JS1434) connector 
      located at the bottom side of MB1183 RevB Camera module is well connected.
      
@note For a good calibration, touch the extreme TOP LEFT point and the extreme 
      BOTTOM RIGHT point of LCD.
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

BSP, LCD, Touchscreen, PSRAM, DMA, QUADSPI, Erase, Read, Write, Audio play, Record, MEMS, Audio Codec, DFSDM,
I2S, Microphone, Headphones

@par Directory contents 

  - BSP/Src/main.c                     Main program
  - BSP/Src/system_stm32f4xx.c         STM32F4xx system clock configuration file
  - BSP/Src/stm32f4xx_it.c             Interrupt handlers 
  - BSP/Src/joystick.c                 Joystick feature
  - BSP/Src/lcd.c                      LCD drawing features
  - BSP/Src/log.c                      LCD Log firmware functions
  - BSP/Src/sd.c                       SD features
  - BSP/Src/sdram.c                    SDRAM features
  - BSP/Src/eeprom.c                   EEPROM features      
  - BSP/Src/qspi.c                     QSPI features      
  - BSP/Src/audio.c                    Audio features      
  - BSP/Src/camera.c                   Camera features      
  - BSP/Src/touchscreen.c              Touchscreen feature
  - BSP/Src/ts_calibration.c           Touchscreen calibration
  - BSP/Inc/main.h                     Main program header file  
  - BSP/Inc/stm32f4xx_hal_conf.h       HAL configuration file
  - BSP/Inc/stm32f4xx_it.h             Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h             lcd_log configuration template file
  - BSP/Inc/stlogo.h                   Image used for BSP example
        
        
@par Hardware and Software environment  

  - This example runs on STM32F446xx devices.
  
  - This example has been tested with STMicroelectronics STM32446E-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    
  
@par How to use it ? 

 - Use STLink utility, available on www.st.com or any other in system programming
   tool to load "BSP/Binary/audio_sample_tdm.bin" file to the STM32 internal flash 
   at the address 0x08040000.

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
     @note Make sure that the tool flash loader does not erase or overwrite the
        loaded audio file at address 0x08040000 by limiting the application
        end address to 0x0803FFFF. This is already done for the example project
 - Connect a headphone and a speaker to the audio jack connectors (CN22/CN23).


 */
