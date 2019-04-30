/**
  @page ADC_GroupsRegularInjected ADC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/ADC/ADC_GroupsRegularInjected/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_GroupsRegularInjected example.
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
This example describes how to use a ADC peripheral with both ADC groups
(ADC group regular and ADC group injected) in their intended use case; 
This example is based on the STM32F4xx ADC LL API; 
peripheral initialization done using LL unitary services functions
for optimization purpose (performance and size).

Description of ADC group regular and group injected intended use case:
 - group regular for a high number of conversions and continuous
   data stream, with DMA transfer capability.
 - group injected for punctual conversions inserted between
   conversions of group regular.


Example configuration:
ADC is configured to use the 2 groups:
 - ADC group regular:
   ADC is configured to convert a single channel (1 channel from a GPIO),
   in single conversion mode, from HW trigger: timer peripheral .
 - ADC group injected:
   ADC is configured to convert a single channel (1 internal channel: VrefInt),
   in single conversion mode, from SW trigger.
Other peripherals are configured to be used with ADC group regular:
DMA is configured to transfer conversion data in an array (DMA transfer can be used
only with ADC group regular).
A timer is configured in time base and to generate TRGO events (trig from
timer can be used with both ADC groups regular and injected).

Example execution:
From the main program execution, the ADC group regular converts the selected channel
at each trig from timer.
DMA transfers conversion data to the array, DMA transfer complete interruption occurs.
Results array is updated indefinitely (DMA in circular mode).

For debug: variables to monitor with debugger watch window:
 - "aADCxConvertedData": ADC group regular conversion data
 - "uhADCxGrpInjectedConvertedData": ADC group injected conversion data
 - "ubDmaTransferStatus": status of DMA transfer of ADC group regular conversions
 - "ubAdcGrpInjectedUnitaryConvStatus": status of ADC group injected unitary conversion

From the first press on User Button, the ADC group injected converts the selected channel
from a SW conversion start command.
ADC group injected conversion complete interruption occurs, SW transfers
conversion data to a variable, LED2 is turned on.

Connection needed:
None.
Note: Optionally, a voltage can be supplied to the analog input pin (cf pin below),
      between 0V and Vdda=3.3V, to perform a ADC conversion on a determined
      voltage level.
      Otherwise, this pin can be let floating (in this case ADC conversion data
      will be undetermined).

Other peripherals used:
  1 GPIO for User Button
  1 GPIO for LED2
  1 GPIO for analog input: PA.04 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)
  DMA
  Timer


@par Directory contents 

  - ADC/ADC_GroupsRegularInjected/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - ADC/ADC_GroupsRegularInjected/Inc/main.h                  Header for main.c module
  - ADC/ADC_GroupsRegularInjected/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_GroupsRegularInjected/Src/stm32f4xx_it.c          Interrupt handlers
  - ADC/ADC_GroupsRegularInjected/Src/main.c                  Main program
  - ADC/ADC_GroupsRegularInjected/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
