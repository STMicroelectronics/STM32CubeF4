/**
  @page IAP_Binary_Template Binary Template Readme file

  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    IAP/IAP_Binary_Template/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the IAP_Binary_Template directory.
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
  */

@par Application Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the program load address at 0x08004000, using your toolchain linker file
2. Relocate the vector table at address 0x08004000, using the "NVIC_SetVectorTable"
   function.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
Four LEDs are toggled with a timing defined by the Delay function.

@par Keywords

In-Application Programming, IAP, USART, Binary, flash

@par Directory contents 

 - IAP\IAP_Binary_Template/Inc/stm32f4xx_hal_conf.h          HAL Configuration file
 - IAP\IAP_Binary_Template/Inc/main.h                        Header for main.c module
 - IAP\IAP_Binary_Template/Inc/stm32f4xx_it.h                Header for stm32f4xx_it.c
 - IAP\IAP_Binary_Template/Src/main.c                        Main program
 - IAP\IAP_Binary_Template/Src/stm32f4xx_it.c                Interrupt handlers
 - IAP\IAP_Binary_Template/Src/system_stm32f4xx.c            STM32F4xx system clock configuration file 

     
@par Hardware and Software environment 

  - This application runs on STM32F407xx/417xx devices.
    
  - This application has been tested with STM324xG_EVAL board revB and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to load the SysTick example with the IAP, you must do the following:
 
 - EWARM:
    - Open the Project.eww workspace
    - Rebuild all files
    - A binary file "STM324xG_EVAL_SysTick.bin" will be generated under "STM324xG_EVAL/Exe" folder.  
    - Finally load this image with IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "/IAP_Binary_Template/MDK-ARM" directory and run "axftobin.bat"
      (Fromelf Exe path might have to be updated in "axftobin.bat" file, according to your Keil setup).
    - A binary file "STM324xG_EVAL_SysTick.bin" will be generated under "STM324xG_EVAL" folder. 
    - Finally load this image with IAP application

 - STM32CubeIDE: 
    - Open STM32CubeIDE toolchain
    - Browse to the STM32CubeIDE workspace directory, select the project 
      (.project file in \IAP_Binary_Template\STM32CubeIDE \STM324xG_EVAL directory).
    - Rebuild all project files: Select the project in the "Project explorer" window 
      then click on Project->build project menu.
    - Load this image with the IAP application (Select option 1 in IAP_Main menu)


 */
