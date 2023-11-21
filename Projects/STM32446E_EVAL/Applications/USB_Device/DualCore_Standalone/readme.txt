/**
  @page DualCore_Standalone USB Device Dual Core application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    USB_Device/DualCore_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Device Dual Core application.
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

This application is a part of the USB Device Library package using STM32Cube firmware. It describes how to use
USB device application based on the STM32F4xx multi core support feature integrating the Device Communication 
Class (CDC) and Human Interface (HID) in the same project.

This is a typical application on how to use the STM32F446xx USB OTG Device peripheral, where STM32 is 
enumerated as a CDC device in the High Speed mode, and also as a HID device in the Full Speed mode,
using the native PC Host HID/CDC drivers to which the STM32446E-EVAL board is connected.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at at 180 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from a specific output of two PLLs PLL or PLL SAI.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

The 48 MHz clock for the USB FS can be derived from one of the two following sources:
  – PLL clock (clocked by the HSE): If the USB uses the PLL as clock source, the PLL VCO clock must be programmed
    to output 336 MHz frequency (USBCLK = PLLVCO/PLLQ).
  – PLLSAI clock (clocked by the HSE): If the USB uses the PLLSAI as clock source, the PLLSAI VCO clock must be programmed
    to output 384 MHz frequency (USBCLK = PLLSAIVCO/PLLSAIP).

When the application is started, the user has just to plug the two USB cables into a PC host and two
USB devices (CDC/HID) are automatically detected. A new VCP drive appears in the system window
for the CDC device and serial communication operations can be performed as with any other serial
communication drive. A new HID Mouse device is detected, and the Joystick buttons mounted on 
the STM32446E-EVAL board, allow to user to emulate the Mouse directions.

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
      - "USE_USB_HS" and "USE_USB_FS" when using USB Dual Core Mode

@par Keywords

Connectivity, USB_Device, USB, HID, MSC, Full Speed, High Speed, OTG, enumeration, mass storage, mouse, USB Stick

@par Directory contents

  - USB_Device/DualCore_Standalone/Src/main.c                  Main program
  - USB_Device/DualCore_Standalone/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - USB_Device/DualCore_Standalone/Src/stm32f4xx_it.c          Interrupt handlers
  - USB_Device/DualCore_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/DualCore_Standalone/Src/usbd_cdc_desc.c         USB device CDC descriptor
  - USB_Device/DualCore_Standalone/Src/usbd_hid_desc.c         USB device HID descriptor
  - USB_Device/DualCore_Standalone/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - USB_Device/DualCore_Standalone/Src/usbd_cdc_interface.c    USBD CDC interface 
  - USB_Device/DualCore_Standalone/Inc/main.h                  Main program header file
  - USB_Device/DualCore_Standalone/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - USB_Device/DualCore_Standalone/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - USB_Device/DualCore_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/DualCore_Standalone/Inc/usbd_desc.h             USB device descriptor header file  
  - USB_Device/DualCore_Standalone/Inc/usbd_cdc_desc.h         USB device CDC descriptor header file
  - USB_Device/DualCore_Standalone/Inc/usbd_hid_desc.h         USB device HID descriptor header file
  - USB_Device/DualCore_Standalone/Inc/usbd_cdc_interface.h    USBD CDC interface header file
	
@par Hardware and Software environment

  - This application runs on STM32F446xx devices.
    
  - This application has been tested with STMicroelectronics STM32446E-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32446E-EVAL RevB Set-up
    - Connect the STM32446E-EVAL board to the PC through two 'USB micro A-Male 
      to A-Male' cables to the connectors:
      - CN8: to use USB High Speed (HS)
      - CN9: to use USB Full Speed (FS)
    - Connect the STM32446E-EVAL board to the PC (or to another evaluation board) through RS232 (USART)
      serial cable CN25 (USART1) connector.
	  @note Make sure that:
         - jumper JP4 is on USART1_RX position (1-2) 
	       - jumper JP6 is on USART1_TX position (2-3) 
	       - jumper JP7 is on USART1_TX position (1-2) 
	       - jumper JP8 is on USART1_RX position (1-2)
  
    - For loopback mode test: remove RS232 cable on CN16 and connect directly USART TX and RX pins:
      PA10 and PA9 (with a cable or a jumper)
              

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 

 */
