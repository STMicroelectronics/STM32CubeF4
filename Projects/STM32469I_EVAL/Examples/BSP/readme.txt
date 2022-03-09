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
(SYSCLK) to run at 180 MHz. 

This example shows how to use the different functionalities of LCD, SD card, 
touchscreen, audio playback, joystick and external memories (SDRAM, SRAM, NOR flash Micron, 
Quad SPI flash Micron) as well as external RF EEPROM by switching between all tests using TAMPER button. 

- Joystick related tests : use the joystick button to move a pointer inside a rectangle 
(up/down/right/left) and change the pointer color(select).

- Touchscreen related tests : after the necessary touchscreen calibration, use the touchscreen 
functionality to select or activate colored circle inside a rectangle. Then handle touchscreen in polling
and interrupt mode to detect single and dual tocuhes.

- LCD related tests : those examples shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmaps on DSI LCD in mode
portrait or landscape.

- SD related tests : this example shows how to erase, write and read the SD card and also 
how to detect the presence of the card.

- LCD Log related : this example shows how to use the LCD log features.

- External Memories related : those examples provides of how to write, read and buffers compare 
for different external memories (SDRAM, SRAM, NOR).

- RF EEPROM related : this example shows how to read and write data in RF EEPROM. 
The I2C EEPROM memory (M24LR64) is available on separate daughter board ANT7-M24LR-A, which is not
provided with the STM32469I-EVAL board. To use this driver you have to connect the 
ANT7-M24LR-A to CN3 connector of STM32469I-EVAL board.

- Audio playback related : 
this example shows how to play an audio file through the SAI peripheral
using the external codec WM8994 implemented on the STM324x9I_EVAL board. The SAI input 
clock, provided by a dedicated PLL (PLLI2S), is configured to have an audio sampling 
frequency at 48 KHz. The audio data is stored in the internal flash memory (Stereo, 
16-bit, 48 KHz) and require the use of ST Link utility to load at address 0x80800000 in Flash
the sample file that is located in /Binary folder.


- Camera related : those examples shows a camera sensor stream on screen (preview).
It is compatible with only Samsung S5K5CAG camera sensor. It displays streams for several 
resolutions : QQVGA (160x120), QVGA (320x240), 480x272, VGA (640x480). Brightness and 
contrast are adjustable using the joystick. Several visual effects can be applied using 
joystick selection button.

- QSPI Flash related : detect, read and write in Quad SPI Flash Micron N25Q512A13GSF40E. 

- Animation related : this example shows an animation on DSI LCD using the DMA2D to draw animated
icons.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note Make sure jumper JP3 of STM32469I-EVAL board is on position PC0, because PA4 is shared with the DCMI_HSYNC signal.

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
  - BSP/Src/sdram_dma.c                SDRAM DMA features
  - BSP/Src/sram.c                     SRAM features
  - BSP/Src/nor.c                      NOR Flash features
  - BSP/Src/eeprom.c                   RF EEPROM features      
  - BSP/Src/audio_play.c               Audio features
  - BSP/Src/audio_rec.c                Audio play features  
  - BSP/Src/camera.c                   Camera record features
  - BSP/Src/animation.c                Animation example
  - BSP/Src/touchscreen.c              Touchscreen feature
  - BSP/Src/ts_calibration.c           Touchscreen calibration
  - BSP/Inc/main.h                     Main program header file  
  - BSP/Inc/stm32f4xx_hal_conf.h       HAL configuration file
  - BSP/Inc/stm32f4xx_it.h             Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h             lcd_log configuration template file
  - BSP/Inc/stlogo.h                   Image used for BSP example
  - BSP/Inc/anime.h                    Animated picture routines driving DMA2D
  - BSP/Inc/battery_20x35.h            Battery icon for animation test
  - BSP/Inc/bluetooth_30x35.h          Bluetooth icon for animation test
  - BSP/Inc/clouds1_124x44.h           Cloud icon for animation test
  - BSP/Inc/clouds2_124x44.h           Cloud icon for animation test
  - BSP/Inc/clouds3_124x44.h           Cloud icon for animation test
  - BSP/Inc/drop_29x35.h               Drop icon for animation test
  - BSP/Inc/gauge1_128x126.h           Gauge icon for animation test
  - BSP/Inc/gauge2_128x126.h           Gauge icon for animation test
  - BSP/Inc/gauge3_128x126.h           Gauge icon for animation test
  - BSP/Inc/gyro_36x36.h               Gyro icon for animation test
  - BSP/Inc/icon_camera_60x60.h        Camera icon for animation test
  - BSP/Inc/logo_compo_134x124.h       Composed logo for animation test
  - BSP/Inc/thermo_25x35.h             Thermo icon for animation test
                 
@par Hardware and Software environment  

  - This example runs on STM32F469xx/STM32F479xx devices.
  
  - This example has been tested with STMicroelectronics STM32469I-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    
  - STM32469I-EVAL RevC Setup: For NOR memory use, make sure that the Jumper JP9
    is open and JP5 is put in position 1-2 (NWAIT signal connected to PD6).  

  
@par How to use it ? 

 - Use STLink utility, available on www.st.com or any other in system programming
   tool to load "BSP/Binary/audio_sample_tdm.bin" file to the STM32 internal flash 
   at the address 0x08080000.

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
     @note Make sure that the tool flash loader does not erase or overwrite the
        loaded audio file at address 0x08080000 by limiting the application
        end address to 0x0807FFFF. This is already done for the example project
 - Connect a headphone and a speaker to the audio jack connectors (CN23/CN24).


 */
