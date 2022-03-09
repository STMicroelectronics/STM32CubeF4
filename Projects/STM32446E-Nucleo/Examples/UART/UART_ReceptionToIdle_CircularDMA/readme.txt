/**
  @page UART_ReceptionToIdle_CircularDMA UART Reception to IDLE event example

  @verbatim
  ******************************************************************************
  * @file    UART/UART_ReceptionToIdle_CircularDMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the UART Reception To IDLE Event using DMA example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to use the HAL UART API for reception to IDLE event in circular DMA mode.


Board: NUCLEO-F446RE (embeds a STM32F446RET6 device)
UART Instance : USART2
Tx Pin: PA2 (pin 35 in CN12 connector)
Rx Pin: PA3 (pin 37 in CN12 connector)
   _________________________
  |           ______________|                       _______________
  |          |USART         |                      | HyperTerminal |
  |          |              |                      |               |
  |          |           TX |______________________|RX             |
  |          |              |                      |               |
  |          |              |    ST-Link Cable     |               |
  |          |              |                      |               |
  |          |           RX |______________________|TX             |
  |          |              |                      |               |
  |          |______________|                      |_______________|
  |                         |
  |                         |
  |                         |
  |                         |
  |_STM32_Board_____________|

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz for STM32F4xx Devices.

The UART peripheral configuration is ensured by the HAL_UART_Init() function.
This later is calling the HAL_UART_MspInit() function which core is implementing
the configuration of the needed UART resources according to the used hardware (CLOCK,
GPIO, DMA and NVIC). You may update this function to change UART configuration.
In this example, a DMA channel is associated to USART2 RX (Circular mode).

The UART/Hyperterminal communication is then initiated (Board is sending an information message,
inviting user to enter characters on Terminal).

Application then starts the Reception To Idle process, using dedicated HAL UART API :
    - HAL_UARTEx_ReceiveToIdle_DMA()

The HAL_UARTEx_ReceiveToIdle_DMA() function allows to handle reception of Data from Hyperterminal
using DMA and notify application of already received data while the reception is still ongoing.
Received characters are handled by DMA and are stored in the user aRXBufferUser buffer.
Notifications to application, that some data are available in reception buffer, are done
through the execution of a user callback : HAL_UARTEx_RxEventCallback().
This callback will be executed when any of following events occurs :
    - HT (Half Transfer) : Half of Rx buffer is filled)
    - TC (Transfer Complete) : Rx buffer is full.
      (In case of Circular DMA, reception could go on, and next reception data will be stored
      in index 0 of reception buffer by DMA).
    - Idle Event on Rx line : Triggered when RX line has been in idle state (normally high state)
      for 1 frame time, after last received byte.

When any of the HT, TC or Idle event occurs, HAL_UARTEx_RxEventCallback() is called,
and provides (as callback parameter) the index in user buffer until which, received data have been stored.

Example : case of a reception of 22 bytes before Idle event occurs, using Circular DMA and a Rx buffer
of size of 20 bytes.
    - User calls HAL_UARTEx_ReceiveToIdle_DMA() providing buffer address and buffer size (20)
    - HAL_UARTEx_RxEventCallback() will be executed on HT DMA event with Size = 10
      Data in user Rx buffer could be retrieved by application from index 0 to 9
    - HAL_UARTEx_RxEventCallback() will be executed on TC DMA event with Size = 20
      New data in user Rx buffer could be retrieved by application from index 10 to 19
    - HAL_UARTEx_RxEventCallback() will be executed after IDLE event occurs with Size = 2
      New data in user Rx buffer could be retrieved by application from index 0 to 1

In this example, provided HAL_UARTEx_RxEventCallback callback implementation example, aims to
echo received characters to HyperTerminal.

The UART instance (USART2) is configured as follows:
    - BaudRate = 115200 baud
    - Word Length = 8 bits (8 data bits, no parity bit)
    - One Stop Bit
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled

@note When the parity is enabled, the computed parity is inserted at the MSB
position of the transmitted data.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, UART/USART, baud rate, RS-232, full-duplex, DMA, parity, stop bit, Receiver, Asynchronous

@par Directory contents

  - UART/UART_ReceptionToIdle_CircularDMA/Inc/stm32f4xx_nucleo_conf.h     BSP configuration file
  - UART/UART_ReceptionToIdle_CircularDMA/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - UART/UART_ReceptionToIdle_CircularDMA/Inc/stm32f4xx_it.h          DMA interrupt handlers header file
  - UART/UART_ReceptionToIdle_CircularDMA/Inc/main.h                  Header for main.c module
  - UART/UART_ReceptionToIdle_CircularDMA/Src/stm32f4xx_it.c          DMA interrupt handlers
  - UART/UART_ReceptionToIdle_CircularDMA/Src/main.c                  Main program
  - UART/UART_ReceptionToIdle_CircularDMA/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - UART/UART_ReceptionToIdle_CircularDMA/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F446RETx devices.
  - This example has been tested with one NUCLEO-F446RE board embedding
    a STM32F446RETx device and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-F446RE Set-up
     - Connect ST-Link cable to the PC USB port to send/receive data between PC and board.
       A virtual COM port will then appear in the HyperTerminal.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
