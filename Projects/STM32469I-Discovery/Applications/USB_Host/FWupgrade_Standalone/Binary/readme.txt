/**
  @page Binary Description of the binary file
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    USB_Host/FWupgrade_Standalone/Binary/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the binary file.
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

This directory contains a binary to be loaded into Flash memory using Device 
Firmware Upgrade application.
   
This binary is a simple LED chaser. The 4 LEDs lights one by one for a period of 100 ms and the cycle 
repeats giving the running light appearance.
The system Timer (Systick) is used for to generate the delay.
The offset address of this binary is 0x08008000 which matches the definition in DFU application
"USBD_DFU_APP_DEFAULT_ADD".


@par Hardware and Software environment

  - This application runs on STM32F469xx devices.
    
  - This application has been tested with STMicroelectronics STM32469I-Discovery
    evaluation boards and can be easily tailored to any other supported device 
    and development board.  

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
