/**
  @page I2C_OneBoard_Communication_DMAAndIT I2C (Master DMA Mode)
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/I2C/I2C_OneBoard_Communication_DMAAndIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C_OneBoard_Communication_DMAAndIT I2C example (Master DMA Mode).
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

This example describes how to transmit data bytes from an I2C Master device using DMA mode
to an I2C Slave device using Interrupt mode. Peripheral initialization is done using
LL unitary services functions for optimization purpose (performance and size).

This example guides you through the different configuration steps by mean of LL API
to configure GPIO, DMA and I2C peripherals using only one NUCLEO-F411RE.

I2C1 Peripheral is configured in Slave mode with EXTI (Clock 400Khz, Own address 7-bit enabled).
I2C2 Peripheral is configured in Master mode with DMA (Clock 400Khz).
GPIO associated to User push-button is linked with EXTI. 

LED2 blinks quickly to wait for user-button press. 

Example execution:
Press the User push-button to initiate a Start condition by Master.
This action will generate an I2C start condition on the I2C bus.
When the I2C start condition is sent on I2C2, a SB interrupt occurs.
I2C2 IRQ Handler routine is then sending the Slave address with a write bit condition.
When address Slave match code is received on I2C1, the Slave acknowledge the address.
When this acknowledge is received on I2C2, an ADDR interrupt occurs.
I2C2 IRQ Handler routine is then enable the DMA transfer of the data from flash memory buffer to I2C2 DR register.

When address Slave match code is received on I2C1, an ADDR interrupt occurs.
I2C1 IRQ Handler routine is then checking Address Match Code and direction at Read (mean write direction for Master).
This will allow Slave to enter in receiver mode and then acknowledge Master to send the bytes through DMA.
Each time a byte is received on I2C1 (Slave), an RXNE interrupt occurs until a STOP condition.

And so each time the Slave acknowledge the byte received, DMA transfer the next data from flash memory buffer to I2C2 DR register
until Transfer completed.
Then Master generate a Stop condition when DMA transfer is achieved.

The STOP condition generate a STOP interrupt and initiate the end of reception on Slave side.
I2C1 IRQ handler routine is then clearing the STOP flag.

LED2 is On if data are well received.

In case of errors, LED2 is blinking.

@par Keywords 

Connectivity, Communication, I2C, DMA, Interrupt, Master, Slave, Transmission, Reception, Fast mode

@par Directory contents 

  - I2C/I2C_OneBoard_Communication_DMAAndIT/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - I2C/I2C_OneBoard_Communication_DMAAndIT/Inc/main.h                  Header for main.c module
  - I2C/I2C_OneBoard_Communication_DMAAndIT/Inc/stm32_assert.h          Template file to include assert_failed function
  - I2C/I2C_OneBoard_Communication_DMAAndIT/Src/stm32f4xx_it.c          Interrupt handlers
  - I2C/I2C_OneBoard_Communication_DMAAndIT/Src/main.c                  Main program
  - I2C/I2C_OneBoard_Communication_DMAAndIT/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F411RE Set-up
    - Connect GPIOs connected to I2C1 SCL/SDA (PB.6 and PB.7)
    to respectively SCL and SDA pins of I2C2 (PB.10 and PB.3).
      - I2C1_SCL  PB.6 (CN10, pin 17) : connected to I2C2_SCL PB.10 (CN10, pin 25) 
      - I2C1_SDA  PB.7 (CN7, pin 21) : connected to I2C2_SDA PB.3 (CN10, pin 31)

  - Launch the program. Press User push-button to initiate a write request by Master 
      then Slave receive bytes.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
