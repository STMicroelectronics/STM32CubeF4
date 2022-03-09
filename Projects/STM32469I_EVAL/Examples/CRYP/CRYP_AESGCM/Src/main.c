/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESGCM/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the STM32F4xx CRYP HAL API
  *          to encrypt/decrypt data and get TAG using AES in GCM Algorithm.
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
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup CRYP_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRYP handler declaration */
CRYP_HandleTypeDef hcryp;

/* AES Key  */
uint32_t AES128Key[4] = {0xfeffe992,0x8665731c,0x6d6a8f94,0x67308308};

/* below InitVector is the Initial Counter Block (ICB)composed of IV and counter*/
uint32_t InitVector[4] = {0xcafebabe,0xfacedbad,0xdecaf888 , 0x00000002};

/* Header */
uint32_t HeaderMessage[5] = {0xfeedface, 0xdeadbeef, 0xfeedface, 0xdeadbeef,0xabaddad2};

/* Plaintext */
uint32_t Plaintext[15] = {0xd9313225,0xf88406e5,0xa55909c5,0xaff5269a
                         ,0x86a7a953,0x1534f7da,0x2e4c303d,0x8a318a72
                         ,0x1c3c0c95,0x95680953,0x2fcf0e24,0x49a6b525
                         ,0xb16aedf5,0xaa0de657,0xba637b39};

/* Expected GCM Ciphertext with AESKey128 */
uint32_t Ciphertext[15] = {0x42831ec2,0x21777424,0x4b7221b7,0x84d0d49c
                          ,0xe3aa212f,0x2c02a4e0,0x35c17e23,0x29aca12e
                          ,0x21d514b2,0x5466931c,0x7d8f6a5a,0xac84aa05
                          ,0x1ba30b39,0x6a0aac97,0x3d58e091}; 
/* Expected GCM TAG */
uint32_t ExpectedTAG[4]={0x5bc94fbc,0x3221a5db,0x94fae95a,0xe7121a47};

/* Used for storing the encrypted text */
uint32_t Encryptedtext[16]={0}; 

/* Used for storing the decrypted text */
uint32_t Decryptedtext[16]={0};

/* Used for storing the computed MAC (TAG) */
uint32_t TAG[4]={0};

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 180 MHz */
  SystemClock_Config();
  
  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  
  /*## Set the CRYP parameters and initialize the CRYP IP  ###################*/ 

  hcryp.Instance             = CRYP;
  hcryp.Init.DataType        = CRYP_DATATYPE_32B;
  hcryp.Init.KeySize         = CRYP_KEYSIZE_128B;
  hcryp.Init.pKey            = AES128Key; 
  hcryp.Init.Algorithm       = CRYP_AES_GCM;
  hcryp.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ALWAYS;
  hcryp.Init.pInitVect       = InitVector;
  hcryp.Init.Header          = HeaderMessage;
  hcryp.Init.HeaderSize      = 5;

  /* Configure the CRYP  */
  HAL_CRYP_Init(&hcryp);
  
  /*## AES GCM Encryption and TAG generation  ################################*/ 
  
  /* Encryption, result in  Encryptedtext buffer */    
  HAL_CRYP_Encrypt(&hcryp, Plaintext, 15, Encryptedtext, TIMEOUT_VALUE);
  
  /*Compare results with expected buffer*/ 
  if(memcmp(Encryptedtext, Ciphertext, 60) != 0)
  {
    /* Not expected result, wrong on Encryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }
  
  /* Generate the authentication TAG */
  HAL_CRYPEx_AESGCM_GenerateAuthTAG(&hcryp,TAG, TIMEOUT_VALUE);
  
  /*Compare results with expected TAG buffer*/ 
  if(memcmp(TAG, ExpectedTAG, 16) != 0)
  {
    /* Not expected result, wrong on TAG buffer: Turn LED3 on */
    Error_Handler();
  }
  
  /*## AES GCM Decryption and TAG generation   ##############################*/   
  
  /* Decryption, result in  Decryptedtext buffer */ 
  HAL_CRYP_Decrypt(&hcryp,Ciphertext , 15, Decryptedtext, TIMEOUT_VALUE);
  /*Compare results with expected buffer*/
  if(memcmp(Decryptedtext, Plaintext, 60) != 0)
  {
    /* Not expected result, wrong on DecryptedText buffer: Turn LED3 on */
    Error_Handler();
  }
  
  /* Generate the authentication TAG */
  HAL_CRYPEx_AESGCM_GenerateAuthTAG(&hcryp,TAG, TIMEOUT_VALUE);
  
  /*Compare results with expected buffer*/ 
  if(memcmp(TAG, ExpectedTAG, 16) != 0)
  {
    /* Not expected result, wrong on TAG buffer: Turn LED3 on */
    Error_Handler();
  }
  
  else
  {
    /* Right Encryptedtext & Decryptedtext buffer : Turn LED1 on */
    BSP_LED_On(LED1);
  }
  
  /* Infinite loop */
  while (1)
  { 
  }  
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Activate the Over-Drive mode */
  HAL_PWREx_EnableOverDrive();
    
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
  {
  }  

}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */
