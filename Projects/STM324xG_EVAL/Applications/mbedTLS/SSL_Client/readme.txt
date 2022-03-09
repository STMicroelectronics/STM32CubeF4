/**
  @page SSL_Client SSL Client application

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    mbedTLS/SSL_Client/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of SSL Client application.
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

This application describes how  to run an SSL client application based on mbedTLS
crypto library and LwIP TCP/IP stack on STM32F4 family.

Scenario 1:
===========
In this screnario the client (STM324xG_EVAL):
- connects to local network either through DHCP or static IP address.
- establishes a secured connection to the Host PC Server.
- sends an encrypted message to the server.
- receives an answser from the server.

Scenario 2:
===========
In this second scenario, the the client (STM324xG_EVAL):
- connects to local network either through DHCP or static IP address.
- establishes a secured connect to a second STM324xG_EVAL board running 
  the "SSL_Sever" application.
- sends an encrypted message to the second board.
- receives an answser from the server board.

Application logs are displayed on the LCD.

In case of success the green led is toggling.
In case of errors the red led is toggling.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about this application, refer to UM1723 "STM32Cube mbedTLS application".

@par Keywords

Security, mbedTLS, SSL Client, Crypto, TCP/IP, LwIP, AES 128,
AES 192, AES 256, Triple DES, MD5, SHA-1, RNG

@par Directory contents

    - mbedTLS/SSL_Client/Inc/ethernetif.h                   header for ethernetif.c file
    - mbedTLS/SSL_Client/Inc/main.h                         Main program header file 
    - mbedTLS/SSL_Client/Inc/mbedtls_config.h               mbedTLS library configuration options
    - mbedTLS/SSL_Client/Inc/FreeRTOSConfig.h               FreeRTOS configuration options
    - mbedTLS/SSL_Client/Inc/lwipopts.h                     LwIP stack configuration options
    - mbedTLS/SSL_Client/Inc/stm32f4xx_it.h                 Interrupt handlers header file 
    - mbedTLS/SSL_Client/Inc/stm32f4xx_hal_conf.h           Library Configuration file
    - mbedTLS/SSL_Client/Src/main.c                         Main program
    - mbedTLS/SSL_Client/Src/ssl_client.c                   SSL client main thread
    - mbedTLS/SSL_Client/Src/ethernetif.c                   Interfacing the LwIP stack to ETH driver
    - mbedTLS/SSL_Client/Src/stm32f4xx_hal_msp.c            HAL MSP module
    - mbedTLS/SSL_Client/Src/stm32f4xx_it.c                 Interrupt handlers 
    - mbedTLS/SSL_Client/Src/stm32f4xx_hal_timebase_tim.c   HAL time base functions
    - mbedTLS/SSL_Client/Src/net_socket.c                   mbedTLS TCP/IP socket API implementation using LwIP.
    - mbedTLS/SSL_Client/Src/system_stm32f4xx.c             STM32 system clock configuration file

@note Because of the low RAM size on the ST32F4 family the mbedtls_config.h is not the full-featured configuration.It is instead based on the configuration provided by the "config-suite-b.h" under mbedTLS-2.4.0/configs.
This file is the minimal configuration required for the  TLS NSA Suite B Profile RFC(6460).
https://tools.ietf.org/html/rfc6460. Thus to get this application working both the SSL_Server and SSL_Client need to be configured with the same profile.

@par Hardware and Software environment  

  - This application runs on STM32F407G/STMF417G Devices.
    
  - This application has been tested with two scenarios.
     - Using a Host PC as server, running a Fedora 23 operating System or similar.
	 - Using a Windows PC as server, running Windows 7 operating system or later.
     - using two STM32F4xG_EVAL boards the first as server, the second as client.
  
  - Scenario 1:
      - Remote PC Set-up
	  - Make sure that the PC is in the same local network as the baord.
	  - Using a Linux Machine
         - Download the mebedTLS source code from (https://tls.mbed.org/download/start/mbedtls-2.4.0-apache.tgz)
	     - build the mbedTLS package following the instructions in the https://github.com/ARMmbed/mbedtls#make
	     - Once done, run the server application "programs/ssl/ssl_server".
	  - Using a Windows Machine
	    - Follow the instructions in the file "Firmware\Utilities\PC_Software\ssl_server\readme.txt" to run the ssl_server application.
	    - the "ssl_server" will wait for remote connections.

  - Scenario 2:
      - a second STM32F4xG_EVAL running the "mdedTLS/SSL_Server" application
  
  - STM32F769I-Discovery Set-up 
    - Connect STM32F4xG_EVAL to your local network (through a straight ethernet cable).
   
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - edit the file "main.h" setup the correct "SERVER_NAME"
 - Rebuild all files and load your image into target memory
 - Run the application.

 @Note: 1 Please ensure that the remote PC IP address is the same IP address
          as the one defined in main.h in the "SERVER_NAME", and the port number
		  used for the connection is the same as the one defined in "SERVER_PORT"
		  
        2 If using two boards for testing and the DHCP is enabled then the "SSL_Server" needs to be
		  run first, to get the board IP Address and use it in the "SSL_Client" application.
        
	    3 To avoid network issues please make sure to assign different MAC addresses for boards connected
	      to the same LAN. Edit the values MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5
	      in the file "stm32f4xx_hal_conf.h"


 */
