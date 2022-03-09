/**
  @page FMC_SDRAM_DataMemory FMC SDRAM data memory example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FMC/FMC_SDRAM_DataMemory/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FMC SDRAM_DataMemory example.
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

This example describes how to configure the FMC controller to access the SDRAM 
memory including heap and stack.

This example guides you through the different configuration steps by mean of HAL API
to use the MT48LC4M32B2B5-7 SDRAM mounted on STM324x9I-EVAL as data memory 
(including heap and stack).
  
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) 
to run at 180 MHz. 

The example scenario does not reflect a real application case, its purpose is to
provide only the procedure to follow to use the external SDRAM as data memory.

This example does not use the default library startup file. It uses a modified 
startup file provided with the example. The user has to add the new startup 
file in the project source list. While startup, the SDRAM memory is configured 
and initialized to be ready to contain data.
  
The user has to configure his preferred toolchain using the provided linker file.
The RAM zone is modified in order to use the external SDRAM memory as a RAM.

At this stage, all the used data can be located in the external SDRAM memory.

The user can use the debugger's watch to evaluate "uwTabAddr" and "MSPValue" variables
values which should be equal to "0xC00xxxxx".

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, FMC, SDRAM, Read, Write, Initialization, Access, Data Memory, Heap, Stack

@par Directory contents
 
 - FMC/FMC_SDRAM_DataMemory/Inc/stm32f4xx_hal_conf.h        Library Configuration file
 - FMC/FMC_SDRAM_DataMemory/Inc/stm32f4xx_it.h              Interrupt handlers header file
 - FMC/FMC_SDRAM_DataMemory/Inc//main.h                     Main program header file 
 - FMC/FMC_SDRAM_DataMemory/Src/stm32f4xx_it.c              Interrupt handlers
 - FMC/FMC_SDRAM_DataMemory/Src/main.c                      Main program 
 - FMC/FMC_SDRAM_DataMemory/Src/system_stm32f4xx.c          STM32F4xx system clock configuration file                                       


@par Hardware and Software environment 

  - This example runs on STM32F429xx/STM32F439xx devices.
  
  - This example has been tested with STMicroelectronics STM324x9I-EVAL RevB 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
