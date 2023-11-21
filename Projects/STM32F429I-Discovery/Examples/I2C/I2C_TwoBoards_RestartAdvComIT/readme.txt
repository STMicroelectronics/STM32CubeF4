/**
  @page I2C_TwoBoards_RestartAdvComIT I2C example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics  *******************
  * @file    I2C/I2C_TwoBoards_RestartAdvComIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C_TwoBoards_RestartAdvComIT I2C example.
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

How to perform multiple I2C data buffer transmission/reception between two boards, 
in interrupt mode and with restart condition.

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
  |   BUTTON_KEY            |                      |   BUTTON_KEY            |
  |                      GND|______________________|GND                      |
  |_STM32F429xx_____________|                      |_____________STM32F429xx_|

This example shows how to configure GPIO, DMA and I2C peripherals using 
2 STM32F429I-Discovery boards for exchanging some data
between an I2C Master device using Interrupt mode
and an I2C Slave device using Interrupt mode through the STM32F4xx I2C HAL API.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change I2C configuration.

The User push-button is used to initiate a communication between Master device to Slave.
User can initiate a new communication after each previous transfer completed.

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

Example execution:
On Master board side:
 - Wait User push-button to be pressed.

This action initiate a write request by Master through HAL_I2C_Master_Sequential_Transmit_IT() or a write then read request
through HAL_I2C_Master_Sequential_Transmit_IT() then HAL_I2C_Master_Sequential_Receive_IT() routine depends on Command Code type.
Initially at power on Slave device through Interrupt "Listen" the I2C bus to perform an acknowledge of Match Address when necessary.
This "Listen" action is initiated by calling HAL_I2C_EnableListen_IT().

Command code type is decomposed in two categories :
1- Action Command code
    a. Type of command which need an internal action from Slave Device without sending any specific answer to Master.
    b. I2C sequence is composed like that :
     _____________________________________________________________________________________
    |_START_|_Slave_Address_|_Wr_|_A_|_Command_Code_BYTE_1_|_A_|_Command_Code_BYTE_2_|_A_|....
     ________________________________
    |_Command_Code_BYTE_M_|_A_|_STOP_|

First of all, through HAL_I2C_Master_Sequential_Transmit_IT() routine, Master device generate an I2C start condition
with the Slave address and a write bit condition.
In Slave side, when address Slave match code is received on I2C1, an event interrupt (ADDR) occurs.
I2C1 IRQ Handler routine is then calling HAL_I2C_AddrCallback() which check Address Match Code and direction Write (Transmit)
to call the correct HAL_I2C_Slave_Sequential_Receive_IT() function.
This will allow Slave to enter in receiver mode and then acknowledge Master to send the Command code bytes through Interrupt.
The Command code data is received and treated byte per byte through HAL_I2C_SlaveRxCpltCallback() in Slave side until a STOP condition.

And so in Master side, each time the Slave acknowledge the byte received,
Master transfer the next data from flash memory buffer to I2C1 TXDR register until "Action Command code" Transfer completed.
Master auto-generate a Stop condition when transfer is achieved.

The STOP condition generate a STOP interrupt and initiate the end of reception on Slave side.
Thanks to HAL_I2C_ListenCpltCallback(), Slave is informed of the end of Communication with Master
and "Listen" mode is also terminated.

STM32F429I-Discovery boards LEDs can be used to monitor the transfer status :
Slave board side only :
   - LED3 is turned ON when an Address match code event is acknowledge.
   - LED3 is turned ON when the reception process is completed.
   - LED3 is then turned OFF when a Stop Condition is treated.

Master board side only :
   - LED3 is turned ON when the transmission process is completed.
 
Both side
 - LED4 is turned ON when there is an error in communication process.(communication is stopped if any, using infinite loop)

These LEDs status are kept at same value during 1 Second and then clear, this will allow to monitor a next transfer status.
 
Also only on Master board side, Terminal I/O can be used to watch the Action Command Code sent by Master and associated Slave action with IDE in debug mode.
Depending of IDE, to watch content of Terminal I/O note that
 - When resorting to EWARM IAR IDE:
 Command Code is displayed on debugger as follows: View --> Terminal I/O

 - When resorting to MDK-ARM KEIL IDE:
 Command Code is displayed on debugger as follows: View --> Serial Viewer --> Debug (printf) Viewer
 
 - When resorting to STM32CubeIDE:
 Command Code is displayed on debugger as follows: Window--> Show View--> Console.
 
 In Debug configuration : 
 
    - Window\Debugger, select the Debug probe : ST-LINK(OpenOCD)
    - Window\Startup,add the command "monitor arm semihosting enable"

- When resorting to Atollic TrueSTUIDIO IDE: debug --> view--> others-->SWV console

2- Request Command code :
    a. Type of command which need a specific data answer from Slave Device.
    b. I2C sequence is composed like that :
     _____________________________________________________________________________________
    |_START_|_Slave_Address_|_Wr_|_A_|_Command_Code_BYTE_1_|_A_|_Command_Code_BYTE_2_|_A_|....
     ______________________________________________________________________________
    |_Command_Code_BYTE_M_|_A_|_RESTART_|_Slave_Address_|_Rd_|_A_|_Data_BYTE_1_|_A_|...
     ___________________________________________
    |_Data_BYTE_2_|_A_|_Data_BYTE_N_|_NA_|_STOP_|

First of all, through HAL_I2C_Master_Sequential_Transmit_IT() routine, Master device generate an I2C start condition
with the Slave address and a write bit condition.
In Slave side, when address Slave match code is received on I2C1, an event interrupt (ADDR) occurs.
I2C1 IRQ Handler routine is then calling HAL_I2C_AddrCallback() which check Address Match Code and direction Write (Transmit)
to call the correct HAL_I2C_Slave_Sequential_Receive_IT() function.
This will allow Slave to enter in receiver mode and then acknowledge Master to send the Command code bytes through Interrupt.
The Command code data is received and treated byte per byte through HAL_I2C_SlaveRxCpltCallback() in Slave side.
If data received match with a Internal Command Code, set the associated index, which will use for Transmission process when requested by Master

And so in Master side, each time the Slave acknowledge the byte received,
Master transfer the next data from flash memory buffer to I2C1 TXDR register until "Request Command code" transfer completed.

Then through HAL_I2C_Master_Sequential_Receive_IT() routine, Master device generate a RESTART condition
with Slave address and a read bit condition.
In Slave side, when address Slave match code is received on I2C1, an event interrupt (ADDR) occurs.
I2C1 IRQ Handler routine is then calling HAL_I2C_AddrCallback() which check Address Match Code and direction Read (Reception)
to call the correct HAL_I2C_Slave_Sequential_Transmit_IT() function.
Slave enter in transmitter mode and send send answer bytes through interrupt until end of transfer size.
Master auto-generate a NACK and STOP condition to inform the Slave that the transfer and communication are finished.

The STOP condition generate a STOP interrupt and initiate the end of reception on Slave side.
Thanks to HAL_I2C_ListenCpltCallback(), Slave is informed of the end of Communication with Master
and "Listen" mode is also terminated.

STM32F429I-Discovery boards LEDs can be used to monitor the transfer status in both side:
Slave board side :
   - LED3 is turned ON when an Address match code event is acknowledge.
   - LED3 is turned ON when the reception process is completed.
   - LED3 is turned ON when the transmission process is completed.
   - LED3 is then turned OFF when a Stop Condition is treated.

Master board side :
   - LED3 is turned ON when the transmission process is completed.
   - LED3 is turned ON when the reception process is completed.
 
Both side
 - LED4 is turned ON when there is an error in communication process.(communication is stopped if any, using infinite loop)

These LEDs status are keeped at same value during 1 Second and then clear, this will allow to monitor a next transfer status.

Also only on Master board side, Terminal I/O can be used to watch the Request Command Code sent by Master and associated Slave answer with IDE in debug mode.
 - When resorting to EWARM IAR IDE:
 Command Code is displayed on debugger as follows: View --> Terminal I/O

 - When resorting to MDK-ARM KEIL IDE:
 Command Code is displayed on debugger as follows: View --> Serial Viewer --> Debug (printf) Viewer
 
 - When resorting to STM32CubeIDE:
 Command Code is displayed on debugger as follows: Window--> Show View--> Console. 
 
  In Debug configuration : 

  - Window\Debugger, select the Debug probe : ST-LINK(OpenOCD)
  - Window\Startup,add the command "monitor arm semihosting enable"

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

Connectivity, I2C, Communication, Transmission, Reception, SCL, SDA, Interrupt, Request, Sequential

@par Directory contents 

  - I2C/I2C_TwoBoards_RestartAdvComIT/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - I2C/I2C_TwoBoards_RestartAdvComIT/Inc/stm32f4xx_it.h          I2C interrupt handlers header file
  - I2C/I2C_TwoBoards_RestartAdvComIT/Inc/main.h                  Header for main.c module  
  - I2C/I2C_TwoBoards_RestartAdvComIT/Src/stm32f4xx_it.c          I2C interrupt handlers
  - I2C/I2C_TwoBoards_RestartAdvComIT/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_RestartAdvComIT/Src/system_stm32f4xx.c      STM32F4xx system source file
  - I2C/I2C_TwoBoards_RestartAdvComIT/Src/stm32f4xx_hal_msp.c     HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F429xx devices.
    
  - This example has been tested with STM32F429I-Discovery board and can be
    easily tailored to any other supported device and development board.    

  -STM32F429I-Discovery Set-up
    - Connect I2C_SCL line of Master board (PB6, P1) to I2C_SCL line of Slave Board (PB6, P1).
    - Connect I2C_SDA line of Master board (PB9, P1) to I2C_SDA line of Slave Board (PB9, P1).
    - Connect GND of Master board to GND of Slave Board.
	- SB9 must be closed to enable the SWO.

  - Launch the program in debug mode on Master board side, and in normal mode on Slave side
  to benefit of Terminal I/O information.
  - Press User push-button to initiate a communication by Master device to Slave device.
      User can initiate a new communication after each previous transfer completed.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
