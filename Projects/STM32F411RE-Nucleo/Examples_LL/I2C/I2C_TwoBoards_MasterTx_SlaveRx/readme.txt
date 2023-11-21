/**
  @page I2C_TwoBoards_MasterTx_SlaveRx I2C example (Polling Mode)
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/I2C/I2C_TwoBoards_MasterTx_SlaveRx/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C_TwoBoards_MasterTx_SlaveRx I2C example (Polling Mode).
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

This example describes how to transmit data bytes from an I2C Master device using Polling mode
to an I2C Slave device using Interrupt mode. Peripheral initialization is done using
LL unitary services functions for optimization purpose (performance and size).

This example guides you through the different configuration steps by mean of LL API
to configure GPIO and I2C peripherals using two NUCLEO-F411RE.

Boards: NUCLEO-F411RE (embeds a STM32F411xx device)
SCL Pin: PB.8 (CN10, pin 3)
SDA Pin: PB.9 (CN10, pin 5)

   ______BOARD SLAVE_____                       _____BOARD MASTER_____
  |        ______________|                     |______________        |
  |       |I2C1          |                     |          I2C1|       |
  |       |              |                     |              |       |
  |       |          SCL |_____________________| SCL          |       |
  |       |              |                     |              |       |
  |       |              |                     |              |       |
  |       |          SDA |_____________________| SDA          |       |
  |       |______________|                     |______________|       |
  |         __           |                     |             __       |
  |        |__|          |                     |            |__|      |
  |        USER       GND|_____________________|GND         USER      |
  |___STM32F4xx-Nucleo___|                     |___STM32F4xx-Nucleo___|

The project is split in two parts the Master Board and the Slave Board
- Master Board
  I2C1 Peripheral is configured in Master mode (Clock 400Khz).
  And GPIO associated to User push-button is linked with EXTI. 
- Slave Board
  I2C1 Peripheral is configured in Slave mode (Clock 400Khz, Own address 7-bit enabled).

The user can choose between Master and Slave through "#define SLAVE_BOARD"
in the "main.h" file:
- Comment "#define SLAVE_BOARD" to select Master board.
- Uncomment "#define SLAVE_BOARD" to select Slave board.

A first program launch, BOARD SLAVE waiting Address Match code through Handle_I2C_Slave() routine.
LED2 blinks quickly on both BOARD to wait for user-button press. 

Example execution:
To perform this example, two steps are necessary to prevent a wrong start/stop detection due to the initialization of the opposite GPIO board.

So at first step, press the User push-button on BOARD SLAVE to activate Slave peripheral and then prepare acknowledge for Slave address reception.
At second step, press the User push-button on BOARD MASTER to activate Master peripheral and then to initiate a write request by Master.
This action will generate an I2C start condition with the Slave address and a write bit condition.
When address Slave match code is received on I2C1 of BOARD SLAVE, an ADDR event occurs.
Handle_I2C_Slave() routine is then checking the direction Read (mean write direction for Master).
This will allow Slave to enter in receiver mode and then acknowledge Master to send the bytes.
When acknowledge is received on I2C1 (Master), a TXE event occurs.
This will allow Master to transmit a byte to the Slave.
Each time a byte is received on I2C1 (Slave), an RXNE or BTF event occurs until a STOP condition.
And so each time the Slave acknowledge the byte received a TXE event occurs on Master side.
Master generate a Stop condition when size of data to transmit is achieved.

The STOP condition generate a STOP event and initiate the end of reception on Slave side.
Handle_I2C_Slave() routine is then clearing the STOP flag.

LED2 turn OFF on BOARD SLAVE when pressing the User push-button.

LED2 is On :
- Slave side if data are well received.
- Master side if transfer sequence is completed.

In case of errors, LED2 is blinking.

@par Keywords

Connectivity, Communication, I2C, Master Rx, Slave Tx, Transmission, Reception, Fast mode, SDA, SCL

@par Directory contents 

  - I2C/I2C_TwoBoards_MasterTx_SlaveRx/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - I2C/I2C_TwoBoards_MasterTx_SlaveRx/Inc/main.h                  Header for main.c module
  - I2C/I2C_TwoBoards_MasterTx_SlaveRx/Inc/stm32_assert.h          Template file to include assert_failed function
  - I2C/I2C_TwoBoards_MasterTx_SlaveRx/Src/stm32f4xx_it.c          Interrupt handlers
  - I2C/I2C_TwoBoards_MasterTx_SlaveRx/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_MasterTx_SlaveRx/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F411RE Set-up
    - Connect GPIOs connected to Board Slave I2C1 SCL/SDA (PB.8 and PB.9)
    to respectively Board Master SCL and SDA pins of I2C1 (PB.8 and PB.9).
      - I2C1_SCL  PB.8 (CN10, pin 3) : connected to I2C1_SCL PB.8 (CN10, pin 3) 
      - I2C1_SDA  PB.9 (CN10, pin 5) : connected to I2C1_SDA PB.9 (CN10, pin 5)
    - Connect Master board GND to Slave Board GND

  - Launch the program. Press User push-button on Slave BOARD then on Master BOARD
      to initiate a write request by Master then Slave receive bytes.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory (The user can choose between Master 
   and Slave target through "#define SLAVE_BOARD" in the "main.h" file)
    o Comment "#define SLAVE_BOARD" and load the project in Master Board
    o Uncomment "#define SLAVE_BOARD" and load the project in Slave Board
 - Run the example


 */
