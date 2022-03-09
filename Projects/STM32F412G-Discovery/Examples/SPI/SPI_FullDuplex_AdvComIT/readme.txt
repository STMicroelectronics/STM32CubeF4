/**
  @page SPI_FullDuplex_AdvComIT SPI Full Duplex Polling example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    SPI/SPI_FullDuplex_AdvComIT/readme.txt 
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
 
This example guides you through the different configuration steps by mean of HAL API
to transmit/receive SPI data buffer using Interrupt, in an advanced communication mode :
The master board always sends the command to the slave before any transmission is performed.
The slave board sends an acknowledge before going further.

- Hardware Description

To use this example, you need to load it on two STM32 boards (let's call them 
BoardA and BoardB) then connect these two boards through SPI lines and GND.
In the firmware example two project are provided to use the SPI peripheral as STM32
Master device or as STM32 Slave .
   _________________________                        _________________________
  |           ______________|                      |______________           |
  |          |SPI2          |                      |          SPI2|          |
  |          |              |                      |              |          |
  |          |     CLK(PB13)|______________________|(PB13)CLK     |          |
  |          |              |                      |              |          |
  |          |    MISO(PB14)|______________________|(PB14)MISO    |          |
  |          |              |                      |              |          |
  |          |    MOSI(PB15)|______________________|(PB15)MOSI    |          |
  |          |              |                      |              |          |
  |          |______________|                      |______________|          |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32F4 Master _________|                      |_STM32F4 Slave __________|
  
@note The connection between the pins should use a short wires and a common Ground.

- Software Description  

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 100 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change SPI configuration.

The SPI peripheral is configured in full duplex mode with baudrate = FCPU/256. 

If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.
                       
In Master board, when the user button is pressed (used for only synchronization at startup)
these steps are executed: 
 - Synchro with the Slave
 - Requests read operation by sending specific command to the slave that contain
   the master read command and the number of data to be read
 - Synchro with the Slave
 - Receive the ACK from the Slave
 - Synchro with the Slave  
 - Receive the requested data "aRxBuffer" from the slave
 - Synchro with the Slave
 - Send the ACK to the Slave
 - Checks the correctness of data and in case of mismatch between sent and received data,
   LED3 is On and communication is stopped (using infinite loop). Otherwise LED4 is Toggle
   indicate that the data is received correctly.
 - Synchro with the Slave
 - Requests write operation by sending specific command to the slave that contain
   the master write command and the number of data to be written
 - Synchro with the Slave 
 - Receive the ACK from the Slave
 - Synchro with the Slave 
 - Send the requested data "aTxMasterBuffer" to the slave
 - Synchro with the Slave 
 - Receive the ACK from the Slave
These previously steps are repeated in an infinitely loop and a delay 100ms is inserted at
the end of data transfer to permit the user to see LED1 toggling.

In Slave board, these steps are executed: 
 - Synchro with the Master
 - Receive requests from the master
 - Synchro with the Master
 - Send the ACK to the Master
 - Synchro with the Master
 - If the "ADDRCMD_MASTER_READ" command is received: 
   - Send the requested data "aTxSlaveBuffer" to the Master
   - Synchro with the Master
   - Receive the ACK from the Master
 - If the "ADDRCMD_MASTER_WRITE" command is received: 
   - Receive the requested data "aRxBuffer" from the Master
   - Synchro with the Master
   - Send the ACK to the Master
   - checks the correctness of data is performed and in case of mismatch between sent and
     received data, LED3 is On and communication is stopped (using infinite loop).
     Otherwise LED4 Toggles indicating that the data is received correctly.

These previously steps are repeated in an infinitely loop.

STM32F4-Discovery boards LEDs can be used to monitor the transfer status:
 - LED1 (green) toggles on Master board waiting for user button to be pushed.
 - LED1 (green) toggles on Master board at end of each loop.
 - LED4 (blue) toggles on both boards when the read comparison is OK.
 - LED3 (red) turns ON when there is an error in transmission/reception process.  

@note SPIx instance used and associated resources can be updated in "main.h"
       file depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, SPI, Full-duplex, Interrupt, Transmission, Reception, Master, Slave, MISO, MOSI,
Command, acknowledge

@par Directory contents 

  - SPI/SPI_FullDuplex_AdvComIT/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - SPI/SPI_FullDuplex_AdvComIT/Inc/stm32f4xx_it.h          SPI interrupt handlers header file
  - SPI/SPI_FullDuplex_AdvComIT/Inc/main.h                  Main program header file  
  - SPI/SPI_FullDuplex_AdvComIT/Src/stm32f4xx_it.c          SPI interrupt handlers
  - SPI/SPI_FullDuplex_AdvComIT/Src/main.c                  Main program
  - SPI/SPI_FullDuplex_AdvComIT/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - SPI/SPI_FullDuplex_AdvComIT/Src/stm32f4xx_hal_msp.c     HAL MSP module
  

@par Hardware and Software environment
  
  - This example runs on STM32F412xG devices.

  - This example has been tested with STM32412G-DISCOVERY board and can be
    easily tailored to any other supported device and development board.

  - STM32412G-DISCOVERY Set-up
    - Use the Joystick Selection Push Button 
    - Connect Master board PB13 (pin 37 in connector P1) to Slave Board PB13 
    - Connect Master board PB14 (pin 38 in connector P1) to Slave Board PB14
    - Connect Master board PB15 (pin 39 in connector P1) to Slave Board PB15
    - Connect Master board GND to Slave Board GND    
    
@par How to use it ?
 
In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */ 
