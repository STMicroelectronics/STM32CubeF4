/**
  @page Demo   Demo STM32F401_Discovery 
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Demonstrations/readme.txt 
  * @author  MCD Application Team
  * @brief   Description STM32F401 Discovery Demo 
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

@par Demo Description

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 84 MHz.
           
- Connect the STM32F401-Discovery RevB board to a PC with a 'USB type A to Mini-B' cable
through USB connector CN1 to power the board. Then red LED1 (COM) and
LED2 (PWR) light up.
- All 4 LEDs between B1 and B2 are blinking as chenillard.
- Press User Button B1 (User) then MEMS sensor (LSM303DLHC) is enabled, move the
board and observe the four Leds blinking according to the motion direction.
- Connect the board to a PC with a second USB'type A to micro-B cable through USB 
connector CN5, this convert it to a standard mouse. Move the STM32F401-Discovery 
board and see the mouse moves according to the motion direction.
- Press B1 or B2 to restart demonstration.



@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Demonstrations, MEMS sensor, LSM303DLHC, USB, mouse moves

@par Directory contents
 
  - Demonstrations/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - Demonstrations/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - Demonstrations/Inc/main.h                  Main program header file
  - Demonstrations/Inc/ffconf.h                FAT file system module configuration file
  - Demonstrations/Inc/usbd_desc.h             USB device HID descriptor header file   
  - Demonstrations/Inc/usbd_conf.h             USB device driver Configuration file    
  - Demonstrations/Src/stm32f4xx_it.c          Interrupt handlers
  - Demonstrations/Src/main.c                  Main program
  - Demonstrations/Src/usbd_desc.c             USB device HID descriptor   
  - Demonstrations/Src/usbd_conf.c             General low level driver configuration
  - Demonstrations/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment

  - This Demo runs on STM32F401xCx Devices.
  
  - This example has been tested with STMicroelectronics STM32F401-Discovery RevB (MB1115B) 
    and can be easily tailored to any other supported device and development board.

  - STM32F401-Discovery RevB Set-up
    - Connect the STM32F401C-DISCO board through 'USB micro A-Male 
      A-Male' cable to the connector CN5.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


  */
