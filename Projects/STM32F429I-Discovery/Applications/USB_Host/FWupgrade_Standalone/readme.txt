/**
  @page FWupgrade_Standalone FWupgrade_Standalone application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics  *******************
  * @file    USB_Host/FWupgrade_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Upgrading STM32F429I-Discovery firmware using a USB key
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

The firmware upgrade application or In-Application programming (IAP) is a feature that 
allows a user application to erase and write to on-chip flash memory. 
This application describes how to perform it.
  
This application uses The USB HOST to:
	- Download a binary file (.bin) from a Flash disk (thumb drive) to the STM32F4xx's 
	  internal flash memory.
	- Upload all the STM32F4xx's internal Flash memory content into a binary file.
	- Execute the user program.

This application is based on the STM32 USB On-The-Go (OTG) Host library. 
For more details about the USB Host stack and a mass storage demonstration,
please refer to user manual.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Host library, please refer to UM1720  
"STM32Cube USB Host library".

@par Keywords

Connectivity, USB_Host, Firmware upgrade, IAP, Binary

@par Directory contents 

  - USB_Host/FWupgrade_Standalone/Src/main.c                  Main program
  - USB_Host/FWupgrade_Standalone/Src/usbh_diskio_dma.c      FatFS usbh diskio driver implementation
  - USB_Host/FWupgrade_Standalone/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - USB_Host/FWupgrade_Standalone/Src/stm32f4xx_it.c          Interrupt handlers
  - USB_Host/FWupgrade_Standalone/Src/iap_menu.c              IAP State Machine   
  - USB_Host/FWupgrade_Standalone/Src/usbh_conf.c             General low level driver configuration
  - USB_Host/FWupgrade_Standalone/Src/command.c               IAP command functions
  - USB_Host/FWupgrade_Standalone/Src/flash_if.c              Flash layer functions
  - USB_Host/FWupgrade_Standalone/Src/usbh_diskio.c           USB diskio interface for FatFs
  - USB_Host/FWupgrade_Standalone/Inc/main.h                  Main program header file
  - USB_Host/FWupgrade_Standalone/Inc/usbh_diskio_dma.h      FatFS usbh diskio driver header file
  - USB_Host/FWupgrade_Standalone/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - USB_Host/FWupgrade_Standalone/Inc/command.h               IAP command functions header file
  - USB_Host/FWupgrade_Standalone/Inc/flash_if.h              Flash layer functions header file
  - USB_Host/FWupgrade_Standalone/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - USB_Host/FWupgrade_Standalone/Inc/usbh_conf.h             USB Host driver Configuration file
  - USB_Host/FWupgrade_Standalone/Inc/ffconf.h                FAT file system module configuration file
  
  - USB_Host/FWupgrade_Standalone/Binary/
    This folder contains images that can be loaded and executed by the FW upgrade application:
    - STM32F429I_DISCO_GPIO_EXTI.bin
    - STM32F429I_DISCO_HelloWorld.bin


@par Hardware and Software environment

  - This application runs on STM32F429xx devices.
    
  - This application has been tested with STMicroelectronics STM32F429I-Discovery RevC
    boards and can be easily tailored to any other supported device 
    and development board.    
     
  - STM32F429I-Discovery RevC Set-up
    - Plug the USB key into the STM32F429I-Discovery board through 'USB micro A-Male 
      to A-Female' cable connector CN6. 

   
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - To run the firmware upgrade application, proceed as follows:
 
1. Load the binary image of the user program to the root directory of a USB key. You can
use the provided binary images under the "USB_Host/FWupgrade_Standalone/Binary" folder. 
The binary should be renamed to "image.bin".

2. Program the firmware upgrade application into the internal Flash memory.
a) Open the project (under USB_Host/FWupgrade_Standalone) with your preferred toolchain.
b) Compile and load the project into the target memory and run the project.

+ After the board reset and depending on the Key button state:

1. Key button pressed: The firmware upgrade application is executed.
2. Key button not pressed: A test on the user application start address will be
performed and one of the below processes is executed.
- User vector table available: User application is executed.
- User vector table not available: firmware upgrade application is executed.

+ During the firmware upgrade application execution, there is a continuous check on the Key
button pressed state time. Depending on the state time of the Key button, one of the
following processes is executed.

=> If Key Button is Pressed More than > 3 seconds at firmware startup:
UPLOAD command will be executed immediately after completed execution of the DOWNLOAD command

=> If Key Button is Pressed less than< 3 seconds Only the DOWNLOAD command is executed.

+ LEDs status
* Red LED blinks in infinite loop 
	- Error: USB key disconnected.

* Red LED blinks in infinite loop
	- Error: Download done and USB key disconnected.

* Red LED blinks in infinite loop
	- Error: binary file not available

* Red LED blinks in infinite loop
	- Error: Buffer size limit, Exceed 32Kbyte
	
* Red LED blinks in infinite loop
	- Error: No available Flash memory size to load the binary file.
	
* Red LED blinks in infinite loop
	- Error: Flash erase error.
        
* Red LED blinks in infinite loop and Green LED On
	- Error: Flash programming error.
        
* Red LED blinks in infinite loop and Green LED ON
	- USB key read out protection ON.
        
* Green LED and Red LED ON
	- UPLOAD condition verified and the user should release the Key button.
	
* Red LED ON
	- DOWNLOAD ongoing.
	
* Green LED ON 
	- DOWNLOAD done.

* Red LED ON 
	- UPLOAD ongoing.

* Green LED ON 
	- UPLOAD done.

* Green LED ON
	- DOWNLOAD and UPLOAD done with success; and the MCU waiting until you
      press the Key button before execute the JUMP command.

* Green LED and Red LED blink in infinite loop
	- JUMP command done. new Biary after FW upgrade is launched.
	
+ User Program Condition
The user application (binary file) to be loaded into the Flash memory using the firmware
upgrade application is built by the following configuration settings:
	- Set the program load address to APPLICATION_ADDRESS in the toolchain linker file.
	- Relocate the vector table to address APPLICATION_ADDRESS using the the VECT_TAB_OFFSET 
	definition inside the system_stm32f4xx.c file.


 */
