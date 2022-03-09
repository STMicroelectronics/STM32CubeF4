/**
  @page USART_SyncCommunication_FullDuplex_IT USART Synchronous Transmitter/Receiver example (IT mode)
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/USART/USART_SyncCommunication_FullDuplex_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USART_SyncCommunication_FullDuplex_IT example.
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

This example shows how to configure GPIO, USART, DMA and SPI peripherals for transmitting
bytes from/to an USART peripheral to/from an SPI peripheral (in slave mode) by using Interrupt
mode through the STM32F4xx USART LL API (SPI is using DMA for 
receiving/transmitting characters sent from/received by USART). Peripheral initialization is
done using LL unitary services functions for optimization purpose (performance and size).

This example works with only one NUCLEO-F411RE.

USART1 Peripheral is acts as Master using IT.
SPI1 Peripheral is configured in Slave mode using DMA.
GPIO associated to User push-button is linked with EXTI. 

Example execution:

LED2 is blinking Fast (200ms) and wait User push-button action.
Press User push-button on BOARD start a Full-Duplex communication from USART using IT.
On USART1 side, Clock will be generated on SCK line, Transmission done on TX, reception on RX.
On SPI1 side, reception is done through the MOSI Line, transmission on MISO line.

At end of transmission, both received buffers are compared to expected ones.
In case of both transfers successfully completed, LED2 is turned on.
In case of errors, LED2 is blinking Slowly (1s).

@par Keywords

Connectivity, USART, Synchronous mode, RS-232, baud rate, Interrupt, SPI, Reception, Transmission, Full Duplex

@par Directory contents 

  - USART/USART_SyncCommunication_FullDuplex_IT/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - USART/USART_SyncCommunication_FullDuplex_IT/Inc/main.h                  Header for main.c module
  - USART/USART_SyncCommunication_FullDuplex_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_SyncCommunication_FullDuplex_IT/Src/stm32f4xx_it.c          Interrupt handlers
  - USART/USART_SyncCommunication_FullDuplex_IT/Src/main.c                  Main program
  - USART/USART_SyncCommunication_FullDuplex_IT/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F411xx devices.

  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F411RE Set-up
    - Connect USART1 SCK PA.08 to Slave SPI1 SCK PB.03
    - Connect USART1 TX PA.09 to Slave SPI1 MOSI PB.05
    - Connect USART1 RX PA.10 to Slave SPI1 MISO PB.04

Board connector:
  PA.08 :   connected to pin 23 of CN10 connector
  PA.09 :   connected to pin 21 of CN10 connector
  PA.10 :   connected to pin 33 of CN10 connector
  PB.03 :   connected to pin 31 of CN10 connector
  PB.05 :   connected to pin 29 of CN10 connector
  PB.04 :   connected to pin 27 of CN10 connector

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 - Launch the program. Press on User push-button on board to initiate data transfers.


 */
