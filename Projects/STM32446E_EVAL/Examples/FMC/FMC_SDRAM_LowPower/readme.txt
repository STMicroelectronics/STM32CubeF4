/**
  @page FMC_SDRAM_LowPower SDRAM memory functionalities example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FMC/FMC_SDRAM_LowPower/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FMC_SDRAM_LowPower example.
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
memory in low power mode (SDRAM Self Refresh mode).

This example guides you through the different configuration steps by mean of HAL API
to configure the FMC controller to access the MT48LC4M16A2P-6A SDRAM memory mounted 
on STM32446E-EVAL evaluation board, in low power mode (SDRAM Self Refresh mode).

It gives a simple application of the FMC SDRAM low power mode use (self refresh mode) 
while the MCU is in a low power mode (STOP mode).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.
  
The purpose is to show how the SDRAM can retain data written after entering STOP mode. 
STOP mode is a CPU low power mode which stops all peripherals clocks in the 1.2V 
domain. Only internal SRAM and registers content are preserved in this mode.
  
After SDRAM initialization, the data is written to the memory and a "self refresh" 
command is sent to the SDRAM. The program waits for User push-button to be pushed 
to enter the CPU in STOP mode, the LED1 and LED3 are then turned ON.
The wakeup from STOP mode is done when pushing User push-button and the CPU returns 
to RUN mode. At this time, the system clock is reconfigured using the function 
SystemClock_Config(). The LED3 is then turned OFF.

Finally, a "normal mode" command is send to SDRAM memory to exit self refresh mode. 
The data written to SDRAM is read back and checked.  
   
LEDs are used to indicate the system state as following:
  - LED1 and LED3 ON: CPU enters STOP mode.
  - LED1 ON: correct data transfer (PASS). CPU in RUN mode.
  - LED3 ON: incorrect data transfer (FAIL). CPU in RUN mode.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, FMC, SDRAM, Read, Write, Initialization, Access, Low power, Self Refresh, Stop mode

@par Directory contents 

  - FMC/FMC_SDRAM_LowPower/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - FMC/FMC_SDRAM_LowPower/Inc/main.h                  Header for main.c module  
  - FMC/FMC_SDRAM_LowPower/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - FMC/FMC_SDRAM_LowPower/Src/main.c                  Main program
  - FMC/FMC_SDRAM_LowPower/Src/stm32f4xx_it.c          Interrupt handlers
  - FMC/FMC_SDRAM_LowPower/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with STM32446E-EVAL board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
