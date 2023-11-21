/**
  @page HID_Standalone USB Device Human Interface (HID) application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    USB_Device/HID_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB HID application.
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

This application shows how to use the USB device application based on the Human Interface (HID) on the STM32F4xx devices.

This is a typical application on how to use the STM32F469xx USB OTG Device peripheral where the STM32 MCU is
enumerated as a HID device using the native PC Host HID driver to which the STM32469I-EVAL
board is connected, in order to emulate the Mouse directions using Joystick buttons mounted on the 
STM32469I-EVAL board.

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
 
This application supports remote wakeup (which is the ability of a USB device to bring a suspended bus back
to the active condition), and the Key button is used as the remote wakeup source. 
   
By default, in Windows powered PC the Power Management feature of USB mouse devices is turned off.
This setting is different from classic PS/2 computer functionality. Therefore, to enable the Wake from 
standby option, user must manually turn on the Power Management feature for the USB mouse.

To manually enable the wake from standby option for the USB mouse, proceed as follows: 
 - Start "Device Manager",
 - Select "Mice and other pointing devices",
 - Select the "HID-compliant mouse" device (make sure that PID &VID are equal to 0x5710 & 0x0483 respectively) 
 - Right click and select "Properties", 
 - Select "Power Management" tab,
 - Finally click to select "Allow this device to wake the computer" check box.
 
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
      - "USE_USB_HS" and "USE_USB_HS_IN_FS" when using USB High Speed (HS) Core in FS mode
      
@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

Connectivity, USB_Device, USB, HID, Full Speed, High Speed, Joystick, Mouse, Keyboard, enumerate

@par Directory contents

  - USB_Device/HID_Standalone/Src/main.c                  Main program
  - USB_Device/HID_Standalone/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - USB_Device/HID_Standalone/Src/stm32f4xx_it.c          Interrupt handlers
  - USB_Device/HID_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/HID_Standalone/Src/usbd_desc.c             USB device HID descriptor
  - USB_Device/HID_Standalone/Inc/main.h                  Main program header file
  - USB_Device/HID_Standalone/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - USB_Device/HID_Standalone/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - USB_Device/HID_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/HID_Standalone/Inc/usbd_desc.h             USB device HID descriptor header file  

@par Hardware and Software environment

  - This application runs on STM32F469xx and STM32F479xx devices.
    
  - This application has been tested with STMicroelectronics STM32469I-EVAL RevC
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32469I-EVAL RevC Set-up
    - Connect the STM32469I-EVAL board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector:
      - CN8 : to use USB High Speed (HS) 
      - CN13: to use USB Full Speed (FS)
      - CN14: to use USB HS-IN-FS.
              Note that some FS signals are shared with the HS ULPI bus, so some PCB rework is needed.
              For more details, refer to section "USB OTG2 HS & FS" in STM32469I-EVAL Evaluation Board 
              User Manual.
        @note Make sure that :
         - jumper JP9 must be removed when using USB OTG FS

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32469I-EVAL_USBH-HS: to configure the project for STM32F469xx devices using USB OTG HS peripheral
   - STM32469I-EVAL_USBH-FS: to configure the project for STM32F469xx devices using USB OTG FS peripheral
   - STM32469I-EVAL_USBH-HS-IN-FS: to configure the project for STM32F469xx devices and use USB OTG HS 
                                   peripheral In FS (using embedded PHY).
 - Run the application

 @note Known Limitations
 The Low Power management (suspend/resume and Remote wakeup) In HS mode is not yet supported. 
 Only FS and HS-IN-FS mode are supported
  

   */
