/**
  @page Demo   Demo STM32F4_Discovery 
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Demonstrations/readme.txt 
  * @author  MCD Application Team
  * @brief   Description STM32F4 Discovery Demo 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Demo Description

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 168 MHz.
           
- Connect the STM32F4-Discovery RevB & RevC board to a PC with a 'USB type A to Mini-B' cable
through USB connector CN1 to power the board. Then red LED1 (COM) and
LED2 (PWR) light up.
- All 4 LEDs between B1 and B2 are blinking as chenillard.
- Press User Button B1 (User) then MEMS sensor (LIS302DL for revB or LIS3DSCH 
for revC or LIS3DSH for revD) is enabled, move the board and observe the four Leds blinking according 
to the motion direction.
- Connect the board to a PC with a second USB'type A to micro-B cable through USB 
connector CN5, this convert it to a standard mouse. Move the STM32F4-Discovery
board and see the mouse moves according to the motion direction.
- Press B1 or B2 to restart demonstration.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


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
  - Demonstrations/Src/stm32f4xx_hal_msp.c     HAL MSP module file 


@par Hardware and Software environment

  - This Demo runs on STM32F407xx Devices.
  
  - This example has been tested with STMicroelectronics STM32F4-Discovery RevB & RevC & RevD (MB997B & MB997C & MB997D) 
    and can be easily tailored to any other supported device and development board.

  - STM32F4-Discovery RevB & RevC & RevD Set-up
    - Connect the STM32F4-DISCO board through 'USB micro A-Male 
      A-Male' cable to the connector CN5.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 