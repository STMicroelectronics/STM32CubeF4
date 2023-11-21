/**
  @page I2C_TwoBoards_RestartComIT I2C Two Boards Communication IT  with Restart condition example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    I2C/I2C_TwoBoards_RestartComIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C Two Boards IT Communication with Restart condition example.
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

How to handle single I2C data buffer transmission/reception between two boards, 
in interrupt mode and with restart condition.

   _________________________                        _________________________
  |           ______________|                      |______________           |
  |          | I2C1         |                      |          I2C1|          |
  |          |              |                      |              |          |
  |          |      SCL(PB8)|______________________|(PB8)SCL      |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |      SDA(PB9)|______________________|(PB9)SDA      |          |
  |          |              |                      |              |          |
  |          |______________|                      |______________|          |
  |      __                 |                      |      __                 |
  |     |__|                |                      |     |__|                |
  |   BUTTON_WAKEUP         |                      |   BUTTON_WAKEUP         |
  |                      GND|______________________|GND                      |
  |_STM32F412xx_____________|                      |_____________STM32F412xx_|

This example guides you through the different configuration steps by mean of HAL API 
to ensure I2C Data buffer transmission and reception using Interrupt.
The communication is done with 2 Boards through I2C.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 100 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change I2C configuration.

The I2C communication is then initiated.
The project is split in two parts the Master Board and the Slave Board
- Master Board
  The HAL_I2C_Master_Sequential_Transmit_IT() and the HAL_I2C_Master_Sequential_Receive_IT() functions 
  allow respectively the transmission and the reception of a predefined data buffer
  in Master mode.
- Slave Board
  The HAL_I2C_EnableListen_IT(), HAL_I2C_Slave_Sequential_Receive_IT() and the HAL_I2C_Slave_Sequential_Transmit_IT() functions 
  allow respectively the "Listen" the I2C bus for address match code event, reception and the transmission of a predefined data buffer
  in Slave mode.
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file.
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.
                       
On Master board side:
 - Wait Joystick sel-button to be pressed.
 - Request write communication after a Start condition by sending aTxBuffer through HAL_I2C_Master_Sequential_Transmit_IT() to I2C Slave 
   - LED1 is turned ON when the transmission process is completed.
 - Then wait Joystick sel-button to be pressed.
 - Request read communication after a Repeated Start condition through HAL_I2C_Master_Sequential_Receive_IT() to I2C Slave.
 - Receive data from Slave in aRxBuffer.
   - LED2 is turned ON when the reception process is completed.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to 
check buffers correctness.
LED3 is turned ON when there is an error in transmission/reception process,
or buffers compared error. (communication is stopped if any, using infinite loop)

On Slave board side:
 - Go Listen mode by calling HAL_I2C_EnableListen_IT().
 - Acknowledge Address match code.
   - LED4 is turned ON when an Address match code event is acknowledge.
 - Slave receives data into aRxBuffer through HAL_I2C_Slave_Sequential_Receive_IT().
   - LED2 is turned ON when the reception process is completed.
 - Then slave transmits data by sending aTxBuffer through HAL_I2C_Slave_Sequential_Transmit_IT().
   - LED1 is turned ON when the transmission process is completed.
   - LED4 is turned OFF when the communication is completed.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to 
check buffers correctness.
LED3 is turned ON when there is an error in transmission/reception process,
or buffers compared error. (communication is stopped if any, using infinite loop)

 @note I2Cx instance used and associated resources can be updated in "main.h"
       file depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, I2C, Communication, Transmission, Reception, SCL, SDA, Interrupt

@par Directory contents 

  - I2C/I2C_TwoBoards_RestartComIT/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - I2C/I2C_TwoBoards_RestartComIT/Inc/stm32f4xx_it.h          I2C interrupt handlers header file
  - I2C/I2C_TwoBoards_RestartComIT/Inc/main.h                  Header for main.c module  
  - I2C/I2C_TwoBoards_RestartComIT/Src/stm32f4xx_it.c          I2C interrupt handlers
  - I2C/I2C_TwoBoards_RestartComIT/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_RestartComIT/Src/system_stm32f4xx.c      STM32F4xx system source file
  - I2C/I2C_TwoBoards_RestartComIT/Src/stm32f4xx_hal_msp.c     HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F412xx devices.
    
  - This example has been tested with STM32412G-DISCOVERY board and can be
    easily tailored to any other supported device and development board.    

  -STM32412G-DISCOVERY Set-up
    - Connect I2C_SCL line of Master board (PB8, P2) to I2C_SCL line of Slave Board (PB8, P2).
    - Connect I2C_SDA line of Master board (PB9, P2) to I2C_SDA line of Slave Board (PB9, P2).
    - Connect GND of Master board to GND of Slave Board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
