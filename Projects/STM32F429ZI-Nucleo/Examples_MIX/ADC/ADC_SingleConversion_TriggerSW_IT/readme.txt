/**
  @page ADC_SingleConversion_TriggerSW_IT ADC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_MIX/ADC/ADC_SingleConversion_TriggerSW_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_SingleConversion_TriggerSW_IT example.
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

This example describes how to use the ADC to perform a single ADC channel
conversion, at each software start. This example uses the interrupt programming
model (for programming models in Polling or DMA mode, refer to other
examples). This example is based on the STM32F4xx ADC HAL and LL API
(LL API usage for performance improvement).

Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from SW trigger.
ADC interruption enabled: EOC (end of conversion of ADC group regular).

Example execution:
At each press on push button, the ADC performs 1 conversion of the selected channel.
When conversion is completed, ADC interruption occurs.
IRQ handler callback function reads conversion data from ADC data register
and stores it into a variable, LED1 is turned on.

Notes: 1- "uhADCxConvertedData": ADC group regular conversion data following:
          uhADCxConvertedData = (Input voltage * ( 2^resolution -1)) / Vdda
          resolution = 12bits / Vdda = 3,3 V

       2- LED2 toggles in case of conversion error.

For debug: variables to monitor with debugger watch window:
 - "uhADCxConvertedData": ADC group regular conversion data
 - "uhADCxConvertedData_Voltage_mVolt": ADC conversion data computation to physical values

Connection needed:
None, if ADC channel and DAC channel are selected on the same GPIO.
Otherwise, connect a wire between DAC channel output and ADC input.
See comment below related to literal "WAVEFORM_GENERATION".

Other peripherals used:
  1 GPIO for push button
  1 GPIO for LED
  1 GPIO for analog input: PA.04 (Connector CN7 pin 17)
  1 GPIO for DAC channel output PA.04 (Connector CN7 pin 17)

Compilation switches are available to customize configuration and execution of this example:

One compilation switch is available to generate a waveform voltage
for test (located in main.h):
 - If literal "WAVEFORM_GENERATION" is defined: For this example purpose, generates a
   waveform voltage on a spare DAC channel LL_DAC_CHANNEL_1 (pin PA.04)
   so user has just to connect a wire between DAC channel output and ADC input to run this example.
   If ADC channel and DAC channel are selected on the same GPIO, then no external wire is required.
   Waveform generation: waveform circular, shape of ramp: Voltage is increasing at each press on push button, 
                        from 0V to maximum range (Vdda) in 5 steps (with vdda=3.3V: 0V, 0.8V, 1.65V, 2.5V, 3.3V),
                        then starting back from 0V.
   and follows circular cycles: At clicks counter maximum value reached, counter is set back to 0.
 - If literal "WAVEFORM_GENERATION" is not defined: no voltage is generated, user has
   to connect a voltage source to the selected ADC channel input to run this example.


@par Directory contents 

  - ADC/ADC_SingleConversion_TriggerSW_IT/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - ADC/ADC_SingleConversion_TriggerSW_IT/Inc/main.h                  Header for main.c module
  - ADC/ADC_SingleConversion_TriggerSW_IT/Src/stm32f4xx_it.c          Interrupt handlers
  - ADC/ADC_SingleConversion_TriggerSW_IT/Src/main.c                  Main program
  - ADC/ADC_SingleConversion_TriggerSW_IT/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F429xx devices.
    
  - This example has been tested with STM32F429ZI-Nucleo Rev B board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
