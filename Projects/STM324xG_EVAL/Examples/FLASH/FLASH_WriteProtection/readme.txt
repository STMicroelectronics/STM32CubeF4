/**
  @page FLASH_WriteProtection FLASH write protection example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FLASH/FLASH_WriteProtection/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FLASH write protection example.
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

This example describes how to configure and use the FLASH HAL API to enable and 
disable the write protection of the internal Flash memory.

This example guides you through the different configuration steps by mean of HAL API
how to enable and disable the write protection for internal Flash memory integrated 
within  STM32F4xx devices, mounted on STM324xG-EVAL evaluation board.
  
At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) 
to run at 168 MHz.

Each time the User Tamper-button is pressed, the program will check the 
write protection status of FLASH_WRP_SECTORS (defined in main.c) 
  - If FLASH_WRP_SECTORS are write protected, the write protection will be disabled.
    Then the following message will be displayed on LCD, if protection disable 
    operation is done correctly: 
    "Write protection is disabled"
    Otherwise the following message will be displayed on LCD:
    "Write protection is not disabled"
  - If FLASH_WRP_SECTORS are not write protected, the write protection will be enabled.
    Then the following message will be displayed on LCD, if protection enable 
    operation is done correctly:
    "Write protection is enabled"
    Otherwise the following message will be displayed on LCD:
    "Write protection is not enabled"

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - FLASH/FLASH_WriteProtection/Inc/stm32f4xx_hal_conf.h        Library Configuration file  
  - FLASH/FLASH_WriteProtection/Inc/stm32f4xx_it.h              Interrupt handlers header file
  - FLASH/FLASH_WriteProtection/Inc/main.h                      Main program header file 
  - FLASH/FLASH_WriteProtection/Src/stm32f4xx_it.c              Interrupt handlers
  - FLASH/FLASH_WriteProtection/Src/main.c                      Main program
  - FLASH/FLASH_WriteProtection/Src/system_stm32f4xx.c          STM32F4xx system clock configuration file
  
  
@par Hardware and Software environment 

  - This example runs on STM32F407xx/417xx devices.  
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

      
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
       
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
