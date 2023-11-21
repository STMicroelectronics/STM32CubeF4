/**
  @page LwIP_TCP_Echo_Client LwIP TCP Echo Client Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LwIP/LwIP_TCP_Echo_Client/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LwIP TCP Echo Client Application.
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

This application guides STM32Cube HAL API users to run TCP Echo Client application 
based on Raw API of LwIP TCP/IP stack

To run this application, On the remote PC, open a command prompt window.
(In Windows, select Start > All Programs > Accessories > Command Prompt.)
At the command prompt, enter:
    C:\>echotool /p tcp /s
where:
    - /p transport layer protocol used for communication (TCP)
    - /s is the actual mode of connection (Server mode)
    
Each time the user pushes the KEY button of the STM324x9I-EVAL board, an echo 
request is sent to the server

If the LCD is used (#define USE_LCD in main.h), log messages will be displayed 
to inform user about ethernet cable status and the IP address value, else this 
will be ensured by LEDs:
  + LED1: ethernet cable is connected.
  + LED2: ethernet cable is not connected.

If a DHCP server is available, a dynamic IP address can be allocated by enabling 
the DHCP process (#define LWIP_DHCP in lwipopts.h).

If a DHCP server is not available, after timeout connection, the device only gets a static 
IP address(the switch from static to dynamic IP address is not available in this application).

If DHCP is used, user should define its PC Host IP address ( DEST_IP_ADDR0 [0..3] in main.h file.

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

Connectivity, LwIP, Ethernet, TCP/IP, FreeRTOS, DHCP, echo client,

@par Directory contents 

  - LwIP/LwIP_TCP_Echo_Client/Inc/app_ethernet.h          header of app_ethernet.c file
  - LwIP/LwIP_TCP_Echo_Client/Inc/ethernetif.h            header for ethernetif.c file
  - LwIP/LwIP_TCP_Echo_Client/Inc/lcd_log_conf.h          LCD Log configuration file
  - LwIP/LwIP_TCP_Echo_Client/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - LwIP/LwIP_TCP_Echo_Client/Inc/stm32f4xx_it.h          STM32 interrupt handlers header file
  - LwIP/LwIP_TCP_Echo_Client/Inc/main.h                  Main program header file
  - LwIP/LwIP_TCP_Echo_Client/Inc/lwipopts.h              LwIP stack configuration options
  - LwIP/LwIP_TCP_Echo_Client/Inc/tcp_echoclient.h        Header for tcp echoclient application
  - LwIP/LwIP_TCP_Echo_Client/Src/app_ethernet.c          Ethernet specific module
  - LwIP/LwIP_TCP_Echo_Client/Src/stm32f4xx_it.c          STM32 interrupt handlers
  - LwIP/LwIP_TCP_Echo_Client/Src/main.c                  Main program
  - LwIP/LwIP_TCP_Echo_Client/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - LwIP/LwIP_TCP_Echo_Client/Src/ethernetif.c            Interfacing LwIP to ETH driver
  - LwIP/LwIP_TCP_Echo_Client/Src/tcp_echoclient.c        tcp echoclient application


@par Hardware and Software environment

  - This application runs on STM32F429x/STM32F439x Devices.
    
  - This application has been tested with the following environments:
     - STM324x9I-EVAL board   
     - echotool: (http://bansky.net/echotool/) is used as echo server that sends
       back every incoming data.
      
  - STM324x9I-EVAL Set-up
    - Connect the eval board to remote PC (through a crossover ethernet cable)
      or to your local network (through a straight ethernet cable)
  
  - Remote PC Set-up
    - Configure a static IP address for your remote PC 
      this address must be 192.168.0.11 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
