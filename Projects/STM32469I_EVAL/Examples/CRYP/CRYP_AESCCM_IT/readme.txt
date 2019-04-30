/**
  @page CRYP_AESCCM_IT  AES CCM  Example using IT
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    CRYP/CRYP_AES_CCM_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES CCM encryption/decryption and TAG 
  *          generation.
  ******************************************************************************
  * 
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 


How to use the CRYPTO peripheral to encrypt/decrypt data(Plaintext/Ciphertext) in
interrupt mode using AES with Combined Cipher Machine (CCM) then generate the authentication TAG .

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32F479xx Devices.

The AES-CCM mode requires:
1. Plaintext/Ciphertext: which will be authenticated and encrypted/decrypted. 
   It's size must be a multiple of 4 words. if it's not it must be padded.
    
2. B1 block or Header ( also named associated data): that will be authenticated 
   but not encrypted.

3. B0 block : first block B0 is formatted by Flags, Nonce and the octet length
   of Plaintext.

4. Key: is the parameter which determines the cipher text. In this example the key
   size is 192 bits = 6 words.

CRYP peripheral must be initialized once from the beginning, then for each operation of 
encryption/decryption, only configuration(HAL_CRYP_SetConfig) should be made if needed.
 
The AES-CCM encryption and TAG generation provides :
1. Encryptedtext: which is the encryption result of Plaintext, it is compared to Ciphertext.
2. TAG: the Message Authentication Code generated after the encryption and it's compared
   to ExpectedTAG.

The AES-CCM decryption and TAG generation provides :
1. Decryptedtext: which is the Decryption result of Ciphertext, it is compared to Plaintext.
2. TAG: the Message Authentication Code generated after the encryption and it's compared
   to ExpectedTAG.

 The encryption/decryption is performed in non-blocking mode using the HAL function
 "HAL_CRYP_Encrypt_IT" and "HAL_CRYP_Decrypt_IT". when compute is complete the
 output FIFO transfer completed callbacks set variable  "CrypCompleteDetected" 
 to 1 then result is compared to the expected buffer. 
   
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

@par Directory contents 
  
  - CRYP/CRYP_AESCCM_IT/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AESCCM_IT/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AESCCM_IT/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_AESCCM_IT/Src/stm32f4xx_it.c          Interrupt handlers
  - CRYP/CRYP_AESCCM_IT/Src/main.c                  Main program
  - CRYP/CRYP_AESCCM_IT/Src/stm32f4xx_hal_msp.c     HAL MSP module 
  - CRYP/CRYP_AESCCM_IT/Src/system_stm32f4xx.c      STM32F4xx system source file

     
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
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 