/**
  @page SPI_TwoBoards_FullDuplex_DMA SPI example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/SPI/SPI_TwoBoards_FullDuplex_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI_TwoBoards_FullDuplex_DMA example.
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

This example shows how to ensure SPI data buffer transmission and reception in
DMA mode. The example is based on the STM32F4xx SPI LL API. Peripheral initialization
is done using LL unitary services functions for optimization purpose
(performance and size).

The communication is done with 2 boards through SPI.
   _________________________                        _________________________
  |       ___________ ______|                      |__________________       |
  |      |SPI1              |                      |             SPI1 |      |
  |      |                  |                      |                  |      |
  |      |         CLK(PB3) |______________________|(PB3)CLK          |      |
  |      |                  |                      |                  |      |
  |      |         MISO(PB4)|______________________|(PB4)MISO         |      |
  |      |                  |                      |                  |      |
  |      |         MOSI(PB5)|______________________|(PB5)MOSI         |      |
  |      |                  |                      |                  |      |
  |      |__________________|                      |__________________|      |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32F4xx Master _______|                      |_STM32F4xx Slave ________|

This example shows how to configure GPIO and SPI peripherals
to use a Full-Duplex communication using DMA Transfer mode through the STM32F4xx SPI LL API.

The project is split in two parts the Master Board and the Slave Board:

- Master Board
  SPI1 Peripheral is configured in Master mode.
  DMA2_Stream2 and DMA2_Stream3 configured to transfer Data via SPI peripheral
  GPIO associated to User push-button is linked with EXTI. 
  
- Slave Board
  SPI1 Peripheral is configured in Slave mode.
  DMA2_Stream2 and DMA2_Stream3 configured to transfer Data via SPI peripheral

The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:
- Uncomment "#define MASTER_BOARD" to select Master board.
- Comment "#define MASTER_BOARD" to select Slave board.

Example execution:
On BOARD MASTER, LED2 is blinking Fast (200ms) and wait User push-button action.
Press User push-button on BOARD MASTER start a Full-Duplex communication through DMA.
On MASTER side, Clock will be generated on SCK line, Transmission(MOSI Line) and reception (MISO Line) 
will be done at the same time. 
SLAVE SPI will received  the Clock (SCK Line), so Transmission(MISO Line) and reception (MOSI Line) will be done also.

LED2 is On if data is well received.

In case of errors, LED2 is blinking Slowly (1s).

@note You need to perform a reset on Master board, then perform it on Slave board
      to have the correct behaviour of this example.


@par Keywords

Connectivity, SPI, DMA, Full-duplex, Advanced communication, Transmission, Reception, Master, Slave, MISO, MOSI

@par Directory contents 

  - SPI/SPI_TwoBoards_FullDuplex_DMA/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Inc/main.h                  Header for main.c module
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Src/stm32f4xx_it.c          Interrupt handlers
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Src/main.c                  Main program
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F411xx devices.

  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F411RE Set-up
    - Connect Master board PB3 to Slave Board PB3 (connected to pin 31 of CN10 connector)
    - Connect Master board PB4 to Slave Board PB4 (connected to pin 27 of CN10 connector)
    - Connect Master board PB5 to Slave Board PB5 (connected to pin 29 of CN7 connector)
    - Connect Master board GND to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
