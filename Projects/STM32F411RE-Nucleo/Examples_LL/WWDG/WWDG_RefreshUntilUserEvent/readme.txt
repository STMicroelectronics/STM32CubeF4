/**
  @page WWDG_RefreshUntilUserEvent WWDG example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/WWDG/WWDG_RefreshUntilUserEvent/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the WWDG_RefreshUntilUserEvent example.
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

This example describes how to configure the WWDG, periodically update the counter,
and generate an MCU WWDG reset when a user button is pressed. Peripheral initialization
is done using LL unitary services functions for optimization purpose (performance and size).

Example Configuration:
Configure the WWDG (Window, Prescaler & Counter) and enable it.
Refresh the WWDG downcounter in the main loop - LED2 is blinking fastly & continuously

Example Execution:
When User Button is pressed, the Downcounter automatic refresh mechanism is disable and thus, reset will occur.
After a reset when re-entering in the main, RCC WWDG Reset Flag will be checked and if we are back from a WWDG reset the LED2 will be switch ON.

Waiting a new user button pressed to re-activate the WWDG

@par Keywords

System, WWDG, Timeout, Refresh, Counter update, MCU Reset, Downcounter, Event, Window

@par Directory contents 

  - WWDG/WWDG_RefreshUntilUserEvent/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - WWDG/WWDG_RefreshUntilUserEvent/Inc/main.h                  Header for main.c module
  - WWDG/WWDG_RefreshUntilUserEvent/Inc/stm32_assert.h          Template file to include assert_failed function
  - WWDG/WWDG_RefreshUntilUserEvent/Src/stm32f4xx_it.c          Interrupt handlers
  - WWDG/WWDG_RefreshUntilUserEvent/Src/main.c                  Main program
  - WWDG/WWDG_RefreshUntilUserEvent/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
