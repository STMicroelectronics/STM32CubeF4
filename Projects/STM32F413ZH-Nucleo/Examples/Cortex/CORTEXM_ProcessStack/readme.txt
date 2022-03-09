    /**
  @page CORTEXM_ProcessStack Cortex-M4 Process Stack example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    CORTEXM/CORTEXM_ProcessStack/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Cortex-M4 Mode Privilege example.
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

This example shows how to modify Thread mode stack. Thread mode is entered on Reset, 
and can be entered as a result of an exception return.  

The associated program is used to:

1. Switch the Thread mode stack from Main stack to Process stack

2. Generate ISR (SVC) to confirm Stack pointer used under exception is Main Stack

3. Returning to main(), check is performed on current Stack to show cortex has switched back to Process
automatically.
In case of error, LED3 is blinking fast. In case of success, LED1 is turned on.

To monitor the stack used  a set of variables is available within the program. It is also
possible to use the 'Cortex register' window of the debugger.
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, Cortex, Process Stack, HCLK, Threading mode Stack, main stack, process stack, ISR

@par Directory contents 

  - CORTEXM/CORTEXM_ProcessStack/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - CORTEXM/CORTEXM_ProcessStack/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - CORTEXM/CORTEXM_ProcessStack/Inc/main.h                  Header for main.c module  
  - CORTEXM/CORTEXM_ProcessStack/Src/stm32f4xx_it.c          Interrupt handlers
  - CORTEXM/CORTEXM_ProcessStack/Src/main.c                  Main program
  - CORTEXM/CORTEXM_ProcessStack/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment

  - This example runs on STM32F413xx/STM32F423xx devices.
    
  - This example has been tested with STM32F413ZH-Nucleo board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 
In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example


 */
