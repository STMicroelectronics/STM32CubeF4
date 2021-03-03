/**
  @page FreeRTOS_Timers FreeRTOS Timers application
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FreeRTOS/FreeRTOS_Timers/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Timers application.
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

How to use timers of CMSIS RTOS API.

This application creates a thread that toggles LED2 every 400 ms, and a periodic 
timer that calls a callback function every 200 ms to toggle the LED1.  


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set according to the OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".

@par Keywords

RTOS, FreeRTOS, Thread, Timer

@par Directory contents

    - FreeRTOS/FreeRTOS_Timers/Inc/main.h                Main program header file
    - FreeRTOS/FreeRTOS_Timers/Inc/stm32f4xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/FreeRTOS_Timers/Inc/stm32f4xx_it.h        Interrupt handlers header file
    - FreeRTOS/FreeRTOS_Timers/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_Timers/Src/main.c                Main program
    - FreeRTOS/FreeRTOS_Timers/Src/stm32f4xx_it.c        Interrupt handlers


@par Hardware and Software environment

  - This application runs on STM32F40xxx/STM32F41xxx devices.
    
  - This application has been tested with STM324xG-EVAL evaluation board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */