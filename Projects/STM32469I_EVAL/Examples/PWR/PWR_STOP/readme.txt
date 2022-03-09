/**
  @page PWR_STOP Power Stop Mode Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    PWR/PWR_STOP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Power Stop Mode example.
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

How to enter the Stop mode and wake up from this mode by using the RTC wakeup 
timer event or an interrupt.

LED1 is ON during RUN mode and OFF during STOP mode.

In the associated software
  - the system clock is set to 180 MHz.
  - the EXTI15_10 is configured to generate an interrupt on falling edge.
  - the SysTick is programmed to generate an interrupt each 1 ms. 

The system enters STOP mode after 5s and will wait for the Tamper push-button is pressed
to wake up from STOP mode. 
Current consumption could be monitored through an ampere-meter.
After the system woken up from STOP, the clock system is reconfigured because the default clock 
after wake up is the HSI.

User may select either Stop mode or under drive stop mode by uncommenting the corresponding line inside the main.h file
  /* #define STOP_MODE            */
  /* #define STOP_UNDERDRIVE_MODE */
 
    - STOP Mode
    ============ 
          - Regulator in LP mode
          - VREFINT OFF with fast wakeup enabled
          - HSI as SysClk after Wake Up
          - No IWDG
          - Wakeup using EXTI Line (Tamper push-button PC.13)

    - Under-Drive STOP Mode
    ======================== 
          - Regulator in LP mode
          - Under drive feature enabled
          - VREFINT OFF with fast wakeup enabled
          - HSI as SysClk after Wake Up
          - No IWDG
          - Wakeup using EXTI Line (User push-button PC.13)

This behavior is repeated in an infinite loop.

@note To measure the current consumption in STOP mode, please refer to 
      @subpage PWR_CurrentConsumption example.

@note This example can not be used in DEBUG mode, this is due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disabled

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select the RTC clock source; in this 
       case the Backup domain will be reset in order to modify the RTC Clock source, as consequence RTC  
       registers (including the backup registers) and RCC_BDCR register are set to their reset values.

@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

Power, PWR, Stop mode, Interrupt, EXTI, Wakeup, Low Power, External reset,

@par Directory contents 

  - PWR/PWR_STOP/Inc/stm32f4xx_conf.h         HAL Configuration file
  - PWR/PWR_STOP/Inc/stm32f4xx_it.h           Header for stm32f4xx_it.c
  - PWR/PWR_STOP/Inc/main.h                   Header file for main.c
  - PWR/PWR_STOP/Src/system_stm32f4xx.c       STM32F4xx system clock configuration file
  - PWR/PWR_STOP/Src/stm32f4xx_it.c           Interrupt handlers
  - PWR/PWR_STOP/Src/main.c                   Main program
  - PWR/PWR_STOP/Src/stm32f4xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - STM32469I-EVAL RevC Set-up :
    - Use the Tamper push-button connected to pin PC.13 (EXTI15_10)
    - Connect an ampere-meter to  to measure the IDD current.
   
  - This example runs on STM32F469xx/STM32F479xx devices.

  - This example has been tested and validated with STMicroelectronics STM32469I-EVAL RevC 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example



 */
