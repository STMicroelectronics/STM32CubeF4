/**
  @page FMC_SRAM SRAM functionalities use example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FMC/FMC_SRAM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FMC SRAM example.
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

This example describes how to configure the FMC controller to access the SRAM 
memory.

This example guides you through the different configuration steps by mean of HAL API
to configure the FMC controller to access the IS61WV102416BLL-10M SRAM mounted 
on STM324x9I-EVAL RevB evaluation board.
 
At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.
  
The goal of this example is to explain the different steps to configure the FMC 
and make the SRAM device ready for access, without using the MSP layer.          

In this example, the SRAM device is configured and initialized explicitly 
following all initialization sequence steps. After initializing the device, user 
can perform read/write operations on it. A data buffer is written to the SRAM 
memory, then read back and checked to verify its correctness.

The user can chose his own configuration by commenting/uncommenting the defines for  
undesired/desired configurations , for example, to enable Continuous clock feature, 
uncomment the define for the configuration "FMC_CONTINUOUS_CLOCK_SYNC_ASYNC" in "main.h".
  
If the data is read correctly from SRAM, the LED1 is ON, otherwise the LED2 is ON.   


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, FMC, SRAM, Read, Write, Initialization, Access

@par Directory contents
                        
 - FMC/FMC_SRAM/Inc/stm32f4xx_hal_conf.h        Library Configuration file
 - FMC/FMC_SRAM/Inc/main.h                      Main program header file 
 - FMC/FMC_SRAM/Inc/stm32f4xx_it.h              Interrupt handlers header file
 - FMC/FMC_SRAM/Src/main.c                      Main program  
 - FMC/FMC_SRAM/Src/stm32f4xx_hal_msp.c         HAL MSP module  
 - FMC/FMC_SRAM/Src/stm32f4xx_it.c              Interrupt handlers
 - FMC/FMC_SRAM/Src/system_stm32f4xx.c          STM32F4xx system clock configuration file                                    
 
 
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
