/**
  @page ADC_TripleModeInterleaved  Use ADC1, ADC2 and ADC3 in Triple interleaved mode and DMA mode2 with 6Msps 

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    ADC/ADC_TripleModeInterleaved/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Triple interleaved mode and DMA mode2 example
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

How to use the ADC peripheral to convert a regular channel in Triple 
interleaved mode.

This example provides a short description of how to use the ADC peripheral to 
convert a regular channel in Triple interleaved mode using DMA in mode 2 with 
6Msps .

The Triple interleaved delay is configured 5 ADC clk cycles.

In Triple ADC mode, three DMA requests are generated: 
- On the first request, both ADC2 and ADC1 data are transferred (ADC2 data take 
  the upper half-word and ADC1 data take the lower half-word). 
- On the second request, both ADC1 and ADC3 data are transferred (ADC1 data take
  the upper half-word and ADC3 data take the lower half-word).
- On the third request, both ADC3 and ADC2 data are transferred (ADC3 data take 
  the upper half-word and ADC2 data take the lower half-word) and so on.

On each DMA request (two data items are available) two half-words representing 
two ADC-converted data items are transferred as a word.

A DMA request is generated each time 2 data items are available :
1st request: ADC_CDR[31:0] = (ADC2_DR[15:0] << 16) | ADC1_DR[15:0] (step1)
2nd request: ADC_CDR[31:0] = (ADC1_DR[15:0] << 16) | ADC3_DR[15:0] (step2)
3rd request: ADC_CDR[31:0] = (ADC3_DR[15:0] << 16) | ADC2_DR[15:0] (step3)
4th request: ADC_CDR[31:0] = (ADC2_DR[15:0] << 16) | ADC1_DR[15:0] (step1) and so on.

The conversion is triggered by software.

The ADC1, ADC2 and ADC3 are configured to convert ADC Channel 12.
By this way, the ADC can reach 6Msps, in fact the same channel is converted
each 5 cycles

In this example, the system clock is 144MHz, APB2 = 72MHz and ADC clock = APB2 /2. 
Since ADCCLK= 36MHz and Conversion rate = 5 cycles 
==> Conversion Time = 36M/5cyc = 7.2Msps

STM32 Eval board's LEDs can be used to monitor the transfer status:
  - LED1 is on when the transfer process is successfully completed.
  - LED3 is ON when there is an error in initialization.

User should monitor aADCTripleConvertedValue variable to get the converted value.


@note Refer to "simulation.xls" file to have the diagram simulation of the example.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - ADC/ADC_TripleModeInterleaved/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_TripleModeInterleaved/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - ADC/ADC_TripleModeInterleaved/Inc/main.h                  Main program header file  
  - ADC/ADC_TripleModeInterleaved/Src/stm32f4xx_it.c          Interrupt handlers
  - ADC/ADC_TripleModeInterleaved/Src/main.c                  Main program
  - ADC/ADC_TripleModeInterleaved/Src/stm32f4xx_hal_msp.c     HAL MSP module 
  - ADC/ADC_TripleModeInterleaved/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment 

  - This example runs on STM32F407xx/417xx devices.
    
  - This example has been tested with STM324xG-EVAL RevC evaluation board and can be
    easily tailored to any other supported device and development board.

  - STM324xG-EVAL RevC Set-up 
    - Connect PC.2 to a power supply (do not forget to connect the power supply GND
      to the EVAL board GND)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
