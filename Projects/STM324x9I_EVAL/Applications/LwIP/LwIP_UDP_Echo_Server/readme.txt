/**
  @page LwIP_UDP_Echo_Server LwIP UDP Echo Server Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LwIP/LwIP_UDP_Echo_Server/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LwIP UDP Echo Server Application.
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

@par Application Description 

This application guides STM32Cube HAL API users to run UDP Echo Server application 
based on Raw API of LwIP TCP/IP stack

To run this application, On the remote PC, open a command prompt window.
(In Windows, select Start > All Programs > Accessories > Command Prompt.)
At the command prompt, enter:
  C:\>echotool IP_address /p udp /r 7 /l 7 /n 15 /t 2 /d Testing LwIP UDP echo server

where:
    - IP_address is the actual board's IP address. By default, the following 
    static IP address is used: 192.168.0.10
    - /p transport layer protocol used for communication (UDP)
    - /r is the actual remote port on the echo server (echo port)
    - /l is the actual local port for the client (echo port)
    - /n is the number of echo requests (for example, 15)
    - /t is the connection timeout in seconds (for example, 2)
    - /d is the message to be sent for echo 

If the LCD is used (#define USE_LCD in main.h), log messages will be displayed 
to inform user about ethernet cable status and the IP address value, else this 
will be ensured by LEDs:
  + LED1: ethernet cable is connected.
  + LED2: ethernet cable is not connected.

If a DHCP server is available, a dynamic IP address can be allocated by enabling 
the DHCP process (#define LWIP_DHCP in lwipopts.h).

If a DHCP server is not available, after timeout connection, the device only gets a static 
IP address(the switch from static to dynamic IP address is not available in this application).

Note: In this application the Ethernet Link ISR need the HAL time base to configure 
the Ethernet MAC, so the Ethernet Link interrupt priority must be set lower (numerically greater) 
than the HAL tick interrupt priority to ensure that the System tick increments while executing 
the Ethernet Link ISR.

Note: By default, the Ethernet Half duplex mode is not supported in the 
STM324x9I-EVAL board, for more information refer to the HAL_ETH_MspInit() 
function in the ethernetif.c file

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
For more details about this application, refer to UM1713 "STM32Cube interfacing with LwIP and applications"

@par Keywords

Connectivity, LwIP, Ethernet, UDP, Echo Server, TCP/IP, DHCP

@par Directory contents 

  - LwIP/LwIP_UDP_Echo_Server/Inc/app_ethernet.h          header of app_ethernet.c file
  - LwIP/LwIP_UDP_Echo_Server/Inc/ethernetif.h            header for ethernetif.c file
  - LwIP/LwIP_UDP_Echo_Server/Inc/lcd_log_conf.h          LCD Log configuration file
  - LwIP/LwIP_UDP_Echo_Server/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - LwIP/LwIP_UDP_Echo_Server/Inc/stm32f4xx_it.h          STM32 interrupt handlers header file
  - LwIP/LwIP_UDP_Echo_Server/Inc/main.h                  Main program header file
  - LwIP/LwIP_UDP_Echo_Server/Inc/lwipopts.h              LwIP stack configuration options
  - LwIP/LwIP_UDP_Echo_Server/Inc/udp_echoserver.h        Header for UDP echoserver application
  - LwIP/LwIP_UDP_Echo_Server/Src/app_ethernet.c          Ethernet specific module
  - LwIP/LwIP_UDP_Echo_Server/Src/stm32f4xx_it.c          STM32 interrupt handlers
  - LwIP/LwIP_UDP_Echo_Server/Src/main.c                  Main program
  - LwIP/LwIP_UDP_Echo_Server/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - LwIP/LwIP_UDP_Echo_Server/Src/ethernetif.c            Interfacing LwIP to ETH driver
  - LwIP/LwIP_UDP_Echo_Server/Src/udp_echoserver.c        UDP echoserver application


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


 */
