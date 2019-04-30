/**
  @page LwIP_HTTP_Server_Netconn_RTOS LwIP HTTP Server Netconn Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LwIP http server Netconn API.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Application Description 

This application guides STM32Cube HAL API users to run a http server application 
based on Netconn API of LwIP TCP/IP stack
The communication is done with a web browser application in a remote PC.

this HTTP server contains two html pages:
  + the first page (home page) is static, it gives information about STM32F4
    and LwIP stack.
  + the second page is dynamically refreshed (every 1 s), it shows the RTOS 
    statistics in runtime

If a DHCP server is available, a dynamic IP address can be allocated by enabling 
the DHCP process (#define USE_DHCP in main.h)

Note: In this application the Ethernet Link ISR need the HAL time base to configure 
the Ethernet MAC, so the Ethernet Link interrupt priority must be set lower (numerically greater) 
than the HAL tick interrupt priority to ensure that the System tick increments while executing 
the Ethernet Link ISR.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond to have correct 
      HAL operation.
      
For more details about this application, refer to UM1713 "STM32Cube interfacing with LwIP and applications"


@par Directory contents

  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Inc/app_ethernet.h                  header of app_ethernet.c file
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Inc/ethernetif.h                    header for ethernetif.c file
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Inc/stm32f4xx_hal_conf.h            HAL configuration file
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Inc/stm32f4xx_it.h                  STM32 interrupt handlers header file
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Inc/main.h                          Main program header file
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Inc/lwipopts.h                      LwIP stack configuration options
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Inc/FreeRTOSConfig.h                FreeRTOS configuration options
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Inc/httpserver-netconn.h            header for httpserver-netconn.c
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/app_ethernet.c                  Ethernet specific module
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/stm32f4xx_it.c                  STM32 interrupt handlers
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/main.c                          Main program
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/system_stm32f4xx.c              STM32F4xx system clock configuration file
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/ethernetif.c                    Interfacing LwIP to ETH driver
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/httpserver-netconn.c            httpserver netconn main thread
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/fsdata_custom.c                 ROM filesystem data (html pages)
  - LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/stm32f4xx_hal_timebase_tim.c    HAL time base based on the hardware TIM
  
        
@par Hardware and Software environment

  - This application runs on STM32F429xx devices.
    
  - This application has been tested with the following environments:
     - NUCLEO-F429ZI Rev.B board
     - Http clients: Firefox Mozilla (v24) or Microsoft Internet Explorer (v8 and later)
     - DHCP server:  PC utility TFTPD32 (http://tftpd32.jounin.net/) is used as a DHCP server    
      
  - NUCLEO-F429ZI Set-up
    - Connect the nucleo board to remote PC (through a crossover ethernet cable)
      or to your local network (through a straight ethernet cable)
  
  - Remote PC Set-up
    - Configure a static IP address for your remote PC 
      for example 192.168.0.11 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Check that nucleo board is connected to a remote PC before running the application
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
