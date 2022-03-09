/**
  @page CEC CEC_ListenMode example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    CEC/CEC_ListenMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CEC Listen Mode example.
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

How to configure and use the CEC peripheral to receive and transmit messages 
between two boards, while a third board (the spy device) listens but doesn't 
acknowledge the received messages.

- Hardware Description

To use this example, three STM32446E-EVAL boards (called Device_1, Device_2 and
Device_3) are loaded with the same software then connected through CEC lines
(PB3 which is connected to PB6) and GND.
 /|\  In the firmware file main.h, uncomment the dedicated line to use
/_!_\ the CEC peripheral as STM32 device_1, STM32 device_2 or STM32 device_3.

@verbatim
*------------------------------------------------------------------------------*
|           STM32446E-EVAL                               STM32446E-EVAL        |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |     __________     |                 |     __________     |         |
|        |    |          |PB3 |                 |PB3 |          |    |         |
|        |    |   CEC    |____|____CECLine______|____|   CEC    |    |         |
|        |    | Device_1 |    |            |    |    | Device_2 |    |         |
|        |    |__________|    |            |    |    |__________|    |         |
|        |                    |            |    |                    |         |
|        |                    |            |    |                    |         |
|        |             GND O--|------------|--0-|--O GND             |         |
|        |____________________|            |  | |____________________|         |
|                                          |  |                                |
|          STM32446E-EVAL                  |  |                                |
|         ____________________             |  |                                |
|        |                    |            |  |                                |
|        |     __________     |            |  |                                |
|        |    |          |PB3 |            |  |                                |
|        |    |   CEC    |____|____CECLine_|  |                                |
|        |    | Device_3 |    |               |                                |
|        |    |__________|    |               |                                |
|        |                    |               |                                |
|        |                    |               |                                |
|        |             GND O__|_______________|                                |
|        |____________________|                                                |
*------------------------------------------------------------------------------*
@endverbatim



- Software Description

The test unrolls as follows.

On TX side, four possible messages can be transmitted and are indicated as
below on the transmitting board:
 - when User push-button is pressed, LED1 toggles
 - when Joystick Selection push-button is pressed, LED2 toggles
 - when Joystick UP push-button is pressed, LED3 toggles
 - when Joystick DOWN push-button is pressed, LED4 toggles


Device_1 sends messages addressed to Device_2,
Device_2 sends messages addressed to Device_1.
Both Device_1 and Device_2 acknowledge the received messages.


Accordingly, the following happens on the RX side (i.e. on both receiving boards,
one which acknowledges the message and the other one which doesn't) in case of successful
reception:
 - when User push-button is pressed on TX side, 
     * all RX side LEDs are turned off 
 - when Joystick Selection push-button is pressed on TX side, on RX side
     *  LED1 and LED2 are turned on
     *  LED3 and LED4 are turned off 
 - when Joystick UP push-button is pressed on TX side, 
     *  all RX side LEDs are turned on
 - when Joystick DOWN push-button is pressed on TX side, on RX side 
     * LED1 and LED2 are turned off
     * LED3 and LED4 are turned on    
In case of unsuccessful reception, LED3 is turned on.


In case Device_3 (the spy device) transmits, only Device_1 reacts
to the received messages. 


Practically, 2 EXTI lines (EXTI15_10 and EXTI0) are configured to 
generate an interrupt on each falling or rising edge. 
A specific message is then transmitted by the CEC IP
and a LED connected to a specific MFX GPIO pin is toggled.
    - EXTI0 is mapped to MFX used to manage Joystick pins
    - EXTI15_10 is mapped to PC.13

Then, on TX side,
  - when rising edge is detected on EXTI0-SEL joystick button, LED2 toggles
  - when falling edge is detected on EXTI15_10 due to line PC.13, LED1 toggles
  - when falling edge is detected on EXTI0-UP joystick button LED3 toggles
  - when falling edge is detected on EXTI0-DOWN joystick button LED4 toggles
   

In this example, HCLK is configured at 180 MHz.

@Known Limitation: Currently there is an issue when pressing the joystick/button on Device_1,
  just after Device_3 has been used (button pressed): there will be a transmission error.
  It is necessary to use (press joystick/button) on Device_2, then Device_1 becomes operative again.
  
@par Keywords

Connectivity, CEC, Listen mode, Transmission, Reception, Joystick, Data exchange

@par Directory contents 

  - CEC/CEC_ListenMode/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - CEC/CEC_ListenMode/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - CEC/CEC_ListenMode/Inc/main.h                  Header for main.c module  
  - CEC/CEC_ListenMode/Src/stm32f4xx_it.c          Interrupt handlers
  - CEC/CEC_ListenMode/Src/system_stm32f4xx.c      STM32F4xx system source file
  - CEC/CEC_ListenMode/Src/main.c                  Main program
  - CEC/CEC_ListenMode/Src/stm32f4xx_hal_msp.c     IP hardware resources initialization
  
@par Hardware and Software environment

  - This example runs on STM32F446xx Devices.
    
  - This example has been tested with STM32446E-EVAL board and can be
    easily tailored to any other supported device and development board.      


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - in main.h: uncomment DEVICE_1 for first board, uncomment DEVICE_2 for second board, uncomment DEVICE_3 for third board
 - Rebuild all files and load your image into target memory
 - Be aware that PB6 pin is missing but PB3 is connected directly to it (SB23 is closed).
 - With a wire, connect PB3-PB3-PB3 between the 3 boards
 - Add a pull-up resistor of 27kohm between PB3 and V3.3
 - Connect the ground of the 2 boards
 - Run the example


 */
