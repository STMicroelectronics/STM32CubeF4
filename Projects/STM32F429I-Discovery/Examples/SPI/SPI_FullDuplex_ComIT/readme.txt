/**
  @page SPI_FullDuplex_ComIT SPI Full Duplex IT example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics  *******************
  * @file    SPI/SPI_FullDuplex_ComIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI Full Duplex IT example.
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

This example shows how to ensure SPI data buffer transmission/reception between 
two boards by using an interrupt.

The communication is done with 2 boards through SPI.
   _________________________                        _________________________
  |           ______________|                      |______________           |
  |          |SPI4          |                      |          SPI4|          |
  |          |              |                      |              |          |
  |          |     CLK(PE02)|______________________|(PE02)CLK     |          |
  |          |              |                      |              |          |
  |          |    MISO(PE05)|______________________|(PE05)MISO    |          |
  |          |              |                      |              |          |
  |          |    MOSI(PE06)|______________________|(PE06)MOSI    |          |
  |          |              |                      |              |          |
  |          |______________|                      |______________|          |
  |      __                 |                      |      __                 |
  |     |__|                |                      |     |__|                |
  |     USER                |                      |     USER                |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32F429i______________|                      |_STM32F429i______________|

This example guides you through the different configuration steps by mean of HAL API 
to ensure SPI Data buffer transmission and reception using Interrupt.

HAL architecture allows user to easily change code to move to DMA or Polling 
mode. To see others communication modes please check following examples:
SPI/SPI_FullDuplex_ComDMA
SPI/SPI_FullDuplex_ComPolling

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change SPI configuration.

The SPI communication is then initiated.
The HAL_SPI_TransmitReceive_IT() function allows the reception and the 
transmission of a predefined data buffer at the same time (Full Duplex Mode). 
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file.
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User Key, SPI Master starts the 
communication by sending aTxBuffer and receiving aRxBuffer through 
HAL_SPI_TransmitReceive_IT(), at the same time SPI Slave transmits aTxBuffer 
and receives aRxBuffer through HAL_SPI_TransmitReceive_IT(). 
The end of this step is monitored through the HAL_SPI_GetState() function
result.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

STM32 Discovery boards LEDs can be used to monitor the transfer status:
 - LED3 toggles on master board waiting user button to be pressed. Once done LED3 turns off.
 - LED4 turns ON when there is an error in transmission/reception process
 - LED3 turns ON when the transmission/reception process is complete.

 @note SPIx instance used and associated resources can be updated in "main.h"
       file depending hardware configuration used.

 @note You need to perform a reset on Slave board, then perform it on Master board
       to have the correct behaviour of this example.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, SPI, Full-duplex, Interrupt, Transmission, Reception, Master, Slave, MISO, MOSI

@par Directory contents 

  - SPI/SPI_FullDuplex_ComIT/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - SPI/SPI_FullDuplex_ComIT/Inc/stm32f4xx_it.h          SPI interrupt handlers header file
  - SPI/SPI_FullDuplex_ComIT/Inc/main.h                  Main program header file  
  - SPI/SPI_FullDuplex_ComIT/Src/stm32f4xx_it.c          SPI interrupt handlers
  - SPI/SPI_FullDuplex_ComIT/Src/main.c                  Main program
  - SPI/SPI_FullDuplex_ComIT/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - SPI/SPI_FullDuplex_ComIT/Src/stm32f4xx_hal_msp.c     HAL MSP module    


@par Hardware and Software environment 

  - This example runs on STM32F429xx devices.
    
  - This example has been tested with STMicroelectronics STM32F429I-Discovery RevC
    boards and can be easily tailored to any other supported device 
    and development board.
  
  - STM32F429I-Discovery RevC Set-up
    - Connect Master board PE02 to Slave Board PE02
    - Connect Master board PE05 to Slave Board PE05
    - Connect Master board PE06 to Slave Board PE06
    - Connect Master board GND  to Slave Board GND


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
