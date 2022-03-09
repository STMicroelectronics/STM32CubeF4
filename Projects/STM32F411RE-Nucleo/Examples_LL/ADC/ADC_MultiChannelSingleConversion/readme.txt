/**
  @page ADC_MultiChannelSingleConversion ADC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/ADC/ADC_MultiChannelSingleConversion/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_MultiChannelSingleConversion example.
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
This example describes how to use a ADC peripheral to convert several channels, 
ADC conversions are performed successively in a scan sequence; 
This example is based on the STM32F4xx ADC LL API; 
peripheral initialization done using LL unitary services functions
for optimization purpose (performance and size).

Example configuration:
ADC is configured in single conversion mode, from SW trigger.
Sequencer of group regular (default group available on ADC of all STM32 devices)
is configured to convert 3 channels: 1 channel from GPIO, 2 internal channels:
temperature sensor and VrefInt.
DMA is configured to transfer conversion data in an array of size three (one array
address for conversion data of each channel), in circular mode to update
results array at each ADC sequence conversions.

Example execution:
At each press on User Button, the ADC executes the scan sequence: 
ADC performs conversion of the three selected channels successively,
in burst (discontinuous mode not enabled).
The 3 ADC conversions data are transferred by DMA into the results array
"aADCxConvertedData".
ADC conversion data of each sequence rank is at an address of the array:
 - aADCxConvertedData[0]: ADC channel set on rank1 (GPIO as analog input)
 - aADCxConvertedData[1]: ADC channel set on rank2 (VrefInt)
 - aADCxConvertedData[2]: ADC channel set on rank3 (Temperature sensor)
LED2 is turned on when ADC conversions and DMA transfer of the sequence
are completed.

ADC conversions raw data are computed to physical values
using LL ADC driver helper macro:
 - Value of voltage on GPIO pin (on which is mapped ADC channel) (unit: mV)
 - Value of internal voltage reference VrefInt (unit: mV)
 - Value of temperature (unit: degree Celsius)
 - Value of analog reference voltage (Vref+), connected to analog voltage supply Vdda (unit: mV)

For debug: variables to monitor with debugger watch window:
 - "aADCxConvertedData": ADC group regular conversion data
 - "ubDmaTransferStatus": status of DMA transfer of ADC group regular conversions
 - "uhADCxConvertedData_VoltageGPIO_mVolt":        Value of voltage on GPIO pin (on which is mapped ADC channel) (unit: mV)
 - "uhADCxConvertedData_VrefInt_mVolt":            Value of internal voltage reference VrefInt (unit: mV)
 - "hADCxConvertedData_Temperature_DegreeCelsius": Value of temperature (unit: degree Celsius)
 - "uhADCxConvertedData_VrefAnalog_mVolt":         Value of analog reference voltage (Vref+), connected to analog voltage supply Vdda (unit: mV)

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

@par Keywords

Analog, ADC, Analog to Digital, Multi Channel, Conversion, DMA, Single, Measurement

@par Directory contents 

  - ADC/ADC_MultiChannelSingleConversion/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - ADC/ADC_MultiChannelSingleConversion/Inc/main.h                  Header for main.c module
  - ADC/ADC_MultiChannelSingleConversion/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_MultiChannelSingleConversion/Src/stm32f4xx_it.c          Interrupt handlers
  - ADC/ADC_MultiChannelSingleConversion/Src/main.c                  Main program
  - ADC/ADC_MultiChannelSingleConversion/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
