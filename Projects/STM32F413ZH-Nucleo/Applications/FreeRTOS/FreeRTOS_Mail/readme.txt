/**
  @page FreeRTOS_Mail FreeRTOS mail application
 
  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_Mail/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Mail application.
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

How to use mail queues with CMSIS RTOS API.

This application creates two threads that send and receive mail
the mail to send/receive is a structure that holds three variables (var1 and var2 are uint32, var3 is a uint8)

One thread acts as a producer and the other as the consumer.  
The consumer is a higher priority than the producer and is set to block on mail receiving.

The Mail queue has space for one item. The producer allocates the mail and put it on the mail queue. 
As soon as the producer posts a mail on the queue the consumer will unblock, preempt the producer, 
get the mail and free it.  

Add the following variables to LiveWatch, the three producer values must respectively remain equals to the three consumer values all the time:
 - ConsumerValue1 must remain equal to ProducerValue1
 - ConsumerValue2 must remain equal to ProducerValue2
 - ConsumerValue3 must remain equal to ProducerValue3
  
LEDs can be used to monitor the application status:
  - LED1 should toggle when the application runs successfully.
  - LED3 is toggling when any error occurs.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in HAL time base ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process, then
      the HAL time base interrupt must have higher priority (numerically lower) than
      the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority()
      function.
 
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the 
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".

@par Keywords

RTOS, FreeRTOS, Threading, Mail, Queues, 

@par Directory contents
    - FreeRTOS/FreeRTOS_Mail/Src/main.c                Main program
    - FreeRTOS/FreeRTOS_Mail/Src/stm32f4xx_hal_timebase_tim.c HAL timebase file
    - FreeRTOS/FreeRTOS_Mail/Src/stm32f4xx_it.c        Interrupt handlers
    - FreeRTOS/FreeRTOS_Mail/Src/system_stm32f4xx.c    STM32F4xx system clock configuration file
    - FreeRTOS/FreeRTOS_Mail/Inc/main.h                Main program header file
    - FreeRTOS/FreeRTOS_Mail/Inc/stm32f4xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/FreeRTOS_Mail/Inc/stm32f4xx_it.h        Interrupt handlers header file
    - FreeRTOS/FreeRTOS_Mail/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file

@par Hardware and Software environment

  - This application runs on STM32F413xx/STM32F423xx devices.
    
  - This application has been tested with STM32F413ZH-Nucleo board and can be
    easily tailored to any other supported device and development board.
    

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
  

 */
