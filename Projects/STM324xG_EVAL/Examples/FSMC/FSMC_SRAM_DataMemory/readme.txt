/**
  @page FSMC_SRAM_DataMemory FSMC SRAM data memory example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FSMC/FSMC_SRAM_DataMemory/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FSMC SRAM data memory example.
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


This example describes how to configure the FSMC controller to access the SRAM 
memory including heap and stack.

This example guides you through the different configuration steps by mean of HAL API
to use the IS61WV102416BLL-10MLI SRAM mounted on STM324xG-EVAL as data memory 
(including heap and stack).
  
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) 
to run at 168 MHz. 

The example scenario does not reflect a real application case, its purpose is to
provide only the procedure to follow to use the external SRAM as data memory.

This example does not use the default library startup file. It uses a modified 
startup file provided with the example. The user has to add the new startup 
file in the project source list. While startup, the SRAM is configured 
and initialized to be ready to contain data.
  
The user has to configure his preferred toolchain using the provided linker file.
The RAM zone is modified in order to use the external SRAM as a RAM.

At this stage, all the used data can be located in the external SRAM.

The user can use the debugger's watch to evaluate "uwTabAddr" and "MSPValue" variables
values which should be equal to "0x640xxxxx". 
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, FSMC, SRAM, Data Memory, Read, Write, Initialization, Access

@par Directory contents
 
 - FSMC/FSMC_SRAM_DataMemory/Inc/stm32f4xx_hal_conf.h        Library Configuration file
 - FSMC/FSMC_SRAM_DataMemory/Inc/stm32f4xx_it.h              Interrupt handlers header file
 - FSMC/FSMC_SRAM_DataMemory/Inc//main.h                     Main program header file 
 - FSMC/FSMC_SRAM_DataMemory/Src/stm32f4xx_it.c              Interrupt handlers
 - FSMC/FSMC_SRAM_DataMemory/Src/main.c                      Main program 
 - FSMC/FSMC_SRAM_DataMemory/Src/system_stm32f4xx.c          STM32F4xx system clock configuration file 


@par Hardware and Software environment 

  - This example runs on STM32F407xx/417xx devices.  
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM324xG-EVAL RevC Set-up  
    - Please ensure that the jumper JP10 is not fitted in the position 2-3. 


In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
