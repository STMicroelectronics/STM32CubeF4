/**
  @page FreeRTOS_LowPower FreeRTOS Low Power example
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FreeRTOS/FreeRTOS_LowPower/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS low power example.
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

How to enter and exit low-power mode with CMSIS RTOS API.

This example creates two threads.

   + A Rx thread that blocks on a queue to wait for data, blinking a LED each 
     time data is received (turning it on and then off again) before returning 
     to block on the queue once more.

   + A Tx thread that repeatedly enters the Blocked state for 500ms.  
     On exiting the blocked state the Tx thread sends a value through the queue 
     to the Rx thread (causing the Rx thread to exit the blocked state and blink 
     the LED).

Blocking for a finite period allows the kernel to stop the tick interrupt
and place the STM32 into sleep mode - the lowest power mode possible
that allows the CPU registers and RAM to retain their state.

In this example, non-used GPIO's are configured to analog, thus helping to reduce 
the power consumption of the device.

Observed behaviour:

Every 500ms the MCU will come out of the low power state to turn the LED on,
then return to the low power state for 20ms before leaving the low power
state again to turn the LED off.  This will be observed as a fast blinking
on the LED.

The RTOS tick is suppressed while the MCU is in its low power state.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the 
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".

@par Keywords

RTOS, FreeRTOS, Threading, Low Power, Sleep mode, 

@par Directory contents
    - FreeRTOS/FreeRTOS_LowPower/Inc/main.h                main config file
    - FreeRTOS/FreeRTOS_LowPower/Inc/stm32f4xx_hal_conf.h  HAL Configuration file
    - FreeRTOS/FreeRTOS_LowPower/Inc/stm32f4xx_it.h        Header for stm32f4xx_it.c
    - FreeRTOS/FreeRTOS_LowPower/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_LowPower/Src/main.c                main program file
    - FreeRTOS/FreeRTOS_LowPower/Src/stm32f4xx_it.c        STM32 Interrupt handlers

@par Hardware and Software environment

  - This example runs on STM32F4xx devices.
    
  - This example has been tested with a STM32412G-DISCOVERY board embedding
    a STM32412G device and can be easily tailored to any other supported device 
    and development board.


@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 

 */
