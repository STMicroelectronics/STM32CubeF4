/**
  @page FatFs_USBDisk   FatFs with USB disk drive application
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FatFs/FatFs_USBDisk/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FatFs with USB disk drive application
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

How to use STM32Cube firmware with FatFs middleware component as a generic FAT
file system module and STM32 USB On-The-Go (OTG) host library, in both Full
Speed (FS) and High Speed (HS) modes. This example develops an application
exploiting FatFs features with USB disk drive configuration.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The user is provided with the SystemClock_Config() function to configure the system 
clock (SYSCLK) to run at 180 MHz. The Full Speed (FS) USB module uses internally 
a 48-MHz clock which is coming from a specific output of two PLLs PLL or PLL SAI.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.
           
The application is based on writing a text file to a drive, and it's performed 
using FatFs APIs to access the FAT volume as described in the following steps: 

 - Link the USB Host disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the USB drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Unlink the USB Host disk I/O driver.
 
Notice that the application manages possible errors that might occur during the 
access to FAT volume, when using FatFs APIs. In addition, user can check if the
written text file is available on the USB disk.

It is possible to fine tune needed FatFs features by modifying defines values 
in FatFs configuration file "ffconf.h" available under the project includes 
directory, in a way to fit the application requirements. 

STM32 Eval boards LEDs can be used to monitor the application status:
  - LED1 is ON when the application runs successfully.
  - LED3 is ON when any error occurs.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about FatFs implementation on STM32Cube, please refer to UM1721 "Developing Applications 
on STM32Cube with FatFs".


@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

Connectivity, USB_Host, FatFS, FAT, File system 

@par Directory contents
 
  - FatFs/FatFs_USBDisk/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - FatFs/FatFs_USBDisk/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - FatFs/FatFs_USBDisk/Inc/main.h                  Main program header file
  - FatFs/FatFs_USBDisk/Inc/usbh_diskio_dma.h       FatFS usbh diskio driver header file
  - FatFs/FatFs_USBDisk/Inc/ffconf.h                FAT file system module configuration file   
  - FatFs/FatFs_USBDisk/Src/stm32f4xx_it.c          Interrupt handlers
  - FatFs/FatFs_USBDisk/Src/main.c                  Main program
  - FatFs/FatFs_USBDisk/Src/usbh_diskio_dma.c       FatFS usbh diskio driver implementation
  - FatFs/FatFs_USBDisk/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
         
 
@par Hardware and Software environment

  - This application runs on STM32F446xx devices.
    
  - This application has been tested with STMicroelectronics STM32469I-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32469I-EVAL Set-up
    - Plug the USB key into the STM32469I-EVAL board through 'USB micro A-Male 
      to A-Female' cable to the connector:
      - CN8: to use USB High Speed (HS) 
      - CN13: to use USB Full Speed (FS)
      @note Please ensure that jumper JP9 is not fitted
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32469I-EVAL_USBH-HS: to configure the project for STM32F4xx devices using USB OTG HS peripheral
   - STM32469I-EVAL_USBH-FS: to configure the project for STM32F4xx devices using USB OTG FS peripheral
 - Run the application


  */
