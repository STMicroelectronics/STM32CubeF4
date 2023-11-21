/**
  @page HID_LPM_Standalone USB Device Human Interface (HID) application with LPM
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    USB_Device/HID_LPM_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB HID_LPM application.
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

The STM32F4xx devices support the USB Link Power Management Protocol (LPM-L1) and complies with the USB 2.0
LPM-L1 ECN. The hpcd.Init.lpm_enable in the usbd_conf.c should be set to 1 to enable the support for LPM-L1
protocol in the USB stack.

LPM-L1 allows a USB host to configure the USB device into inactive state much faster than the normal 
USB suspend mode (L2). It also provides much faster wake-up times in the order of micro-seconds compared 
to the generic resume by host or upstream resume by device.

When LPM packet is received from the host, STM32 USB will Acknowledge the LPM packet
and it will enter in L1 suspend mode. During USB L1 suspend mode ,system will be
in STOP low power mode.

On Host L1 resume, STM32 will wakeup from STOP and USB resumes operations.

You can test L1 suspend/resume, by running the USBCV3.0 chapter9 for USB2.0 devices 
and select (in debug mode) test "TD9.21: LPM L1 Suspend Resume Test".
Please note that for running USBCV3.0, you'll need a PC with a USB3.0 (xHCI)
host controller (please refer to USBCV3.0 documentation for more information).

This application is a part of the USB Device Library package using STM32Cube firmware. It describes how to 
implement the USB Link Power Management (LPM) feature and how it can be used with the selective suspend 
mechanism to reduce system power consumption, in this application the STM32 is enumerated as a HID device 
supporting (LPM) feature. This application is based on HID_LPM_Standalone application available in the same 
firmware package.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 180 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from a specific output of two PLLs PLL or PLL SAI.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

The 48 MHz clock for the USB FS can be derived from one of the two following sources:
  – PLL clock (clocked by the HSE): If the USB uses the PLL as clock source, the PLL clock must be programmed
    to output 48 MHz frequency (USBCLK = PLLVCO/PLLQ).
  – PLLSAI clock (clocked by the HSE): If the USB uses the PLLSAI as clock source, the PLLSAI clock must be programmed
    to output 48 MHz frequency (USBCLK = PLLSAIVCO/PLLSAIP).
 
The function GetPointerData() is responsible to move the mouse cursor automatically,
which can be easily customized (square, circle...). In this application the cursor
is moving forward and backward in an infinite loop.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
For more details about the STM32Cube USB Device library, please refer to UM1734 
"STM32Cube USB Device library".
      
@note The connection of the LCD reset pin to a dedicated GPIO PH7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

Connectivity, USB_Device, USB, HID, Full Speed, High Speed, Joystick, Mouse, Power Management, LPM, LPM-L1,
ECN, Suspend mode, Stop mode, 

@par Directory contents

  - USB_Device/HID_LPM_Standalone/Src/main.c                  Main program
  - USB_Device/HID_LPM_Standalone/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - USB_Device/HID_LPM_Standalone/Src/stm32f4xx_it.c          Interrupt handlers
  - USB_Device/HID_LPM_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/HID_LPM_Standalone/Src/usbd_desc.c             USB device HID descriptor
  - USB_Device/HID_LPM_Standalone/Inc/main.h                  Main program header file
  - USB_Device/HID_LPM_Standalone/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - USB_Device/HID_LPM_Standalone/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - USB_Device/HID_LPM_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/HID_LPM_Standalone/Inc/usbd_desc.h             USB device HID descriptor header file  

@par Hardware and Software environment

  - This application runs on STM32F469xx devices.
    
  - This application has been tested with STMicroelectronics STM32469I-Discovery 
    boards and can be easily tailored to any other supported device and development board.

  - STM32469I-Discovery Set-up
    - Connect the STM32469I-Discovery board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector: 
      - CN13: to use USB Full Speed (FS)

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
  

   */
