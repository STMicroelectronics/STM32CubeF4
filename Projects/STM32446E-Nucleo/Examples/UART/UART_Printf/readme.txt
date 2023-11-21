/**
  @page UART_Printf UART Printf example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    UART/UART_Printf/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the UART Printf example.
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

This example shows how to re-route the C library printf function to the UART.
The UART outputs a message on the HyperTerminal.            

The STM32 Nucleo boards integrate the ST-LINK/V2.1 interfacing with the PC and 
offering the following capabilities:
  – Debug and programming port
  – Virtual com port interface
  – Mass storage interface
  – USB power management request for more than 100 mA power

Once connected to your PC via the ST-LINK port, the STM32 Nucleo board is enumerated 
as a serial communication port USART2 and listed under Ports (COM & LPT) of Windows
Device Manager.

This implementation shows how to retarget the C library printf function to the UART
in order to output the printf message on the Hyperterminal using USART2 interface 
connected to ST-LINK MCU in order to support Virtual Com Port.

The communication port USART2 is configured as follows:
    - BaudRate = 9600 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - ODD parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

To identify the Nucleo board VCP, you have just to perform the following actions on
your Windows PC: 
    - Click Start, and enter Device Manager into the Search field. Click Device Manager .
    - Double-click Ports (COM & LPT).
    - Find the Nucleo board VCP: STMicroelectronics STLink Virtual COM Port (COM..)

To receive the printf output message on the Hyperterminal, apply this configuration
on your serial terminal:       
    - Port name = COM.. (already identified on Device Manager)
    - Word Length = 7 Bits
    - One Stop Bit
    - Odd parity
    - BaudRate = 9600 baud
    - flow control: None 
    

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The clock setting is configured to have the max product performance (max clock frequency) 
      so not optimized in term of power consumption.

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

  - This example runs on STM32F446xx devices.
    
  - This example has been tested with STMicroelectronics STM32F4xx-Nucleo rev C 
    boards and can be easily tailored to any other supported device 
    and development board.

  - For This example to work properly, a 2.2 µF Capacitor must be present on the board in slot C26.

  - STM32F4xx-Nucleo Set-up    
    - Connect the Nucleo board to your PC with a USB cable type A to mini-B 
      to ST-LINK connector (CN1).
    - Please ensure that the ST-LINK connector CN2 jumpers are fitted.
        
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
