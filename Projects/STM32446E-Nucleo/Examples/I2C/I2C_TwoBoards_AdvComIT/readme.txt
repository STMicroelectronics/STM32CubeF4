/**
  @page I2C_TwoBoards_AdvComIT I2C Two Boards Advanced Communication IT example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    I2C/I2C_TwoBoards_AdvComIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C Two Boards Advanced Communication with 
  *          Interrupt example.
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
 
How to handle I2C data buffer transmission/reception between two boards, 
using an interrupt.

   _________________________                        _________________________
  |           ______________|                      |______________           |
  |          | I2C1         |                      |          I2C1|          |
  |          |              |                      |              |          |
  |          |      SCL(PB6)|______________________|(PB6)SCL      |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |      SDA(PB9)|______________________|(PB9)SDA      |          |
  |          |              |                      |              |          |
  |          |______________|                      |______________|          |
  |      __                 |                      |      __                 |
  |     |__|                |                      |     |__|                |
  |     USER                |                      |     USER                |
  |                      GND|______________________|GND                      |
  |_STM32F4 ________________|                      |_________________STM32F4_|


The communication is done with 2 Boards through I2C and using the following I2C features:
  - 7-bit addressing mode
  - Fast speed mode (400KHz)
  - Interrupting capability

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which implements
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change I2C configuration.

The I2C communication is then initiated.
The project is split in two parts the Master Board and the Slave Board
- Master Board
  The HAL_I2C_Master_Receive_IT() and the HAL_I2C_Master_Transmit_IT() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Master mode using interrupt.
- Slave Board
  The HAL_I2C_Slave_Receive_IT() and the HAL_I2C_Slave_Transmit_IT() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Slave mode using interrupt.
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:
- Uncomment "#define MASTER_BOARD" to select Master board.
- Comment "#define MASTER_BOARD" to select Slave board.

For this example two buffers are used 
- aTxBuffer buffer is used to save the data to be transmitted 
- aRxBuffer buffer is used to save the received data
Note that the two buffers have the same size 
                       
In Master board side:
 - LED2 toggles every 1sec on master board, waiting for user button to be pressed 
   (used only for synchronization at startup)
 - Requests write operation by sending specific command "MASTER_REQ_WRITE" to Slave
 - Sends the number of data to be written
 - Transmits aTxBuffer buffer to slave
 - Requests read operation by sending specific command "MASTER_REQ_READ" to Slave
 - Sends the number of data to be read
 - Receives data from Slave in aRxBuffer
 - Checks the correctness of data and Toggle LED2 when data is received correctly, 
   otherwise it turns ON LED2 and communication is stopped (using infinite loop)

In Slave board side:
 - Receives request from Master
 - Receives the request operation from Master and depending on the operation type (write or read):
   - If Master requests write operation:
      - Receives number of data to be written by Master
      - Receives data from master in aRxBuffer
      - Checks the correctness of data and Toggle LED2 when data is received correctly
        otherwise it turns ON LED2 and communication is stopped (using infinite loop)
   - If Master requests read operation:
      - Receives number of data to be written to Master
      - Transmits aTxBuffer buffer to master

These operations are repeated periodically and the start of communication is triggered 
by pushing the user button of the Master board.       

 @note In Master side, only Acknowledge failure error is handled. When this error
       occurs Master restart the current operation until Slave acknowledges its
       address.
        
 @note I2Cx instance used and associated resources can be updated in "main.h"
       file depending on hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, Communication, I2C, Interrupt, Master, Slave, Transmission, Reception, Fast mode,
Command, Acknowledge

@par Directory contents 

  - I2C/I2C_TwoBoards_AdvComIT/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - I2C/I2C_TwoBoards_AdvComIT/Inc/stm32f4xx_it.h          I2C interrupt handlers header file
  - I2C/I2C_TwoBoards_AdvComIT/Inc/main.h                  Main program header file  
  - I2C/I2C_TwoBoards_AdvComIT/Src/stm32f4xx_it.c          I2C interrupt handlers
  - I2C/I2C_TwoBoards_AdvComIT/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_AdvComIT/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - I2C/I2C_TwoBoards_AdvComIT/Src/stm32f4xx_hal_msp.c     HAL MSP module    


@par Hardware and Software environment

  - This example runs on STM32F446xx Devices.
    
  - This example has been tested with STM32F4xx-Nucleo board rev C and can be
    easily tailored to any other supported device and development board.

  - For This example to work properly, a 2.2 µF Capacitor must be present on the board in slot C26.    
      
  - STM32F4xx-Nucleo Set-up
    - Connect Master board PB6 to Slave Board PB6
    - Connect Master board PB9 to Slave Board PB9
    - Connect Master board GND to Slave Board GND


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
