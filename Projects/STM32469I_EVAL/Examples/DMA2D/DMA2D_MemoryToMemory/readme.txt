/**
  @page DMA2D_MemoryToMemory DMA2D Memory to Memory example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DMA2D/DMA2D_MemoryToMemory/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32F4xx DMA2D Memory to Memory example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

This example provides a description of how to configure DMA2D peripheral in 
Memory_to_Memory transfer mode.
The example transfers with DMA2D an internal SRAM static texture array aBufferInput[] of size 16x32 
and format ARGB4444 (16 bpp) to destination buffer in internal SRAM in array aBufferResult[].
The number of bytes transferred is then 16x32x2=1024 bytes.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the SYSTICK.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

In this basic example the goal is to explain the different fields of the DMA2D 
structure in the case of Memory_to_Memory transfer mode

After DMA2D configuration, the data transfer is performed and then the transferred 
data is checked by byte to byte memory comparing of the source and destination buffers aBufferInput[] and aBufferResult[].   

STM32469I-EVAL board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the DMA2D transfer is complete.
 - LED2 is ON when there is a DMA2D transfer error or when data are not correctly transferred.
 - LED3 is ON when there is an error in DMA2D transfer/Init process.
 - LED4 is ON when the data are correctly transferred : that is no error in comparison source and destination buffers.
If everything all right, at the end of the test, LED1 and LED4 must be ON, LED2 and LED3 must be OFF.
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - DMA2D/DMA2D_MemoryToMemory/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - DMA2D/DMA2D_MemoryToMemory/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - DMA2D/DMA2D_MemoryToMemory/Inc/main.h                  Header for main.c module  
  - DMA2D/DMA2D_MemoryToMemory/Src/stm32f4xx_it.c          Interrupt handlers
  - DMA2D/DMA2D_MemoryToMemory/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - DMA2D/DMA2D_MemoryToMemory/Src/main.c                  Main program
  - DMA2D/DMA2D_MemoryToMemory/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F469xx/STM32F479xx devices.

  - This example has been tested and validated with STMicroelectronics STM32469I-EVAL RevC board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
