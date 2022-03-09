/**
  @page FreeRTOS_Queues FreeRTOS Queues application
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FreeRTOS/FreeRTOS_Queues/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Queues application.
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

How to use message queues with CMSIS RTOS API.

This application creates two threads that send and receive an incrementing number 
to/from a queue, as following:
One thread acts as a producer and the other as the consumer.  The consumer 
is a higher priority than the producer and is set to block on queue reads. 
The queue only has space for one item - as soon as the producer posts a
message on the queue the consumer will unblock, preempt the producer, 
and remove the item.  

Add the following variables to LiveWatch, these variables must remain equals all the time:
 - ProducerValue
 - ConsumerValue

STM32 Eval boards LEDs can be used to monitor the application status:
  - LED1 should toggle when the application runs successfully.
  - LED3 is ON when any error occurs.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond to have correct 
      HAL operation.
	  
@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set according to the OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".

@par Keywords

RTOS, FreeRTOS, Threading, Message, Queues

@par Directory contents

    - FreeRTOS/FreeRTOS_Queues/Inc/main.h                Main program header file
    - FreeRTOS/FreeRTOS_Queues/Inc/stm32f4xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/FreeRTOS_Queues/Inc/stm32f4xx_it.h        Interrupt handlers header file
    - FreeRTOS/FreeRTOS_Queues/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_Queues/Src/main.c                Main program
    - FreeRTOS/FreeRTOS_Queues/Src/stm32f4xx_it.c        Interrupt handlers


@par Hardware and Software environment

  - This application runs on STM32F42xxx/STM32F43xxx devices
    
  - This application has been tested with STM324x9I-EVAL RevB evaluation board and can be
    easily tailored to any other supported device and development board.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
