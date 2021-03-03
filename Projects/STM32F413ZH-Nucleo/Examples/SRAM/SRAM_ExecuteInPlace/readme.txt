/**
  @page SRAM_ExecuteInPlace Execute a part of the code from the SRAM2 example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    SRAM/SRAM_ExecuteInPlace/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the SRAM Execute In Place example.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
   @endverbatim

@par Example Description 

This example describes how to execute a part of the code from the SRAM2 memory. To do this, 
a section is created where the function is stored.
 
At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) 
to run at 100 MHz. 

After LEDs configurations, a dedicated function called SRAM1_Read_Write() is executed from 
aliased SRAM2 (0x10000000 - 0x1000FFFF) to:
 - Write data in SRAM1.
 - Once this operation is finished, read back and check the written data.

The STM32F413ZH-Nucleo board LEDs can be used to monitor the SRAM1 Read/Write status:
 - LED1 is ON when there are no errors during R/W Operation. 
 - LED2 is ON when the data written in SRAM1 is not correct.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, SRAM, Execute, Initialization, Access

@par Directory contents 

  - SRAM/SRAM_ExecuteInPlace/Inc/stm32f4xx_hal_conf.h        HAL Configuration file  
  - SRAM/SRAM_ExecuteInPlace/Inc/stm32f4xx_it.h              Header for stm32f4xx_it.c
  - SRAM/SRAM_ExecuteInPlace/Inc/main.h                      Header for main.c module 
  - SRAM/SRAM_ExecuteInPlace/Src/stm32f4xx_it.c              Interrupt handlers
  - SRAM/SRAM_ExecuteInPlace/Src/main.c                      Main program
  - SRAM/SRAM_ExecuteInPlace/Src/system_stm32f4xx.c          STM32F4xx system clock configuration file

@par Hardware and Software environment

  - This example runs on STM32F413xx/STM32F423xx devices.
    
  - This example has been tested with STM32F413ZH-Nucleo board and can be
    easily tailored to any other supported device and development board.
      
@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
