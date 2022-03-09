/**
  @page QSPI_ReadWrite_IT QSPI Read/Write in interrupt mode example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    QSPI/QSPI_ReadWrite_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the QSPI Read/Write in interrupt mode example.
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

How to use a QSPI memory in interrupt mode.

This example describes how to erase a part of a QSPI memory, write data in Interrupt mode, read data 
in interrupt mode and compare the result in an infinite loop.

LED3 toggles each time a new comparison is good
LED4 is on as soon as a comparison error occurs
LED4 toggles as soon as an error is returned by HAL API

In this example, HCLK is configured at 100 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, QUADSPI, Erase, Read, Write, Interrupt

@par Directory contents 

  - QSPI/QSPI_ReadWrite_IT/Inc/stm32f4xx_hal_conf.h HAL configuration file
  - QSPI/QSPI_ReadWrite_IT/Inc/stm32f4xx_it.h       Interrupt handlers header file
  - QSPI/QSPI_ReadWrite_IT/Inc/main.h               Header for main.c module  
  - QSPI/QSPI_ReadWrite_IT/Src/stm32f4xx_it.c       Interrupt handlers
  - QSPI/QSPI_ReadWrite_IT/Src/main.c               Main program
  - QSPI/QSPI_ReadWrite_IT/Src/system_stm32f4xx.c   STM32F4xx system source file
  - QSPI/QSPI_ReadWrite_IT/Src/stm32f4xx_hal_msp.c  HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F413xx devices.
    
  - This example has been tested with STM32F413H-DISCOVERY board and can be
    easily tailored to any other supported device and development board.

  - STM32F413H-DISCOVERY Set-up :
    - Board is configured by default to access QSPI memory

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
