/**
  @page QSPI_PreInitConfig QSPI pre-Init configuration example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    QSPI/QSPI_ExecuteInPlace/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the code execution from QSPI memory example.
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

How to configure the QSPI IP in order to have access to external memory just after reset.

This example describes how to execute a part of the code from a QSPI memory. To do this, 
a section is created where the function is stored.

The QSPI is configured in memory-mapped mode before to call main() function and QSPI memory 
is then available after the reset. 

The configuration of the QSPI is done in the file system_stm32f4xx.c file in the 
function SystemInit_ExtMemCtl(). It is called by the function SystemInit(), which
is the first function called by the Reset_Handler.

The code in executed in a forever loop. It is the same code as GPIO_IOToggle example 
with some code mapped in QSPI mode and other in Flash, to show the jump from one 
memory to the other.

LED1, LED2, LED3 and LED4 toggle in this forever loop.

In this example, HCLK is configured at 180 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The connection of the LCD reset pin to a dedicated GPIO PH7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

Memory, QUADSPI, Erase, Write, DMA, section, memory-mapped mode, PreInit configuration, Jump

@par Directory contents 

  - QSPI/QSPI_ExecuteInPlace/Inc/stm32f4xx_hal_conf.h HAL configuration file
  - QSPI/QSPI_ExecuteInPlace/Inc/stm32f4xx_it.h       Interrupt handlers header file
  - QSPI/QSPI_ExecuteInPlace/Inc/main.h               Header for main.c module  
  - QSPI/QSPI_ExecuteInPlace/Src/stm32f4xx_it.c       Interrupt handlers
  - QSPI/QSPI_ExecuteInPlace/Src/main.c               Main program
  - QSPI/QSPI_ExecuteInPlace/Src/system_stm32f4xx.c   STM32F4xx system source file
  - QSPI/QSPI_ExecuteInPlace/Src/stm32f4xx_hal_msp.c  HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F469xx devices.

    
  - This example has been tested with STM32469I-DISCOVERY board and can be
    easily tailored to any other supported device and development board.

  - STM32469I-DISCOVERY Set-up :
    - Board is configured by default to access QSPI memory

  - A specific region for the QSPI memory has been added in the scatter file 
   (STM32F469xx_flash.icf for IAR toolchain, STM32469I-DISCOVERY.sct for Keil toolchain 
   and STM32F469NIHx_FLASH.ld for STM32CubeIDE toolchain) in order to map functions in this memory

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files 
 - Load your image into target memory with ST-Link Utility and external loader N25Q128A_STM32469I-DISCO
   -> !!! Be careful, don't try to use the toolchain to load the image, it is not able to load some code in QSPI memory !!!
 - Run the example


 */
