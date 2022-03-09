/**
  @page CRC_Example Cyclic Redundancy Check Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    CRC/CRC_Example/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of Cyclic Redundancy Check Example.
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

@par Example Description 

This example guides you through the different configuration steps by means of the 
HAL API. The CRC (Cyclic Redundancy Check) calculation unit computes the CRC code 
of a given buffer of 32-bit data words, using a fixed generator polynomial (0x4C11DB7).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

The CRC peripheral configuration is ensured by HAL_CRC_Init() function.
The latter is calling HAL_CRC_MspInit() function which core is implementing
the configuration of the needed CRC resources according to the used hardware (CLOCK). 
You can update HAL_CRC_Init() input parameters to change the CRC configuration.

The calculated CRC code is stored in uwCRCValue variable
and compared with the expected one stored in uwExpectedCRCValue variable.

STM32 nucleo's LEDs are used to monitor the example status:
  - LED1 (GREEN) is ON when a correct CRC value is calculated
  - LED3 (BLUE) is ON when an incorrect CRC value is calculated or when there is an initialization error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Security, CRC, CRC Polynomial, IEC 60870-5, hardware CRC

@par Directory contents 
  
  - CRC/CRC_Example/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - CRC/CRC_Example/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - CRC/CRC_Example/Inc/main.h                  Header for main.c module
  - CRC/CRC_Example/Src/stm32f4xx_it.c          Interrupt handlers
  - CRC/CRC_Example/Src/main.c                  Main program
  - CRC/CRC_Example/Src/stm32f4xx_hal_msp.c     HAL MSP module 
  - CRC/CRC_Example/Src/system_stm32f4xx.c      STM32F4xx system source file
 
@par Hardware and Software environment

  - This example runs on STM32F429xx devices.
  
  - This example has been tested with an STMicroelectronics NUCLEO-F429ZI  Rev B
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 

  */
