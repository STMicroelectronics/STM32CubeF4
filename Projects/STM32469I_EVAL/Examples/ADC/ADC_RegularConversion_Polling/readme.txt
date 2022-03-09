/**
  @page ADC_RegularConversion_Polling conversion using Polling

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    ADC/ADC_RegularConversion_Polling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC RegularConversion Polling example.
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

How to use the ADC in Polling mode to convert data through the HAL API.

When the end of conversion occurs, the converted data of ADC3 DR register is 
affected to the uhADCxConvertedValue variable.

Conversion time (reference manual, "Reset and clock control" and "ADC clocks" sections):
  1) the system clock is 180 MHz. APB2 = 90MHz and ADC clock = APB2/4
  2) __HAL_RCC_ADC_CONFIG() macro in HAL_ADC_MspInit() API sets the system clock as ADC asynchronous clock source 
  2) ClockPrescaler field of initialization structure is set to ADC_CLOCKPRESCALER_PCLK_DIV2
     => the input ADC clock is set in asynchronous clock mode   
     => the input ADC clock is the undivided system clock 


  Sampling time is set to ADC_SAMPLETIME_6CYCLES_5 (6.5 cycles).
  ConvTime = Sampling time + 12.5 ADC clock cycles.
           = 19 clock cycles
           = (19 / 22.5 MHz) = 844.4 ns 
                    

Please select PC0 option on JP3 jumper called Potentiometer
User can vary the ADC_CHANNEL_10 voltage using the Eval Board potentiometer (RV1) connected to PC.0.
The converted value is monitored through debugger: uhADCxConvertedValue variable.

STM32 Eval boards LEDs can be used to monitor the transfer status:
  - LED3 is ON when there is an error in initialization.

@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

Analog, ADC, Analog to Digital Converter, Regular Conversion, Polling, Single Conversion

@par Directory contents 

  - ADC/ADC_RegularConversion_Polling/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_RegularConversion_Polling/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - ADC/ADC_RegularConversion_Polling/Inc/main.h                  Header for main.c module  
  - ADC/ADC_RegularConversion_Polling/Src/stm32f4xx_it.c          Interrupt handlers
  - ADC/ADC_RegularConversion_Polling/Src/main.c                  Main program
  - ADC/ADC_RegularConversion_Polling/Src/stm32f4xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_RegularConversion_Polling/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F469xx/STM32F479xx devices.
  
  - This example has been tested with STM32469I-EVAL RevC board and can be
    easily tailored to any other supported device and development board.

  - STM32469I-EVAL RevC Set-up
    - Use the Potentiometer (RV1) of the Eval board (connected to  PC.0).
    - Make sure that JP3 is fitted in 2-3 positions to use potentiometer.
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
