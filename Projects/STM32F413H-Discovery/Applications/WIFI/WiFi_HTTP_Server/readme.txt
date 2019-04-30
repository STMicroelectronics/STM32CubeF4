/**
  @page WiFi_HTTP_Server es WiFi module in Web server demonstration
  
  @verbatim
  ******************************************************************************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Es_WIFI Module in Web server mode.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. All rights reserved.
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

This application shows how to make HTTP requests using the Es-WiFi module based 
on STM32 Cube HAL. 

In this example, you will use Es_WiFi module and a web Browser to create a simple 
Web server. 

After opening a browser and navigating to es-WiFi shield's IP address, the 
STM32F413H-Discovery will be able to answer a HTTP request with es-WiFI shield. 

An HTTP client sends a request message to an HTTP server.
     
This application may be used with a Smartphone (more detailed setup
instructions are described in section "Hardware and Software environment").

If the LCD is used (#define LCD_USE in main.c), log messages will be displayed 
to inform user about es_wifi module status and the IP address value, else this 
will be ensured by LEDs:
  + LED4: error connection.

The communication is done with a web browser application in a SmartPhone.

This HTTP server contains a html page:
  + LED control: allows to control the Green Led LED3 located in the 
    STM32F413H-Discovery board
  + ADC conversion: it shows the temperature sensor converted value

@par Directory contents
 - WiFi/WiFi_HTTP_Server/Inc/main.h                   header for main.c module
 - WiFi/WiFi_HTTP_Server/Inc/stm32f4xx_hal_conf.h     HAL configuration file
 - WiFi/WiFi_HTTP_Server/Inc/stm32f4xx_it.h           STM32 interrupt handlers header file
 - WiFi/WiFi_HTTP_Server/Src/main.c                   Main Program
 - WiFi/WiFi_HTTP_Server/Src/system_stm32f4xx.c       STM32F4xx system clock configuration file
 - WiFi/WiFi_HTTP_Server/Src/stm32f4xx_it.c           STM32 interrupt handlers
 - WiFi/Common/Src/es_wifi.c                          Implementation of the ES_WIFI_XXX() API.
 - WiFi/Common/Src/es_wifi_io.c                       Implementation of the ES_WIFI_IO_XXX() API.
 - WiFi/Common/Src/wifi.c                             Implementation of the WIFI_XXX() API.
 - WiFi/Common/Inc/es_wifi.h                          Header for the functions and defines used by the es_wifi.c 
 - WiFi/Common/Inc/es_wifi_io.h                       Header for the functions and defines used by the es_wifi_io.c   
 - WiFi/Common/Inc/wifi.h                             Header for the functions and defines used by the wifi.c   

@par Hardware and Software environment

  - This application runs on STM32F413XX devices.
    
  - This application has been tested with the following environment:
     - STM32F413H_DISCOVERY Rev-B  board. 
	  
  - When using Android Phone
    - Connect your Android phone to a network.
    - Update in main.c the following defines SSID and PASSWORD with your own AP settings
    - Open a Web browser
    - Put the es-wifi IP address displayed on LCD in the web browser
    
In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Check that DK board is connected to a remote PC before running the application
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
