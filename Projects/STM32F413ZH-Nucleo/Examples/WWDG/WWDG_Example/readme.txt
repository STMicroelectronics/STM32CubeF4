/**
  @page WWDG_Example Window Watchdog example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    WWDG/WWDG_Example/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Window Watchdog example.
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

This example guides you through the different configuration steps by means of the 
HAL API to perform periodic WWDG counter update and simulate a software fault that 
generates an MCU WWDG reset when a predefined time period has elapsed.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 100 MHz.

The WWDG peripheral configuration is ensured by the HAL_WWDG_Init() function.
This later is calling the HAL_WWDG_MspInit() function which core is implementing
the configuration of the needed WWDG resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change WWDG configuration.

The WWDG timeout is set, through counter value, to 42 ms. 
The refresh window is set in order to make user wait 32 ms after a wadchdog refresh, 
before writing again counter. Hence the WWDG counter is refreshed each (32 + 1) ms in the 
main program infinite loop to prevent a WWDG reset. 
LED2 is toggling at same frequency, indicating that the program is running.

An EXTI Line is connected to a GPIO pin, and configured to generate an interrupt
on the rising edge of the signal.

The EXTI Line is used to simulate a software failure: once the EXTI Line event 
occurs by pressing the User push-button (PC.13), the corresponding interrupt is served.

In the ISR, a write to invalid address generates a Hardfault exception containing
an infinite loop and preventing to return to main program (the WWDG counter is 
not refreshed).

As a result, when the WWDG counter falls to 0x3F and WWDG reset occurs.

If the WWDG reset is generated, after the system resumes from reset, LED1 is turned ON for 4 seconds.

If the EXTI Line event does not occur, the WWDG counter is indefinitely refreshed
in the main program infinite loop, and there is no WWDG reset.

LED3 is turned ON and remains ON if any error occurs.

@note This example must be tested in standalone mode (not in debug).


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, WWDG, Downcounter, MCU Reset, Timeout, Software fault

@par Directory contents 

  - WWDG/WWDG_Example/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - WWDG/WWDG_Example/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - WWDG/WWDG_Example/Inc/main.h                  Header for main.c module  
  - WWDG/WWDG_Example/Src/stm32f4xx_it.c          Interrupt handlers
  - WWDG/WWDG_Example/Src/main.c                  Main program
  - WWDG/WWDG_Example/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - WWDG/WWDG_Example/Src/system_stm32f4xx.c      STM32F4xx system source file


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
