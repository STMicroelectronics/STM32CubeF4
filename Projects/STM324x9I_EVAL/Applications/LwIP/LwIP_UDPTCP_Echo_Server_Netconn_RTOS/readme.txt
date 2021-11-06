/**
  @page LwIP_UDPTCP_Echo_Server_Netconn_RTOS LwIP UDP/TCP Echo Server Netconn Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LwIP/LwIP_UDPTCP_Echo_Server_Netconn_RTOS/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LwIP UDP/TCP Echo Server Netconn API Application.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
   @endverbatim

@par Application Description 

This application guides STM32Cube HAL API users to run a UDP/TCP Echo Server application 
based on Netconn API of LwIP TCP/IP stack

To run this application, On the remote PC, open a command prompt window.
(In Windows, select Start > All Programs > Accessories > Command Prompt.)
At the command prompt, enter:
  C:\>echotool IP_address /p tcp /r 7 /n 15 /t 2 /d Testing LwIP TCP echo server
or
  C:\>echotool IP_address /p udp /r 7 /l 7 /n 15 /t 2 /d Testing LwIP UDP echo server
where:
    – IP_address is the actual board’s IP address. By default, the following 
    static IP address is used: 192.168.0.10
    – /p transport layer protocol used for communication (TCP or UDP)
    – /r is the actual remote port on the echo server (echo port)
    - /l is the actual local port for the client (echo port)
    – /n is the number of echo requests (for example, 15)
    – /t is the connection timeout in seconds (for example, 2)
    – /d is the message to be sent for echo 

If the LCD is used (#define USE_LCD in main.h), log messages will be displayed 
to inform user about ethernet cable status and the IP address value, else this 
will be ensured by LEDs:
  + LED1: ethernet cable is connected.
  + LED2: ethernet cable is not connected.

If a DHCP server is available, a dynamic IP address can be allocated by enabling 
the DHCP process (#define USE_DHCP in main.h)

Note: In this application the Ethernet Link ISR need the HAL time base to configure 
the Ethernet MAC, so the Ethernet Link interrupt priority must be set lower (numerically greater) 
than the HAL tick interrupt priority to ensure that the System tick increments while executing 
the Ethernet Link ISR.

Note: By default, the Ethernet Half duplex mode is not supported in the 
STM324x9I-EVAL board, for more information refer to the HAL_ETH_MspInit() 
function in the ethernetif.c file

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond to have correct 
      HAL operation.
      
For more details about this application, refer to UM1713 "STM32Cube interfacing with LwIP and applications"

@par Keywords

Connectivity, LwIP, Ethernet, UDP, Echo Server, TCP/IP, DHCP, Netconn, FreeRTOS, RTOS

@par Directory contents 

  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/app_ethernet.h                  header of app_ethernet.c file
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/ethernetif.h                    header for ethernetif.c file
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/lcd_log_conf.h                  LCD Log configuration file
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/stm32f4xx_hal_conf.h            HAL configuration file
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/stm32f4xx_it.h                  STM32 interrupt handlers header file
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/main.h                          Main program header file
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/lwipopts.h                      LwIP stack configuration options
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/FreeRTOSConfig.h                FreeRTOS configuration options
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/stm32f4xx_it.c                  STM32 interrupt handlers
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/app_ethernet.c                  Ethernet specific module
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/main.c                          Main program
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/system_stm32f4xx.c              STM32F4xx system clock configuration file
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/ethernetif.c                    Interfacing LwIP to ETH driver
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/tcpecho.c                       tcp echo server main thread
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/udpecho.c                       udp echo server main thread
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/stm32f4xx_hal_timebase_tim.c    HAL time base based on the hardware TIM


@par Hardware and Software environment

  - This application runs on STM32F429x/STM32F439x Devices.
    
  - This application has been tested with the following environments:
     - STM324x9I-EVAL board   
     - echotool: (http://bansky.net/echotool/) is used as echo client that sends
       data to the server and checking whether they came back    
      
  - STM324x9I-EVAL Set-up
    - Connect the eval board to remote PC (through a crossover ethernet cable)
      or to your local network (through a straight ethernet cable)
  
  - Remote PC Set-up
    - Configure a static IP address for your remote PC 
      for example 192.168.0.11 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
