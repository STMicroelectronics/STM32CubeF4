/**
  @page ADC_RegularConversion_DMA conversion using DMA for Data transfer

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    ADC/ADC_RegularConversion_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC RegularConversion DMA example.
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

How to use the ADC1 and DMA to transfer continuously converted data from 
ADC1 to memory.

The ADC1 is configured to convert continuously ADC_CHANNEL_1.

Each time an end of conversion occurs the DMA transfers, in circular mode, the
converted data from ADC1 DR register to the uhADCxConvertedValue variable.

In this example, the system clock is 100MHz, APB2 = 100MHz and ADC clock = APB2/4. 
Since ADC1 clock is 25 MHz and sampling time is set to 3 cycles, the conversion 
time to 12bit data is 12 cycles so the total conversion time is (12+3)/25= 0.6us(1.7Msps).

User can vary the ADC_CHANNEL_11 voltage by applying an input voltage on pin PA1 (pin A1 from connector CN9) 
(e.g. using function generator).

STM32 boards LEDs can be used to monitor the transfer status:
  - LED3 is ON when the conversion is complete.
  - LED4 is ON when there is an error in ADC initialization, in ADC channel configuration 
    or during conversion process.
The converted value is monitored through debugger: uhADCxConvertedValue variable.
The uhADCxConvertedValue read value is coded on 12 bits, the Vref+ reference voltage is connected 
on the board to VDD (+3.3V), the Vref- reference voltage is connected on the board to the ground.
To convert the read value in volts, here is the equation to apply :
Voltage = uhADCxConvertedValue * (Vref+ - Vref-) / (2^12) = uhADCxConvertedValue * 3.3 / 4096

@par Keywords

Analog, ADC, Analog to Digital Converter, Regular Conversion, DMA, Continuous Conversion

@par Directory contents 

  - ADC/ADC_RegularConversion_DMA/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_RegularConversion_DMA/Inc/stm32f4xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_RegularConversion_DMA/Inc/main.h                  Header for main.c module  
  - ADC/ADC_RegularConversion_DMA/Src/stm32f4xx_it.c          DMA interrupt handlers
  - ADC/ADC_RegularConversion_DMA/Src/main.c                  Main program
  - ADC/ADC_RegularConversion_DMA/Src/stm32f4xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_RegularConversion_DMA/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F413xx devices.
  
  - This example has been tested with STM32F413H-DISCOVERY board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
