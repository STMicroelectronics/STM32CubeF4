/**
  @page MSC_Standalone USB Device Mass Storage (MSC) application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    USB_Device/MSC_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Device MSC application.
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

This application shows how to use the USB device application based on the Mass Storage Class (MSC) on the STM32F4xx devices.

This is a typical application on how to use the STM32F413xx USB OTG Device peripheral to communicate with a PC
Host using the Bulk Only Transfer (BOT) and Small Computer System Interface (SCSI) transparent commands, 
while the microSD card is used as storage media. The STM32 MCU is enumerated as a MSC device using the 
native PC Host MSC driver to which the STM32F413H-Discovery board is connected.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 100 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from the PLL I2S.

The 48 MHz clock for the USB FS can be derived from one of the two following sources:
  – PLL clock (clocked by the HSE): If the USB uses the PLL as clock source, the PLL clock must be programmed
    to output 48 MHz frequency (USBCLK = PLLVCO/PLLQ).
  – PLLI2S clock (clocked by the HSE): If the USB uses the PLLI2S as clock source, the PLLI2S clock must be programmed
    to output 48 MHz frequency (USBCLK = PLLI2SVCO/PLLI2SQ).
 
When the application is started, the user has just to plug the USB cable into a PC host and the device 
is automatically detected. A new removable drive appears in the system window and write/read/format 
operations can be performed as with any other removable drive.
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
For more details about the STM32Cube USB Device library, please refer to UM1734 
"STM32Cube USB Device library".
 
@par Keywords

Connectivity, USB_Device, USB, MSC, Mass Storage, Full Speed, High Speed, BOT, SCSI, Removable drive, FatFs,
File system, Write, Read, Format
     
@par Directory contents

  - USB_Device/MSC_Standalone/Src/main.c                  Main program
  - USB_Device/MSC_Standalone/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - USB_Device/MSC_Standalone/Src/stm32f4xx_it.c          Interrupt handlers
  - USB_Device/MSC_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/MSC_Standalone/Src/usbd_desc.c             USB device MSC descriptor
  - USB_Device/MSC_Standalone/Src/ubsd_storage.c          Media Interface Layer
  - USB_Device/MSC_Standalone/Inc/main.h                  Main program header file
  - USB_Device/MSC_Standalone/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - USB_Device/MSC_Standalone/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - USB_Device/MSC_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/MSC_Standalone/Inc/usbd_desc.h             USB device MSC descriptor header file  


@par Hardware and Software environment

  - This application runs on STM32F413xx devices.
    
  - This application has been tested with STMicroelectronics STM32F413H-Discovery boards 
    and can be easily tailored to any other supported device and development board.

  - STM32F413H-Discovery Set-up
    - Insert a microSD card into the STM32F413H-Discovery uSD slot 
    - Connect the STM32F413H-Discovery board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
  

   */
