/**
  @page USART_HardwareFlowControl USART example (IT Mode with HW Flow Control)
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/USART/USART_HardwareFlowControl/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USART_HardwareFlowControl example.
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

This example shows how to configure GPIO and USART peripheral
to receive characters asynchronously from HyperTerminal (PC) in Interrupt mode
with Hardware Flow Control feature enabled. This example is based on STM32F4xx
USART LL API. Peripheral initialization is done using LL unitary services functions
for optimization purpose (performance and size).

USART Peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
HW flow control is used.
GPIO associated to User push-button is linked with EXTI. 

Example execution:
After startup from reset and system configuration, LED2 is blinking quickly.
On press on push-button , USART TX Empty interrupt is enabled.
First character of buffer to be transmitted is written into USART Transmit Data Register (DR) 
in order to initialise transfer procedure.
When character is sent from DR, a TXE interrupt occurs. 
USART IRQ Handler routine is sending next character on USART Tx line. 
IT will be raised until last byte is to be transmitted : Then, Transmit Complete (TC) interrupt is enabled 
instead of TX Empty (TXE). 
When character is received on USART Rx line, a RXNE interrupt occurs. 
USART IRQ Handler routine is then checking received character value. 
On a specific value ('S' or 's'), LED2 is turned On. 
Received character is echoed on Tx line.
In case of errors, LED2 is blinking.

@par Keywords

Connectivity, USART, Asynchronous mode, RS-232, baud rate, Interrupt, HyperTerminal, Reception,
Hardware Flow Control, CTS/RTS

@par Directory contents 

  - USART/USART_HardwareFlowControl/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - USART/USART_HardwareFlowControl/Inc/main.h                  Header for main.c module
  - USART/USART_HardwareFlowControl/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_HardwareFlowControl/Src/stm32f4xx_it.c          Interrupt handlers
  - USART/USART_HardwareFlowControl/Src/main.c                  Main program
  - USART/USART_HardwareFlowControl/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F411xx devices.

  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F411RE Set-up
    GPIOs connected to USART1 TX/RX/CTS/RTS signals should be wired
    to corresponding pins on PC UART (could be done through a USB to UART adapter).
    - Connect STM32 MCU board USART1 TX pin (GPIO PA9 connected to pin 21 of CN10 connector)
      to PC COM port RX signal
    - Connect STM32 MCU board USART1 RX pin (GPIO PA10 connected to pin 33 of CN10 connector)
      to PC COM port TX signal
    - Connect STM32 MCU board USART1 RTS pin (GPIO PA12 connected to pin 12 of CN10 connector)
      to PC COM port CTS signal
    - Connect STM32 MCU board USART1 CTS pin (GPIO PA11 connected to pin 14 of CN10 connector)
      to PC COM port RTS signal
    - Connect STM32 MCU board GND to PC COM port GND signal

  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration 
    (115200 bauds, 8 bits data, 1 stop bit, no parity, HW flow control enabled).

  - Launch the program. Enter characters on PC communication SW side.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
