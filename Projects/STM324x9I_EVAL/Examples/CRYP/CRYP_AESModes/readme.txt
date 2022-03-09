/**
  @page CRYP_AESModes  AES ECB/CBC & CTR  Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    CRYP/CRYP_AESmodes/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES encryption/decryption mode
  *          with ECB, CBC and CTR algorithm.
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


How to use the CRYP peripheral to encrypt/decrypt data(Plaintext/Ciphertext) using AES
ECB, CBC and CTR algorithm.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32F439xx Devices.

The AES mode requires:
1. Plaintext/Ciphertext: which will be encrypted with several algorithm & keys.    
   CiphertextAESECB128,CiphertextAESCBC192 and CiphertextAESCTR256 : which will 
   be decrypted respectively with ECB algorithm & 128bits key, CBC algorithm &
   192bits key and CTR algorithm & 256 key. 

3. InitVector or the Initialization Vector used only for CBC and CTR.

4. Key: is the parameter which determines the Ciphertext. In this example several
   key size are used,  128 bits for ECB, 192 bits for CBC and 256 bits for CTR.

CRYP peripheral must be initialized once from the beginning, then for each 
operation of encryption/decryption, only configuration should be made if needed.
 
The AES-ECB encryption/decryption in polling mode provide :
1. Encryptedtext: which is the encryption result of Plaintext, it is compared 
   to CiphertextAESECB128.
2. Decryptedtext: which is the Decryption result of Ciphertext,it is compared
   to Plaintext.

The AES-CBC encryption/decryption in DMA mode provide :
1. Encryptedtext: which is the encryption result of Plaintext, it is compared
   to CiphertextAESCBC192.
2. Decryptedtext: which is the Decryption result of Ciphertext,it is compared 
   to Plaintext.

The AES-CTR encryption/decryption in interrupt mode provide:
1. Encryptedtext: which is the encryption result of Plaintext, it is compared
   to CiphertextAESCTR256.
2. Decryptedtext: which is the Decryption result of Ciphertext,it is compared
   to Plaintext.
   
STM324x9I-EVAL LEDs are used to monitor the encryption/decryption  status:
 - LED1(GREEN) is ON when encryption/decryption  are right.
 - LED3(RED) is ON when encryption or decryption are wrong.
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
            
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Security, CRYP, AES, ECB, CBC,CTR, NIST FIPS publication 197, hardware CRYP,

@par Directory contents 
  
  - CRYP/CRYP_AESModes/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AESModes/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AESModes/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_AESModes/Src/stm32f4xx_it.c          Interrupt handlers
  - CRYP/CRYP_AESModes/Src/main.c                  Main program
  - CRYP/CRYP_AESModes/Src/stm32f4xx_hal_msp.c     HAL MSP module 
  - CRYP/CRYP_AESModes/Src/system_stm32f4xx.c      STM32F4xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32F439xx devices.
  
  - This example has been tested with an STMicroelectronics STM324x9I-EVAL RevB
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  

  */
