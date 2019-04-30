/**
  @page LCD_DSI_ULPM_Data : LCD DSI example : enter and exit DSI ULPM Mode on data lane only,
  while displaying a picture 800x480 (WVGA) in landscape mode in DSI mode Video Burst on LCD
  screen.

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LCD_DSI/LCD_DSI_ULPM_Data/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD DSI enter and exit DSI ULPM Mode on data lane only example.
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This example provides a description of how to use the embedded LCD DSI controller 
(using IPs LTDC and DSI Host) to drive the KoD LCD mounted on board and manage entry and exit in 
DSI ULPM mode on data lane only. In this mode, the DSI PHY state machine is entering a low power state
on data lane and allows to save some power when the LCD does not need to display. 
When the display is needed again, the DSI ULPM on data lane is exited and display should operate as before.

In this example a WVGA landscape picture (800x480) is displayed on LCD in DSI Mode Video Burst.
On a USER button press from the user, the LCD display will be switched off. 100ms later,  
the DSI PHY Data lane will enter ULPM mode. After 2 seconds in Off mode, the ULPM on data lane 
will be exited, the LCD will be switched back on and display the same image as before with a 
text notifying that the exit from ULPM was successful. 

The image candies_800x480_argb8888 is of format ARGB8888 and is initially copied from Flash to SDRAM Frame Buffer.
The LTDC is reading continuously the LCD Frame buffer from SDRAM, sent it to DSI Host which sends it in burst mode (DCS commands)
via the DSI DPHY to the KoD display that decodes DSI packets and refresh its internal Graphic RAM to display on glass.
The Frame number display is managed by the BSP_LCD by function implementing font to display a text in SDRAM Frame buffer.

LED1 ON: DSI PHY Data lane in ULPM mode
LED1 OFF: DSI PHY Data lane in run mode
LED3 ON: an error occured.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

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

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
