/**
  @page ADC_TemperatureSensor ADC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/ADC/ADC_TemperatureSensor/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_TemperatureSensor example.
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
This example describes how to use a ADC peripheral to perform 
a single ADC conversion of the internal temperature sensor 
and to calculate the temperature in Celsius degrees; 
Example using programming model: polling 
(for programming models interrupt or DMA transfer, refer to
other examples); 
This example is based on the STM32F4xx ADC LL API; 
peripheral initialization done using LL unitary services functions
for optimization purpose (performance and size).

Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from SW trigger.
Sequencer of group regular (default group available on ADC of all STM32 devices)
is configured to convert 1 channel: internal channel temperature sensor.

Example execution:
At each press on User Button, the ADC performs 1 conversion of the selected channel.
Software polls for conversion completion.
When conversion is completed, main program reads conversion data
from ADC data register and stores it into a variable, LED2 is turned on.

ADC conversions raw data are computed to physical values
using LL ADC driver helper macro:
 - Value of temperature (unit: degree Celsius)

For debug: variables to monitor with debugger watch window:
 - "uhADCxConvertedData": ADC group regular conversion data
 - "hADCxConvertedData_Temperature_DegreeCelsius": Value of temperature (unit: degree Celsius)

Connection needed:
None.

Other peripherals used:
  1 GPIO for User Button
  1 GPIO for LED2

@par Keywords

Analog, ADC, Temperature Sensor, Single Conversion, Polling, Software trigger, Signal measurement,

@par Directory contents 

  - ADC/ADC_TemperatureSensor/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - ADC/ADC_TemperatureSensor/Inc/main.h                  Header for main.c module
  - ADC/ADC_TemperatureSensor/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_TemperatureSensor/Src/stm32f4xx_it.c          Interrupt handlers
  - ADC/ADC_TemperatureSensor/Src/main.c                  Main program
  - ADC/ADC_TemperatureSensor/Src/system_stm32f4xx.c      STM32F4xx system source file


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
