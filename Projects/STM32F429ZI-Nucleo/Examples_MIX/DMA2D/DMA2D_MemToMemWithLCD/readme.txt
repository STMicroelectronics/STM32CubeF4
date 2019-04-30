/**
  @page DMA2D_MemToMemWithLCD DMA2D : DMA2D Memory to Memory with LCD example (HAL/LL mixed usage example)
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_MIX/DMA2D/DMA2D_MemToMemWithLCD/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA2D_MemToMemWithLCD example.
  ******************************************************************************
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

  This example provides a description of how to configure the DMA2D peripheral in 
  Memory_to_Memory transfer mode and display the result on LCD, in resorting to
  DMA2D LL APIs for performance improvement.

  The source buffer in format RGB565 and size 128x160 located in Flash is copied 
  in Memory to Memory mode by the DMA2D to the LCD. 
  
  In this basic example, the goal is to show how to set some of the DMA2D IP
  parameters thru DMAD LL APIs:
  - input and output image format, alpha value
  - origin and destination buffers addresses
  - transfer size
  - DMA2D interruptions enabling
  - DMA2D transfer start 
  
  STM32F429ZI-Nucleo Rev B board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the DMA2D transfer is complete.
 - LED2 is ON when there is a DMA2D transfer error.
 - LED3 is ON if the adafruit shield is not detected.
 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 
  
  - DMA2D/DMA2D_MemToMemWithLCD/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - DMA2D/DMA2D_MemToMemWithLCD/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - DMA2D/DMA2D_MemToMemWithLCD/Inc/main.h                  Header for main.c module
  - DMA2D/DMA2D_MemToMemWithLCD/Inc/RGB565_128x160.h        RGB565 Image binary
  - DMA2D/DMA2D_MemToMemWithLCD/Src/stm32f4xx_it.c          Interrupt handlers
  - DMA2D/DMA2D_MemToMemWithLCD/Src/main.c                  Main program
  - DMA2D/DMA2D_MemToMemWithLCD/Src/stm32f4xx_hal_msp.c     HAL MSP module 
  - DMA2D/DMA2D_MemToMemWithLCD/Src/system_stm32f4xx.c      STM32F4xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32F429xx devices.
  
  - This example has been tested with STMicroelectronics STM32F429ZI-Nucleo Rev B 
    board and can be easily tailored to any other supported device 
    and development board. 

  - Adafruit 1.8" TFT shield must be connected on CN7, CN8, CN9 and CN10 Arduino connectors,
    for more details please refer to UM1726.

For more details about the adafruit 1.8" TFT shield, please visit: 
http://www.adafruit.com/blog/2012/04/26/new-product-adafruit-1-8-18-bit-color-tft-shield-wmicrosd-and-joystick/

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 