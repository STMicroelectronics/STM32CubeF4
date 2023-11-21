/**
  @page MSC_RTOS USB Host Mass Storage (MSC) RTOS application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    USB_Host/MSC_RTOS/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Host MSC RTOS application.
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

@par Application Description

This application shows how to use the USB host application based on the Mass Storage Class (MSC) on the STM32F4xx devices in RTOS mode
configuration.

This is a typical application on how to use the STM32F412xG USB OTG Host peripheral to operate with an USB 
flash disk using the Bulk Only Transfer (BOT) and Small Computer System Interface (SCSI) transparent
commands combined with a file system FatFs (Middleware component).

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the HAL time base. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 100 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from a specific output of two PLLs PLL or PLL SAI.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

The 48 MHz clock for the USB FS can be derived from one of the two following sources:
  – PLL clock (clocked by the HSE): If the USB uses the PLL as clock source, the PLL clock must be programmed
    to output 48 MHz frequency (USBCLK = PLLVCO/PLLQ).
  – PLLI2S clock (clocked by the HSE): If the USB uses the PLLI2S as clock source, the PLLI2S clock must be programmed
    to output 48 MHz frequency (USBCLK = PLLI2SVCO/PLLI2SQ).

When the application is started, the connected USB flash disk device is detected in MSC mode and gets 
initialized. The STM32 MCU behaves as a MSC Host, it enumerates the device and extracts VID, PID, 
manufacturer name, Serial no and product name information and displays it on the LCD screen. 
This application is based on read/write file and explore the USB flash disk content. It creates a normal
priority thread, that executes a MSC routine.
LCD and joystick are provided by Adafruit 1.8" TFT shield to be plugged on Arduino connectors.

User can start any operations using User button:
 - "File Operations" operation writes a small text file (less to 1 KB) on the USB flash disk.
 - "Explorer Disk" operation explores the USB flash disk content and displays it on the LCD screen.
    User has to press the User button to display the whole USB flash disk content (recursion level 2).

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Host library, please refer to UM1720  
"STM32Cube USB Host library".

@note LED1 is turned ON, because it's sharing the same pin with the SPI CLK signal used to communicate with the Adafruit 1.8" TFT shield.

Since this is an RTOS based application, built around FreeRTOS and CMSIS-OS wrapping layer common APIs,
the RTOS is enabled by setting, #define USBH_USE_OS      1

@par USB Library Configuration

It is possible to fine tune needed USB Host features by modifying defines values in USBH configuration
file "usbh_conf.h" available under the project includes directory, in a way to fit the application
requirements, such as:
  - Level of debug: USBH_DEBUG_LEVEL
                  0: No debug messages
                  1: Only User messages are shown
                  2: User and Error messages are shown
                  3: All messages and internal debug messages are shown
   By default debug messages are displayed on the debugger IO terminal; to redirect the Library
   messages on the LCD screen, lcd_log.c driver need to be added to the application sources.

@par Keywords

Connectivity, USB_Host, FreeRTOS, MSC, Mass Storage, Full Speed, High Speed, BOT, SCSI, Removable drive, FatFs,
File system, Write, Read, Format

@par Directory contents

  - USB_Host/MSC_RTOS/Src/main.c                  Main program
  - USB_Host/MSC_RTOS/Src/usbh_diskio_dma.c      FatFS usbh diskio driver implementation
  - USB_Host/MSC_RTOS/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - USB_Host/MSC_RTOS/Src/stm32f4xx_it.c          Interrupt handlers
  - USB_Host/MSC_RTOS/Src/menu.c                  MSC State Machine
  - USB_Host/MSC_RTOS/Src/usbh_conf.c             General low level driver configuration
  - USB_Host/MSC_RTOS/Src/explorer.c              Explore the USB flash disk content
  - USB_Host/MSC_RTOS/Src/file_operations.c       Write/read file on the disk
  - USB_Host/MSC_RTOS/Src/usbh_diskio.c           USB diskio interface for FatFs
  - USB_Host/MSC_RTOS/Inc/main.h                  Main program header file
  - USB_Host/MSC_RTOS/Inc/usbh_diskio_dma.h      FatFS usbh diskio driver header file
  - USB_Host/MSC_RTOS/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - USB_Host/MSC_RTOS/Inc/lcd_log_conf.h          LCD log configuration file
  - USB_Host/MSC_RTOS/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - USB_Host/MSC_RTOS/Inc/usbh_conf.h             USB Host driver Configuration file
  - USB_Host/MSC_RTOS/Inc/ffconf.h                FAT file system module configuration file
 

@par Hardware and Software environment

  - This application runs on STM32F412Zx devices.
    
  - This application has been tested with STMicroelectronics NUCLEO-F412ZG 
    boards and can be easily tailored to any other supported device and
    development board.

  - NUCLEO-F412ZG Set-up
    - Plug the USB key into the NUCLEO-F412ZG board through 'USB micro A-Male 
      to A-Female' cable to the connector:

  - Make sure that the jumper JP4 is closed

  - Adafruit 1.8" TFT shield must be connected on CN7, CN8, CN9 and CN10 Arduino connectors, 
    for more details please refer to UM1726.

  - Make sure the SB72 is closed and the SB57 and SB80 are opened on the NUCLEO-F412ZG board (bottom side)

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 

 */
