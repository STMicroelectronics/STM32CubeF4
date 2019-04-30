/**
  @page FMC_SRAM_PreInitConfig FMC PSRAM PreInit Config example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FMC/FMC_PSRAM_PreInitConfig/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the code execution from FMC PSRAM memory example.
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

This example describes how to execute a part of the code from the PSRAM external memory.
To do this, a section is created where the function is stored.

The PSRAM is configured before to call main() function and the memory is then available
after the system reset. 

The configuration of the PSRAM is done in the file system_stm32f4xx.c file in the 
function SystemInit_ExtMemCtl(). It is called by the function SystemInit(), which
is the first function called by the Reset_Handler.

The code in executed in a forever loop. It is a LEDs toggling example, with some 
code mapped in the PSRAM and other in Flash, to show the jump from one memory 
to the other.

When a power-off is performed, the PSRAM memory content is corrupted.

LED3, and LED4 toggle in this forever loop.

In this example, HCLK is configured at 100 MHz.
The configuration of the Clock is done in the system_stm32f4xx.c by calling SetSysClk().
A specefic flashloader is required for the programming of the PSRAM memory.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - FMC/FMC_PSRAM_PreInitConfig/Inc/stm32f4xx_hal_conf.h HAL configuration file
  - FMC/FMC_PSRAM_PreInitConfig/Inc/stm32f4xx_it.h       Interrupt handlers header file
  - FMC/FMC_PSRAM_PreInitConfig/Inc/main.h               Header for main.c module  
  - FMC/FMC_PSRAM_PreInitConfig/Src/stm32f4xx_it.c       Interrupt handlers
  - FMC/FMC_PSRAM_PreInitConfig/Src/main.c               Main program
  - FMC/FMC_PSRAM_PreInitConfig/Src/system_stm32f4xx.c   STM32F4xx system source file 


@par Hardware and Software environment

  - This example runs on STM32F413xx/STM32F423xx devices.
    
  - This example has been tested with STM32F413H-DISCO board and can be
    easily tailored to any other supported device and development board.

  - STM32F413H-DISCO Set-up :
      Board is configured by default to access PSRAM external memory

  - A specific region for the PSRAM memory has been added in the scatter file 
    (STM32F413xx/STM32F423xx for IAR toolchain, 
    STM32F413H-DISCO.sct for Keil toolchain and linker file for SW4STM32)
    
The PSRAM external flash loader is not integrated with supported toolchains, it’s only supported with STM32
ST-Link Utility V4.0.1
To load the example, use STM32 ST-Link Utility to program both internal Flash and external PSRAM memory:
1- Open STM32 ST-Link Utility V4.1.0, click on "External Loader" from the bar menu then check 
   "IS66WV51216EBLL_STM32413-DISCO" box 
2- Connect the STM32F413H-DISCO board to PC with USB cable through CN2
3- Use the generated file named "STM32F413H-DISCO.hex" with STM32 ST-Link Utility
   to program both internal Flash and external PSRAM memory

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
