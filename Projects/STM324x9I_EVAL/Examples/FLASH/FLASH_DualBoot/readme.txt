/**
  @page FLASH_DualBoot  FLASH Dual Boot example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FLASH/FLASH_DualBoot/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FLASH Dual boot example.
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

This example guides you through the different configuration steps to program the internal
flash memory bank 1 and bank 2 and to swap between both of them by mean of the FLASH HAL API.
  
At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) 
to run at 180 MHz.

Below are the steps to run this example:
1- Choose the FLASH_DualBoot_Bank2 project and generate its binary(ie: FLASH_DualBoot.bin)

2- Load this binary at the bank2 of the flash(at the address 0x08100000) using 
   STM32 ST-LINK Utilities(www.st.com) or any similar tool. 
   
@note:
 - You can avoid step 1 by directly loading the binary file provided with the example
 - You have to configure your preferred toolchain in order to generate the binary
   file after compiling the project.
   
 - You can use STM32 ST-LINK Utilities or any similar tool to initially reset the 
   BFB2 bit (disable the dual boot feature). 
   
3- Choose the FLASH_DualBoot_Bank1 project and run it, this project will be loaded
   in the bank1 of the flash: at the address 0x08000000
   
4- Click the user Tamper button to swap between the two banks

- If program in bank1 is selected, a message with a blue text back color will be 
  displayed on LCD and LED3 will remain toggling while LED4 is turn on.

- If program in bank2 is selected, a message with a red text back color will be 
  displayed on LCD and LED4 will remain toggling while LED3 is turn on.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, Flash, Dual boot, Dual Bank, Execute, Binary, Option bytes

@par Directory contents 

  - FLASH/FLASH_DualBoot/Inc/stm32f4xx_hal_conf.h        Library Configuration file  
  - FLASH/FLASH_DualBoot/Inc/stm32f4xx_it.h              Interrupt handlers header file
  - FLASH/FLASH_DualBoot/Inc/main.h                      Main program header file 
  - FLASH/FLASH_DualBoot/Src/stm32f4xx_it.c              Interrupt handlers
  - FLASH/FLASH_DualBoot/Src/main.c                      Main program
  - FLASH/FLASH_DualBoot/Src/system_stm32f4xx.c          STM32F4xx system clock configuration file
  - FLASH/FLASH_DualBoot/Binary/FLASH_DualBoot.bin       Binary file to load at bank2
  
  
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
