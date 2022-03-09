/**
  @page DMA_FIFOMode DMA FIFO mode Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DMA/DMA_FIFOMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA FIFO mode example.
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

This example provides a description of how to use a DMA to transfer 
a word data buffer from Flash memory to embedded SRAM with FIFO mode
enabled through the HAL API.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

DMA2_Stream0 is configured to transfer the contents of a 32-word data 
buffer stored in Flash memory to the reception buffer declared in RAM.

In this example the FIFO mode is enabled and the FIFO threshold is configured
as "FULL". Also an incremental burst transfer of 4 beats is requested. 

The start of transfer is triggered by software. DMA2_Stream0 memory-to-memory
transfer is enabled. Source and destination addresses incrementing is also enabled.
The transfer is started by setting the stream enable bit for DMA2_Stream0.
At the end of the transfer a Transfer Complete interrupt is generated since it
is enabled and the callback function (customized by user) is called.
 

boards LEDs can be used to monitor the transfer status:
 - LED1 is ON when the transfer is complete (into the Transfer Complete interrupt
   routine).
 - LED2 is ON when there are an error of the transfer (into the Transfer error 
   interrupt routine)
 - LED3 is ON when there are an error in the transfer process or in initialisation 
  (into the Transfer error interrupt routine)
 
It is possible to select a different stream for the DMA transfer
example by modifying defines values in the file main.h.
Note that only DMA2 streams are able to perform Memory-to-Memory transfers.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

System, DMA, FIFO Mode, Data Transfer, FIFO Threshold, Memory to memory, Stream

@par Directory contents 

  - DMA/DMA_FIFOMode/Src/system_stm32f4xx.c    STM32F4xx system clock configuration file
  - DMA/DMA_FIFOMode/Src/stm32f4xx_it.c        Interrupt handlers
  - DMA/DMA_FIFOMode/Src/main.c                Main program
  - DMA/DMA_FIFOMode/Inc/stm32f4xx_hal_conf.h  HAL Configuration file
  - DMA/DMA_FIFOMode/Inc/stm32f4xx_it.h        Interrupt handlers header file
  - DMA/DMA_FIFOMode/Inc/main.h                Main program header file

     
@par Hardware and Software environment

  - This example runs on STM32F469xx/STM32F479xx devices.

  - This example has been tested and validated with STMicroelectronics STM32469I-EVAL RevC
    evaluation boards and can be easily tailored to any other supported device 
    and development board.    

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

   

 */
