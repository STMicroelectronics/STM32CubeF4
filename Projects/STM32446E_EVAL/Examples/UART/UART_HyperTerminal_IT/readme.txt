/**
  @page UART_Hyperterminal_IT UART Hyperterminal IT example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    UART/UART_Hyperterminal_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the UART Hyperterminal example.
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

This example describes an UART transmission (transmit/receive) between a board and 
an Hyperterminal PC application by using an interrupt.

Board: STM32446E-EVAL
Tx Pin: PA.09
Rx Pin: PA.10
   _________________________ 
  |           ______________|                       _______________
  |          |USART1        |                      | HyperTerminal |
  |          |              |                      |               |
  |          |           TX |______________________|RX             |
  |          |              |                      |               |
  |          |              |     RS232 Cable      |               |             
  |          |              |                      |               |
  |          |           RX |______________________|TX             |          
  |          |              |                      |               |           
  |          |______________|                      |_______________|          
  |                         |                       
  |                         |                    
  |                         |                      
  |                         |                      
  |_STM32_Board_____________|                      

This example guides you through the different configuration steps by mean of HAL API 
to ensure UART Data buffer transmission and reception with Interrupt.
The communication is done with the Hyperterminal PC application.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz for STM32F4xx Devices.

The UART peripheral configuration is ensured by the HAL_UART_Init() function.
This later is calling the HAL_UART_MspInit()function which core is implementing
the configuration of the needed UART resources according to the used hardware.
You may update this function to change UART configuration.

The UART/Hyperterminal communication is then initiated.
The HAL_UART_Receive_IT() and the HAL_UART_Transmit_IT() functions allow respectively 
the reception of Data from Hyperterminal and the transmission of a predefined data 
buffer.

The Asynchronous communication aspect of the UART is clearly highlighted as the  
data buffers transmission/reception to/from Hyperterminal are done simultaneously.

For this example the TxBuffer is predefined and the RxBuffer size is limited to 
10 data by the mean of the RXBUFFERSIZE define in the main.c file.

In a first step the received data will be stored in the RxBuffer buffer and the 
TxBuffer buffer content will be displayed in the Hyperterminal interface.
In a second step the received data in the RxBuffer buffer will be sent back to 
Hyperterminal and displayed.
The end of this two steps are monitored through the HAL_UART_GetState() function
result.

STM32 Eval boards LEDs can be used to monitor the transfer status:
 - LED1 toggles when the transmission process is complete.
 - LED1 stays ON when the reception process is complete.
 - LED3 is ON when there is an error in transmission/reception process.  
 - LED3 toggles when there another error is detected.  

The UART is configured as follows:
    - BaudRate = 9600 baud  
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - One Stop Bit
    - Odd parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

@note USARTx/UARTx instance used and associated resources can be updated in "main.h"
      file depending hardware configuration used.

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

Connectivity, UART, Printf, Baud rate, RS-232, HyperTerminal, full-duplex, HyperTerminal, DMA,
Transmission, Reception, Asynchronous

@par Directory contents 

  - UART/UART_Hyperterminal_IT/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - UART/UART_Hyperterminal_IT/Inc/stm32f4xx_it.h          IT interrupt handlers header file
  - UART/UART_Hyperterminal_IT/Inc/main.h                  Main program header file  
  - UART/UART_Hyperterminal_IT/Src/stm32f4xx_it.c          IT interrupt handlers
  - UART/UART_Hyperterminal_IT/Src/main.c                  Main program
  - UART/UART_Hyperterminal_IT/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - UART/UART_Hyperterminal_IT/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with STMicroelectronics STM32446E-EVAL board and can be
    easily tailored to any other supported device and development board.    
      
  - STM32446E-EVAL Set-up
    - Connect a null-modem female/female RS232 cable between the DB9 connector
      CN25 (USART1) and PC serial port if you want to display data on the HyperTerminal.
      @note 
         - Make sure that jumper JP4 pin1 (PA10) and pin2 (uart1_rx) are connected.   
         - Make sure that jumper JP6 pin2 (uart1_tx) and pin3 (rs232) are connected.   
         - Make sure that jumper JP7 pin1 (uart1_tx) and pin2 (PA9) are connected.   
         - Make sure that jumper JP8 pin1 (PA10) and pin2 (rs232) are connected.   
          
  - Hyperterminal configuration:
    - Word Length = 7 Bits
    - One Stop Bit
    - Odd parity
    - BaudRate = 9600 baud
    - flow control: None 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
