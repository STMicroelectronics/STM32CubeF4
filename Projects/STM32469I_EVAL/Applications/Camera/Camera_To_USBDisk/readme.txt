/**
  @page Camera_To_USBDisk Camera to USB Disk application
  
  @verbatim
  ******************************************************************************
  * @file    Camera/Camera_To_USBDisk/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Camera to USB Disk application.
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

This application provides a short description of how to use the DCMI to interface with
camera module and display in continuous mode the picture on LCD and to save a picture 
in USB device.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

The Digital camera interface is configured to receive the capture from
the camera module mounted on STM32469I_EVAL evaluation board.
DMA2 Stream1 channel1 is configured to transfer the picture from DCMI peripheral
to an external RAM used by the LCD as a frame buffer.   

The camera module is configured to generate (480x272) image resolution
and the LCD is configured to display in resolution WVGA(800x480) in orientation landscape.

When the tamper button is pressed an image is saved under USBdisk.
  - LED1 is on to indicate the end of saving operation and a message is displayed 
    on LCD to indicate the beginning and the end of the saving operation.
  - LED3 is ON when any error occurs.
  
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note Make sure jumper JP3 of STM32469I-EVAL board is on position PC0, because PA4 is shared with the DCMI_HSYNC signal.

@par Keywords

Display, Camera, USB Disk, DCMI, LCD, DMA, Frame buffer, WVGA

@par Directory contents

    - Camera/Camera_To_USBDisk/Inc/stm32f4xx_hal_conf.h    HAL configuration file
    - Camera/Camera_To_USBDisk/Inc/main.h                  Main program header file
    - Camera/Camera_To_USBDisk/Inc/usbh_diskio_dma.h       FatFS usbh diskio driver header file
    - Camera/Camera_To_USBDisk/Inc/stm32f4xx_it.h          Interrupt handlers header file
    - Camera/Camera_To_USBDisk/Inc/ffconf.h                FAT file system module configuration file 
    - Camera/Camera_To_USBDisk/Inc/usbh_conf.h             Header for usbh_conf.c
    - Camera/Camera_To_USBDisk/Src/main.c                  Main program  
    - Camera/Camera_To_USBDisk/Src/usbh_diskio_dma.c       FatFS usbh diskio driver implementation
    - Camera/Camera_To_USBDisk/Src/stm32f4xx_it.c          Interrupt handlers   
    - Camera/Camera_To_USBDisk/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
    - Camera/Camera_To_USBDisk/Src/usbh_conf.c             USB configuration file.


@par Hardware and Software environment
  
  - This application runs on STM32F469xx and STM32F479xx devices.
  
  - This application has been tested with STM32469I-EVAL RevC board which includes
    the KoD DSI LCD and S5K5CAG camera module.

  - STM32469I-EVAL Set-up
    - Plug the USB key into the STM32469I-EVAL board through 'USB micro A-Male 
      to A-Female' cable (HS mode: connector CN8).
    
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
  

 */
