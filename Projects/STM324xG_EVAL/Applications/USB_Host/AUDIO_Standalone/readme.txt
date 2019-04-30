/**
  @page AUDIO_Standalone USB Host AUDIO application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    USB_Host/AUDIO_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Host AUDIO application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim
  
@par Application Description

This application shows how to use the USB host application based on the Audio OUT class on the STM32F4xx devices.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 168 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock, which is generated from an integrated PLL. In the High Speed (HS) mode the
USB clock (60 MHz) is driven by the ULPI.

It's worth noting that the system clock (SYSCLK) can be configured, depending on the used USB Core:
 - SYSCLK is set to 168 MHz: for FS Core because used embedded PHY requires 48 MHz clock,
                             achieved only when system clock is set to 168 MHz.                             
 - SYSCLK is set to 180 MHz: for only HS Core, since no embedded PHY is used.

When the application is started, the connected USB headset device is detected in AUDIO mode and gets 
initialized. The STM32 MCU behaves as a AUDIO Host, it enumerates the device and extracts VID, PID, 
manufacturer name, Serial no and product name information and displays it on the LCD screen. 

A menu is displayed and the user can select any operation from the menu using the Joystick buttons:
 - "Explore audio file" operation searches for all .Wav files in the uSD and displays it on the LCD screen.
 - "Start audio Player" operation starts playing the song from the uSD. The files are read using the 
   FatFs file system then USBH_AUDIO_ChangeOutBuffer() function is called while playing songs and the
   bytes are stored in a circular buffer. Sampling frequency, Channels number and File size (duration) 
   are detected. The audio.c file contains a set of APIs for Audio Out playback, for example when the 
   WAV file is playing, USBH_AUDIO_SetVolume() is called to change the volume setting. This application does not 
   support compressed WAV formats.
 - "Re-Enumerate" operation performs a new Enumeration of the device.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Host library, please refer to UM1720  
"STM32Cube USB Host library".


@par USB Library Configuration

To select the appropriate USB Core to work with, user must add the following macro defines within the
compiler preprocessor (already done in the preconfigured projects provided with this application):
      - "USE_USB_HS" when using USB High Speed (HS) Core
      - "USE_USB_FS" when using USB Full Speed (FS) Core

It is possible to fine tune needed USB Host features by modifying defines values in USBH configuration
file “usbh_conf.h” available under the project includes directory, in a way to fit the application
requirements, such as:
  - Level of debug: USBH_DEBUG_LEVEL
                  0: No debug messages
                  1: Only User messages are shown
                  2: User and Error messages are shown
                  3: All messages and internal debug messages are shown
   By default debug messages are displayed on the debugger IO terminal; to redirect the Library
   messages on the LCD screen, lcd_log.c driver need to be added to the application sources.


@par Directory contents

  - USB_Host/AUDIO_Standalone/Src/main.c                  Main program
  - USB_Host/AUDIO_Standalone/Src/sd_diskio.c      FatFS sd diskio driver implementation
  - USB_Host/AUDIO_Standalone/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - USB_Host/AUDIO_Standalone/Src/stm32f4xx_it.c          Interrupt handlers
  - USB_Host/AUDIO_Standalone/Src/menu.c                  MSC State Machine
  - USB_Host/AUDIO_Standalone/Src/usbh_conf.c             General low level driver configuration
  - USB_Host/AUDIO_Standalone/Src/explorer.c              Explore the uSD content
  - USB_Host/AUDIO_Standalone/Src/audio.c                 Audio Out (playback) interface API
  - USB_Host/AUDIO_Standalone/Inc/main.h                  Main program header file
  - USB_Host/AUDIO_Standalone/Inc/sd_diskio.h      FatFS sd diskio driver header file
  - USB_Host/AUDIO_Standalone/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - USB_Host/AUDIO_Standalone/Inc/lcd_log_conf.h          LCD log configuration file
  - USB_Host/AUDIO_Standalone/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - USB_Host/AUDIO_Standalone/Inc/usbh_conf.h             USB Host driver Configuration file
  - USB_Host/AUDIO_Standalone/Inc/ffconf.h                FAT file system module configuration file
 

@par Hardware and Software environment

  - This application runs on STM32F407xx/STM32F417xx devices.
    
  - This application has been tested with STMicroelectronics STM324xG-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM324xG-EVAL RevC Set-up
    - Insert a microSD card containing .Wav audio file into the STM324xG-EVAL uSD slot (CN6)  
    - Plug the USB headset for sound playback into the STM324xG-EVAL board through 
      'USB micro A-Male to A-Female' cable to the connector:
      - CN9: to use USB High Speed (HS) 
      - CN8: to use USB Full Speed (FS) with embedded PHY(U2)
             Please ensure that jumpers JP16 and JP22 are in position 1-2.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM324xG-EVAL_USBH-HS: to configure the project for STM32F4xx devices using USB OTG HS peripheral
   - STM324xG-EVAL_USBH-FS: to configure the project for STM32F4xx devices using USB OTG FS peripheral
 - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 