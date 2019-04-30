/**
  @page CRYP_AESModes_DMA  AES ECB in DMA mode  Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    CRYP/CRYP_AESModes_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES ECB encryption/decryption in DMA mode
  *          
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


How to use the CRYPTO peripheral to encrypt/decrypt data(Plaintext/Ciphertext) using AES
ECB algorithm  in DMA mode with swapping.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32F479xx   Devices

The AES ECB requires:
1. Plaintext     : which will be encrypted with ECB algorithm & 128bits key.    
   Ciphertext    : which will be decrypted with ECB algorithm & 128bits key.
   Data Byte swapping (DataType 8):
   Plaintext_8   : which will be encrypted with ECB algorithm & 128bits key.    
   Ciphertext_8  : which will be decrypted with ECB algorithm & 128bits key.  
   Data half-word swapping (DataType 16):
   Plaintext_16  : which will be encrypted with ECB algorithm & 128bits key.    
   Ciphertext_16 : which will be decrypted with ECB algorithm & 128bits key.
   Data bit swapping (DataType 1):
   Plaintext_1   : which will be encrypted with ECB algorithm & 128bits key.    
   Ciphertext_1  : which will be decrypted with ECB algorithm & 128bits key.
   
2. Key: is the parameter which determines the Ciphertext. In this example 128 bits
   key is used

CRYP peripheral must be initialized once from the beginning, then for each 
operation of encryption/decryption, only configuration should be made if needed.
 
The AES-ECB encryption/decryption in DMA mode provide :
   Encryptedtext: which is the encryption result of Plaintext, it is compared
   to Ciphertext.
   Decryptedtext: which is the Decryption result of Ciphertext,it is compared
   to Plaintext.
1. No swapping, Encryptedtext compared to Ciphertext. then Decryptedtext compared to Plaintext.
2. Byte swapping, Encryptedtext compared to Ciphertext_8. then Decryptedtext compared to Plaintext_8.
3. half-word swapping, Encryptedtext compared to Ciphertext_16. then Decryptedtext compared to Plaintext_16.
4. Bit swapping, Encryptedtext compared to Ciphertext_1. then Decryptedtext compared to Plaintext_1.

STM32469I-EVAL LEDs are used to monitor the encryption/decryption  status:
 - LED1(GREEN) is ON when encryption/decryption  are right.
 - LED3(RED) is ON when encryption or decryption are wrong.
      
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 
  
  - CRYP/CRYP_AESModes_DMA/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AESModes_DMA/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AESModes_DMA/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_AESModes_DMA/Src/stm32f4xx_it.c          Interrupt handlers
  - CRYP/CRYP_AESModes_DMA/Src/main.c                  Main program
  - CRYP/CRYP_AESModes_DMA/Src/stm32f4xx_hal_msp.c     HAL MSP module 
  - CRYP/CRYP_AESModes_DMA/Src/system_stm32f4xx.c      STM32F4xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32F479xx  devices.
  
  - This example has been tested with an STMicroelectronics STM32469I-EVAL RevC
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 