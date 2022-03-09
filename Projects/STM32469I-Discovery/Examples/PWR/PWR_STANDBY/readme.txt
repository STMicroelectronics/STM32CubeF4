/**
  @page PWR_Standby PWR_STANDBY example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    PWR/PWR_STANDBY/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR STANDBY example.
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

How to enter the Standby mode and wake up from this mode by using an external 
reset or the WKUP pin.

In the associated software, the system clock is set to 180 MHz, an EXTI line
is configured to generate an interrupt on falling edge and the SysTick is programmed
to generate an interrupt each 250 ms. In the SysTick interrupt handler, the LED1 is
toggled, this is used to indicate whether the MCU is in STANDBY or RUN mode.

When a falling edge is detected on the EXTI line, an interrupt is generated. In the 
EXTI handler routine, the system enters STANDBY mode causing the LED1 to stop toggling. 

On STM32469I-DISCOVERY board, WKUP pin is connected to the USER Button.
So, the USER Button is used to drive both the standby sequence and the WKUP pin.

An edge on WKUP pin or an external RESET will wake-up the system from STANDBY. 
To wake up from Standby, either use the RESET button or the USER button. 

After wake-up from STANDBY mode, program execution restarts in the same way as after
a RESET and LED1 restarts toggling. Therefore there is no need to configure the RTC.

Two LEDs LED1 and LED3 are used to monitor the system's state as follows:
 - LED3 ON: configuration failed (system will go into an infinite loop)
 - LED1 toggling: system in RUN mode
 - LED1 OFF: system in STANDBY mode

These Steps are repeated in an infinite loop.

@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select the RTC clock source; in this 
       case the Backup domain will be reset in order to modify the RTC Clock source, as consequence RTC  
       registers (including the backup registers) and RCC_BDCR register are set to their reset values.


@note The connection of the LCD reset pin to a dedicated GPIO PH7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

Power, PWR, Standby mode, Interrupt, EXTI, Wakeup, Low Power, RTC, External reset, LSI

@par Directory contents 

  - PWR/PWR_STANDBY/Inc/stm32f4xx_conf.h         HAL Configuration file
  - PWR/PWR_STANDBY/Inc/stm32f4xx_it.h           Header for stm32f4xx_it.c
  - PWR/PWR_STANDBY/Inc/main.h                   Header file for main.c
  - PWR/PWR_STANDBY/Src/system_stm32f4xx.c       STM32F4xx system clock configuration file
  - PWR/PWR_STANDBY/Src/stm32f4xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY/Src/main.c                   Main program
  - PWR/PWR_STANDBY/Src/stm32f4xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32F469xx devices.
    
  - This example has been tested with STMicroelectronics STM32469I-DISCOVERY
    board and can be easily tailored to any other supported device 
    and development board.

  - STM32469I-DISCOVERY Set-up
    - Use LED1 and LED3 connected respectively to PG.06 and PD.05 pins
    - User push-button connected to pin PA.00 (EXTI0)
    - WakeUp Pin PWR_WAKEUP_PIN1 connected to PA.00


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
