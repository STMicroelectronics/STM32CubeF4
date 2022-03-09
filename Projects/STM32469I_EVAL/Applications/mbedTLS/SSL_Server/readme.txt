/**
  @page SSL_Server SSL Server application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    mbedTLS/SSL_Server/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of SSL Server application.
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

This application guides STM32Cube HAL API users to run an SSL Server application 
based on mbedTLS crypto library and LwIP TCP/IP stack.

This application starts SSL server on the board waiting for connections from clients.
The board joins the local network using either a DHCP server or a static IP address.
Once done a green led is toggling informing that the server is ready.

As soon as a request from a client is received, the application establishes the secured
connection then sends the response to the client. the response is a HTML page with the text similar to:

"
mbed TLS Test Server
Successful connection using: TLS-ECDHE-RSA-WITH-AES-256-CBC-SHA
"

it is possible to test this application via two scenarios:

Scenario 1:
===========
  - the client is a PC with a web browser.
  - the browser connects to the address https://BOARD_IP_ADDRESS:SERVER_PORT
  - the afementioned string is displayed on the browser.
  
Scenario 2:
  - the client is a second board (STM32469I_EVAL), running the "SSL_Client" application.
  - the second board connects to this application.
  - the "webpage content" is displayed in text format on the LCD screen of the second board.
  

In both scenarios, the "SSL_Server" Application log is displayed on the LCD screen.

@note the Scenario 1 was tested using Google Chrome 52 and Internet Explorer 11, earlier versions of those
      browsers may not work.

@note the ssl_server is not using a valid certificate to authentify, thus the browsers show a warning about
it, ignore that warning to let the application continue.

Note: In this application the Ethernet Link ISR need the System tick interrupt 
to configure the Ethernet MAC, so the Ethernet Link interrupt priority must be 
set lower (numerically greater) than the Systick interrupt priority to ensure 
that the System tick increments while executing the Ethernet Link ISR.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about this application, refer to UM1723 "STM32Cube mbedTLS application".

@par Keywords

Security, mbedTLS, SSL Server, Crypto, TCP/IP, LwIP, AES 128,
AES 192, AES 256, Triple DES, MD5, SHA-1, RNG

@par Directory contents

    - mbedTLS/SSL_Server/Inc/ethernetif.h                   header for ethernetif.c file
    - mbedTLS/SSL_Server/Inc/main.h                         Main program header file 
    - mbedTLS/SSL_Server/Inc/config.h                       mbedTLS library configuration options
    - mbedTLS/SSL_Server/Inc/FreeRTOSConfig.h               FreeRTOS configuration options
    - mbedTLS/SSL_Server/Inc/lwipopts.h                     LwIP stack configuration options
    - mbedTLS/SSL_Server/Inc/stm32f4xx_it.h                 Interrupt handlers header file 
    - mbedTLS/SSL_Server/Inc/stm32f4xx_hal_conf.h           Library Configuration file
    - mbedTLS/SSL_Server/Inc/ssl_server.h                   header for ssl_server.c
    - mbedTLS/SSL_Server/Src/main.c                         Main program
    - mbedTLS/SSL_Server/Src/ssl_server.c                   SSL Server main thread
    - mbedTLS/SSL_Server/Src/ethernetif.c                   Interfacing the LwIP stack to ETH driver
    - mbedTLS/SSL_Server/Src/stm32f4xx_hal_msp.c            HAL MSP module
    - mbedTLS/SSL_Server/Src/stm32f4xx_it.c                 Interrupt handlers
    - mbedTLS/SSL_Server/Src/stm32f4xx_hal_timebase_tim.c   HAL time base functions
    - mbedTLS/SSL_Server/Src/system_stm32f4xx.c             STM32 system clock configuration file
    - mbedTLS/SSL_Server/Src/net_sockets.c                  the TCP/IP socket API implemented over the LwIP stack.

@note Because of the low RAM size on the ST32F4 family the mbedtls_config.h is not the full-featured configuration.It is instead based on the configuration provided by the "config-suite-b.h" under mbedTLS-2.4.0/configs.
This file is the minimal configuration required for the  TLS NSA Suite B Profile RFC(6460).
https://tools.ietf.org/html/rfc6460. Thus to get this application working both the SSL_Server and SSL_Client need to be configured with the same profile.


@par Hardware and Software environment  

  - This application runs on STM32469/STM32479 Devices.
  
  - This application has been tested with two scenarios.
     - Using a Host PC as a client.
     - using two STM32469I_EVAL boards the first as server, the second as client.

 - Screnario 1:
      - Remote PC Set-up
	  - Make sure that the PC is in the same local network as the baord.
	  - Web browser available on the PC.
	  
  - Screnario 2:
    - a second STM32469I_EVAL running the "mdedTLS/SSL_Server" application
    - STM32469I_EVAL Set-up 
    - Connect STM32469I_EVAL to your local network (through a straight ethernet cable).
   
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 - Use Scenario 1 or Scenario 2 described above.

 @Note: 1 When connecting to the board using a web Browser, make sure to type the URL as follows:
          https://BOARD_IP_ADDRESS:SERVER_PORT
		  
        2 If using two boards for testing and the DHCP is enabled then the "SSL_Server" needs to be
		  run first, to get the board IP Address and use it in the "SSL_Client" application.
        
        3 To avoid network issues please make sure to assign different MAC addresses for boards connected
	      to the same LAN. Edit the values MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5
	      in the file "stm32f4xx_hal_conf.h"

	    4 The HTML response may differ depending on the encryption method used between the server
	      and the client, this is highly depending on the mbedTLS configuration.


 */
