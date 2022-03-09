/**
  @page LTDC_Paint LTDC Paint application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics  *******************
  * @file    Display/LTDC_Paint/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LTDC Paint application.
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

 This application describes how to configure LCD touch screen and attribute 
 an action related to configured touch zone.
 
 At the beginning of the main program the HAL_Init() function is called to reset 
 all the peripherals, initialize the Flash interface and the systick.
 Then the SystemClock_Config() function is used to configure the system
 clock (SYSCLK) to run at 180 MHz. 

 After LCD and touch screen initialization, a menu is displayed on the screen 
 
 The menu contains a palette of colors, clear icon, save icon, different kind 
 of draw size, the current selected size and color and a working rectangle. 

 The user can select the color, the size draw inside the rectangle and save 
 the picture in USB mass storage. 
 Link the the USB mass storage disk I/O driver.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Graphic, Display, LTDC, Pictures, USB, BMP, Background, Foreground, Layers, Touchscreen, Calibration

@par Directory contents

    - Display/LTDC_Paint/Inc/main.h                 Main configuration file
    - Display/LTDC_Paint/Inc/usbh_diskio_dma.h      FatFS usbh diskio driver header file
    - Display/LTDC_Paint/Inc/stm32f4xx_it.h         Interrupt handlers header file
    - Display/LTDC_Paint/Inc/stm32f4xx_hal_conf.h   HAL Configuration file 
    - Display/LTDC_Paint/Inc/ffconf.h               FAT file system module configuration file
    - Display/LTDC_Paint/Inc/usbh_conf.h            Configuration file for USB module
    - Display/LTDC_Paint/Inc/color2.h               Image used to display colored pens
    - Display/LTDC_Paint/Inc/save.h                 Image used to display save icon
    - Display/LTDC_Paint/Src/main.c                 Main program 
    - Display/LTDC_Paint/Src/usbh_diskio_dma.c      FatFS usbh diskio driver implementation
    - Display/LTDC_Paint/Src/stm32f4xx_it.c         Interrupt handlers
    - Display/LTDC_Paint/Src/usbh_conf.c            Main function to configure USB
    - Display/LTDC_Paint/Src/ts_calibration.c       Main function used to calibrate TS
    - Display/LTDC_Paint/Src/system_stm32f4xx.c     STM32F4xx system clock configuration file


@par Hardware and Software environment

  - This application runs on STM32F429xx Devices.
    
  - This application has been tested with STM32F429I-Discovery RevC board and can be
    easily tailored to any other supported device and development board.  

  - STM32F429I-DISCO RevB Set-up
    - Plug the USB key into the STM32F429I-DISCO board through 'USB micro A-Male 
      to A-Female' cable(CN6).


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


                                    */
