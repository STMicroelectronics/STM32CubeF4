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

This is a typical application on how to use the STM32F446xx USB OTG Device peripheral to communicate with a PC
Host using the Bulk Only Transfer (BOT) and Small Computer System Interface (SCSI) transparent commands, 
while the microSD card is used as storage media. The STM32 MCU is enumerated as a MSC device using the 
native PC Host MSC driver to which the STM32446E-EVAL board is connected.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 180 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from a specific output of two PLLs PLL or PLL SAI.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

The 48 MHz clock for the USB FS can be derived from one of the two following sources:
  – PLL clock (clocked by the HSE): If the USB uses the PLL as clock source, the PLL VCO clock must be programmed
    to output 336 MHz frequency (USBCLK = PLLVCO/PLLQ).
  – PLLSAI clock (clocked by the HSE): If the USB uses the PLLSAI as clock source, the PLLSAI VCO clock must be programmed
    to output 384 MHz frequency (USBCLK = PLLSAIVCO/PLLSAIP).

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
      

@par USB Library Configuration

To select the appropriate USB Core to work with, user must add the following macro defines within the
compiler preprocessor (already done in the preconfigured projects provided with this application):
      - "USE_USB_HS" when using USB High Speed (HS) Core
      - "USE_USB_FS" when using USB Full Speed (FS) Core 

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
  - USB_Device/MSC_Standalone/Inc/ubsd_storage.h          Media Interface Layer header file   

	
@par Hardware and Software environment

  - This application runs on STM32F446xx devices.
    
  - This application has been tested with STMicroelectronics STM32446E-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32446E-EVAL RevB Set-up
    - Insert a microSD card into the STM32446E-EVAL uSD slot (CN4)
    - Connect the STM32446E-EVAL board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector:
      - CN8: to use USB High Speed (HS) 
      - CN9: to use USB Full Speed (FS)
	@note Make sure that :
	- jumper JP4 is on FS position (2-3)
	- jumper JP7 is on FS position (2-3)
        
@note  When the uSD Card is used; the Camera module must be unplugged, this is due to
       the shared pins between the two devices.
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32446E-EVAL_USBD-HS: to configure the project for STM32F446xx devices using USB OTG HS peripheral
   - STM32446E-EVAL_USBD-FS: to configure the project for STM32F446xx devices using USB OTG FS peripheral
 - Run the application
 

  */
