/**
  @page CRYP_TDESModes  TDES ECB/CBC Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    CRYP/CRYP_TDESModes/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP TDES encryption/decryption mode
  *          with ECB and CBC algorithm.
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


How to use the CRYPTO peripheral to encrypt/decrypt data(Plaintext/Ciphertext) using 
TDES ECB and CBC algorithm.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32F479xx Devices.


The TDES mode requires:
1. Plaintext : which will be encrypted with several algorithm & keys.    
   Ciphertext_ECB,Ciphertext_CBC : which will be decrypted respectively with ECB
   and CBC algorithm. 

3. InitVector or the Initialization Vector used only for CBC.

4. Key: is the parameter which determines the Ciphertext. In this example key
   size is 128 bits.

CRYP peripheral must be initialized (HAL_CRYP_Init) once from the beginning with
user parameter, then for each operation of encryption/decryption, only configuration
(HAL_CRYP_SetConfig) should be made if needed.
Before setting new configuration via HAL_CRYP_SetConfig function, we should get
the current parameters of CRYP via HAL_CRYP_GetConfig function.
 
The TDES-ECB encryption/decryption in polling mode provide :
1. Encryptedtext: which is the encryption result of Plaintext, it is compared to Ciphertext_ECB.
2. Decryptedtext: which is the Decryption result of Ciphertext_ECB,it is compared to Plaintext.

The TDES-CBC encryption/decryption in polling mode provide :
1. Encryptedtext: which is the encryption result of Plaintext, it is compared to Ciphertext_CBC.
2. Decryptedtext: which is the Decryption result of Ciphertext_CBC,it is compared to Plaintext.
   
STM32469I-EVAL LEDs are used to monitor the encryption/decryption and TAG generation  status:
 - LED1(GREEN) is ON when encryption/decryption and TAG generation is right.
 - LED3(RED) is ON when encryption or decryption or TAG generation is wrong.
	       
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
    
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Security, CRYP, TDES, ECB, CBC, hardware CRYP, 

@par Directory contents 
  
  - CRYP/CRYP_TDESModes/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_TDESModes/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_TDESModes/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_TDESModes/Src/stm32f4xx_it.c          Interrupt handlers
  - CRYP/CRYP_TDESModes/Src/main.c                  Main program
  - CRYP/CRYP_TDESModes/Src/stm32f4xx_hal_msp.c     HAL MSP module 
  - CRYP/CRYP_TDESModes/Src/system_stm32f4xx.c      STM32F4xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32F479xx devices.
  
  - This example has been tested with an STMicroelectronics STM32469I-EVAL RevB
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  

  */
