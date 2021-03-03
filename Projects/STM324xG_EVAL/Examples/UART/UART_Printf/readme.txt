/**
  @page UART_Printf UART Printf example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    UART/UART_Printf/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the UART Printf example.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
   @endverbatim

@par Example Description 

This example shows how to re-route the C library printf function to the UART.
The UART outputs a message on the HyperTerminal.

   _________________________ 
  |           ______________|                       _______________
  |          |USART3        |                      | HyperTerminal |
  |          |              |                      |               |
  |          |      TX(PC10)|______________________|RX             |
  |          |              |                      |               |
  |          |              |     RS232 Cable      |               |             
  |          |              |                      |               |
  |          |      RX(PC11)|______________________|TX             |          
  |          |              |                      |               |           
  |          |______________|                      |_______________|          
  |                         |                       
  |                         |                    
  |                         |                      
  |                         |                      
  |_STM324xG________________|                      

This implementation output the printf message on the Hyperterminal using USART3.

The USART3 is configured as follows:
    - BaudRate = 9600 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - ODD parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, UART, Printf, Baud rate, RS-232, HyperTerminal, full-duplex

@par Directory contents 

  - UART/UART_Printf/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - UART/UART_Printf/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - UART/UART_Printf/Inc/main.h                  Main program header file  
  - UART/UART_Printf/Src/stm32f4xx_it.c          Interrupt handlers
  - UART/UART_Printf/Src/main.c                  Main program
  - UART/UART_Printf/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - UART/UART_Printf/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment  
  - This example runs on STM32F407xx/417xx devices.    
  - This example has been tested with STMicroelectronics STM324xG-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM324xG-EVAL RevC Set-up
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN16 (USART3) and PC serial port if you want to display data on the HyperTerminal.
    - Please ensure that jumper JP22 is in position RS232.
     
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

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
