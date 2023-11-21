/**
  @page LwIP_TFTP_Server LwIP_TFTP_Server application
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LwIP/LwIP_TFTP_Server/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the tftp server demonstration.
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

This application guides STM32Cube HAL API users to run a tftp server demonstration
for STM32F4xx devices.

User has to use a PC SW tool (ex. TFTPD32) to send/receive data to/from micro SD 
card installed in STM324x9I-EVAL board 

If the LCD is used (#define USE_LCD in main.h), log messages will be displayed 
to inform user about ethernet cable status and the IP address value, else this 
will be ensured by LEDs:
  + LED1: ethernet cable is connected.
  + LED2: ethernet cable is not connected.

If a DHCP server is available, a dynamic IP address can be allocated by enabling 
the DHCP process (#define LWIP_DHCP in lwipopts.h).

If a DHCP server is not available, after timeout connection, the device only gets a static 
IP address(the switch from static to dynamic IP address is not available in this application).

Note: Files to be transferred must have a short file name format (8.3), 
      maximum 8 characters for name and 3 characters for type, example:
       image.jpg => valid name
       image.jpeg => invalid name 
       image1234.jpg => invalid name  

Note: In this application the Ethernet Link ISR need the HAL time base to configure 
the Ethernet MAC, so the Ethernet Link interrupt priority must be set lower (numerically greater) 
than the HAL tick interrupt priority to ensure that the System tick increments while executing 
the Ethernet Link ISR.

Note: By default, the Ethernet Half duplex mode is not supported in the 
STM324x9I-EVAL board, for more information refer to the HAL_ETH_MspInit() 
function in the ethernetif.c file

@note the system clock (SYSCLK) is configured to run at 175 MHz and 50 MHz is provided 
      at the output PLL divided by PLL_Q. This frequency permit to reach 25 MHz clock 
      needed for SD operation and in line with microSD specification. 
      
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
For more details about this application, refer to UM1713 "STM32Cube interfacing with LwIP and applications"

@par Keywords

Connectivity, LwIP, Ethernet, TFTP Server, TCP/IP, DHCP, send, receive, SD Card, LCD

@par Directory contents

    - LwIP/LwIP_TFTP_Server/Inc/app_ethernet.h           header of app_ethernet.c file
    - LwIP/LwIP_TFTP_Server/Inc/ethernetif.h             header for ethernetif.c file
    - LwIP/LwIP_TFTP_Server/Inc/lcd_log_conf.h           LCD Log configuration file
    - LwIP/LwIP_TFTP_Server/Inc/ffconf.h                 FAT file system module configuration file
    - LwIP/LwIP_TFTP_Server/Inc/main.h                   Main program header file
    - LwIP/LwIP_TFTP_Server/Inc/sd_diskio.h              FatFS sd diskio driver header file
    - LwIP/LwIP_TFTP_Server/Inc/lwipopts.h               LwIP stack configuration options
    - LwIP/LwIP_TFTP_Server/Inc/stm32f4xx_hal_conf.h     HAL Configuration file
    - LwIP/LwIP_TFTP_Server/Inc/stm32f4xx_it.h           header for stm32f4xx_it.c
    - LwIP/LwIP_TFTP_Server/Inc/tftpserver.h             header for tftpserver.c
    - LwIP/LwIP_TFTP_Server/Inc/tftputils.h              header for tftputils.c
    - LwIP/LwIP_TFTP_Server/Src/app_ethernet.c           Ethernet specific module
    - LwIP/LwIP_TFTP_Server/Src/main.c                   Main program  
    - LwIP/LwIP_TFTP_Server/Src/sd_diskio.c              FatFS sd diskio driver implementation
    - LwIP/LwIP_TCP_Echo_Server/Src/ethernetif.c         Interfacing LwIP to ETH driver
    - LwIP/LwIP_TFTP_Server/Src/system_stm32f4xx.c       STM32 system configuration file
    - LwIP/LwIP_TFTP_Server/Src/stm32f4xx_it.c           Interrupt handlers
    - LwIP/LwIP_TFTP_Server/Src/tftpserver.c             tftp server application
    - LwIP/LwIP_TFTP_Server/Src/tftputils.c              tftp server utilities functions


@par Hardware and Software environment

  - This application runs on STM32F429xx/STM32F439xx Devices
  
  - This application has been tested with the following environments:
     - STM324x9I-EVAL board
     - TFTP client and DHCP server: PC utility TFTPD32 (http://tftpd32.jounin.net/)

  - STM324x9I-EVAL Set-up
    - Connect the eval board to remote PC (through a crossover ethernet cable)
      or to your local network (through a straight ethernet cable)
    - Connect a uSD Card to the MSD connector (CN17).

@note  When the uSD Card is used; the Camera module must be unplugged, this is due to
       the shared pins between the two devices.
  
  - Remote PC Set-up
    - Configure a static IP address for your remote PC 
      for example 192.168.0.11 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
