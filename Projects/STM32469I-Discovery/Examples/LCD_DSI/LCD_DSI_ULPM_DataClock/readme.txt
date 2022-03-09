/**
  @page LCD_DSI_ULPM_DataClock : LCD DSI example : enter and exit DSI ULPM Mode on data lanes 
  and clock lane while displaying a picture 800x480 (WVGA) 
  in landscape mode in DSI mode Video Burst on LCD screen.

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LCD_DSI/LCD_DSI_ULPM_DataClock/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD DSI enter and exit DSI ULPM Mode on data lane 
  *          and clock lane example.
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

This example provides a description of how to use the embedded LCD DSI controller 
(using IPs LTDC and DSI Host) to drive the KoD LCD mounted on board and manage entry and exit in 
DSI ULPM mode on data lane and clock lane. 

In this mode, the DSI PHY state machine is entering a low power state
on data lane and clock lane.
The DSI Dphy_Clk should be derived from the clock tree PLL.PLLR (ck_plldsi) with a 60 MHz clock for instance
to allow a switch off of the PLL DPHY to save power during the ULPM phase.
When exiting from ULPM, the PLL DPHY is first switched back on and locked, then the DSI DPHY will exit
power down and display again as before.

In this example a WVGA landscape picture (800x480) is displayed on LCD in DSI Mode Video Burst.
On a Tamper button press from the user, the LCD display will be switched off. 100ms later, 
the DSI PHY Data and clock lane will enter ULPM mode. DphyClk is derived from PLL.PLLR, then
DPHY PLL is switched Off.

After 2 seconds in Off mode, The PLL DPHY is switched back on and locked,
then the ULPM on data and clock lanes will be exited in DPHY DSI state machine.
The LCD will then be switched back on and display the same image as before 
with a text notifying that the exit from ULPM was successful. 

The image candies_800x480_argb8888 is of format ARGB8888 and is initially copied from Flash to SDRAM Frame Buffer.
The LTDC is reading continuously the LCD Frame buffer from SDRAM, sent it to DSI Host which sends it in burst mode (DCS commands)
via the DSI DPHY to the KoD display that decodes DSI packets and refresh its internal Graphic RAM to display on glass.
The Frame number display is managed by the BSP_LCD by function implementing font to display a text in SDRAM Frame buffer.

LED1 ON: DSI PHY Data and clock lane in ULPM mode
LED1 OFF: DSI PHY Data and clock lane in run mode
LED3 ON: an error occurred.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Graphic, Display, LCD, DSI, MIPI Alliance, Ultra low power mode, Tearing effect, Partial refresh, Single buffer,
LTDC, WVGA, ARGB8888, SDRAM, Burst mode, Data lane, clock lane

@par Directory contents

  - LCD_DSI/LCD_DSI_ULPM_Data/Inc/stm32f4xx_hal_conf.h          HAL configuration file
  - LCD_DSI/LCD_DSI_ULPM_Data/Inc/stm32f4xx_it.h                Interrupt handlers header file
  - LCD_DSI/LCD_DSI_ULPM_Data/Inc/main.h                        Header for main.c module
  - LCD_DSI/LCD_DSI_ULPM_Data/Inc/candies_800x480_argb8888.h    Image 800x480 in ARGB8888 to display on LCD
  - LCD_DSI/LCD_DSI_ULPM_Data/Src/stm32f4xx_it.c                Interrupt handlers
  - LCD_DSI/LCD_DSI_ULPM_Data/Src/main.c                        Main program
  - LCD_DSI/LCD_DSI_ULPM_Data/Src/stm32f4xx_hal_msp.c           HAL MSP file
  - LCD_DSI/LCD_DSI_ULPM_Data/Src/system_stm32f4xx.c            STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F469xx devices.

  - This example has been tested with STMicroelectronics STM32469I-DISCOVERY
    board and can be easily tailored to any other supported device
    and development board.

  - This example is configured to run by default on STM32469I-DISCO RevC board.
  - In order to run this example on RevA or RevB boards, replace the flag 
    USE_STM32469I_DISCO_REVC, which is defined in the pre-processor options, by 
    either USE_STM32469I_DISCO_REVA or USE_STM32469I_DISCO_REVB respectively.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example



 */
