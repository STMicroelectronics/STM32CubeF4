/**
  @page CRYP_TDESModes  TDES ECB/CBC Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    CRYP/CRYP_TDESModes/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP TDES encryption/decryption mode
  *          with ECB and CBC algorithm.
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


How to use the CRYPTO peripheral to encrypt/decrypt data(Plaintext/Ciphertext) using 
TDES ECB and CBC algorithm.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32F439xx Devices.


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
   
STM324x9I-EVAL LEDs are used to monitor the encryption/decryption and TAG generation  status:
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

  - This example runs on STM32F439xx devices.
  
  - This example has been tested with an STMicroelectronics STM324x9I-EVAL RevB
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 