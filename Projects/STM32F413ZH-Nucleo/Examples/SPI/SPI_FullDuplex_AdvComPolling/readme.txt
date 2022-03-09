/**
  @page SPI_FullDuplex_AdvComPolling SPI Full Duplex Polling example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    SPI/SPI_FullDuplex_AdvComPolling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI Full Duplex Polling example.
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
to transmit/receive SPI data buffer in Polling mode and in an advanced communication mode:
the master board always sends the command to the slave before any transmission is performed.
The slave board sends an acknowledge before going further.

- Hardware Description

To use this example, you need to load it on two STM32 boards and connect them
through SPI lines and GND.

   _________________________                        _________________________
  |           ______________|                      |______________           |
  |          |SPI1          |                      |          SPI1    |      |
  |          |              |                      |                  |      |
  |          |     CLK(PB3) |______________________|(PB3)CLK  CN7.15  |      |
  |          |              |                      |                  |      |
  |          |     MISO(PA6)|______________________|(PA6)MISO CN7.D12 |      |
  |          |              |                      |                  |      |
  |          |     MOSI(PA7)|______________________|(PA7)MOSI CN7.D11 |      |
  |          |              |                      |                  |      |
  |          |______________|                      |__________________|      |
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
the configuration of the needed SPI resources according to the used hardware (CLOCK & 
GPIO). You may update this function to change SPI configuration.

The SPI peripheral is configured in full duplex mode with baudrate = FCPU/256. 

If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.
                       
In Master board, when the user button is pressed.
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
   LED3 is On and communication is stopped (using infinite loop). Otherwise LED2 is Toggled
   indicating that the data is received correctly.
 - Synchro with the Slave
 - Requests write operation by sending specific command to the slave that contain
   the master write command and the number of data to be written
 - Synchro with the Slave 
 - Receive the ACK from the Slave
 - Synchro with the Slave 
 - Send the requested data "aTxMasterBuffer" to the slave
 - Synchro with the Slave 
 - Receive the ACK from the Slave
These steps are repeated with each button press and a delay 100ms is inserted at
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
     Otherwise LED2 Toggles indicating that the data is received correctly.

These steps are repeated with each button press.

NUCLEO-F413ZH boards LEDs can be used to monitor the transfer status:
 - LED1 (green) toggles on Master board waiting for user button to be pushed.
 - LED1 (green) toggles on Master board at end of each loop.
 - LED2 (blue) toggles on both boards when the read comparison is OK.
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

Connectivity, SPI, Full-duplex, Polling, Transmission, Reception, Master, Slave, MISO, MOSI,
Command, acknowledge

@par Directory contents 

  - SPI/SPI_FullDuplex_AdvComPolling/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - SPI/SPI_FullDuplex_AdvComPolling/Inc/stm32f4xx_it.h          SPI interrupt handlers header file
  - SPI/SPI_FullDuplex_AdvComPolling/Inc/main.h                  Main program header file  
  - SPI/SPI_FullDuplex_AdvComPolling/Src/stm32f4xx_it.c          SPI interrupt handlers
  - SPI/SPI_FullDuplex_AdvComPolling/Src/main.c                  Main program
  - SPI/SPI_FullDuplex_AdvComPolling/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - SPI/SPI_FullDuplex_AdvComPolling/Src/stm32f4xx_hal_msp.c     HAL MSP module
  

@par Hardware and Software environment
  
  - This example runs on STM32F413xx/STM32F423xx devices.
      
  - This example has been tested with NUCLEO-F413ZH board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F413ZH Rev B Set-up
    - Use LED1, LED2 and LED3 connected respectively to PA.5, PB.7, PB.14
    - Use the User Push Button 
    - Connect Master board PB3 to Slave Board PB3 (Stuino CN7, pin 15)
    - Connect Master board PA6 to Slave Board PA6 (Arduino D12)
    - Connect Master board PA7 to Slave Board PA7 (Arduino D11)
    - Connect Master board GND to Slave Board GND    
    
@par How to use it ?
 
In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */ 
