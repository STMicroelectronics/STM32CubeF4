/**
  @page FLASH_EraseProgram FLASH Erase and Program application

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FLASH/FLASH_EraseProgram/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FLASH Erase and Program example.
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

This example describes how to configure and use the FLASH HAL API to erase and program 
the internal Flash memory.

After Reset, the Flash memory Program/Erase Controller is locked. To unlock it,
the FLASH_Unlock function is used.
Before programming the desired addresses, an erase operation is performed using 
the flash erase sector feature.  A programmation of the erase procedure is done 
in filling the erase init structure (erase will be done by sectors from the 1st 
sector to the calculation of sectors number) 
Then all these sectors will be erased one by one by calling HAL_FLASHEx_Erase function. 

@note: if problem occurs on a sector, erase will be stopped and faulty sector will 
be returned to user (through variable 'SectorError').

Once this operation is finished, the word programming operation will be performed by 
using the HAL_FLASH_Program function. The written data is then checked and the
result of the programming operation is stored into the MemoryProgramStatus variable.

STM32 Discovery boards LEDs can be used to monitor the transfer status:
 - LED4 (GREEN) is ON when there are no errors detected after programmation
 - LED5 (RED) is ON when there are errors detected after programmation
 - LED6 (BLUE) is ON when there is an issue during erase procedure
 - LED3 (ORANGE) is ON when there is an issue during program procedure

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, FLASH, Erase, Program, Sector, Mass Erase

@par Directory contents 

  - FLASH/FLASH_EraseProgram/Src/stm32f4xx_it.c       Interrupt handlers
  - FLASH/FLASH_EraseProgram/Src/main.c               Main program
  - FLASH/FLASH_EraseProgram/Src/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - FLASH/FLASH_EraseProgram/Inc/stm32f4xx_hal_conf.h HAL Configuration file
  - FLASH/FLASH_EraseProgram/Inc/stm32f4xx_it.h       Interrupt handlers header file
  - FLASH/FLASH_EraseProgram/Inc/main.h               Main program header file

@par Hardware and Software environment 

  - This example runs on STM32F401xCx Devices.
    
  - This example has been tested with STM32F401-Discovery RevB board (MB1115B) and can be
    easily tailored to any other supported device and development board.
  
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
