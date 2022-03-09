/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESmodes/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the STM32F4xx CRYP HAL API
  *          to encrypt and decrypt data using TDES in ECB and CBC Algorithm.
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
CRYP_ConfigTypeDef Conf;
DMA_HandleTypeDef hdmaIn;
DMA_HandleTypeDef hdmaOut;
__IO uint32_t CrypCompleteDetected = 0;

/* AES Keys  */
uint32_t AESKey128[4] = {0x2B7E1516 ,0x28AED2A6 ,0xABF71588 ,0x09CF4F3C};
uint32_t AESKey192[6] = {0x8E73B0F7 ,0xDA0E6452 ,0xC810F32B ,0x809079E5 ,0x62F8EAD2 ,0x522C6B7B};
uint32_t AESKey256[8] = {0x603DEB10 ,0x15CA71BE ,0x2B73AEF0 ,0x857D7781 ,0x1F352C07 ,0x3B6108D7 ,0x2D9810A3 ,0x0914DFF4};

/*Initialization Vector*/
uint32_t AESIV_CBC[4]  = {0x00010203 , 0x04050607 , 0x08090A0B , 0x0C0D0E0F};
uint32_t AESIV_CTR[4]  = {0xF0F1F2F3 , 0xF4F5F6F7 , 0xF8F9FAFB , 0xFCFDFEFF};

/* Plaintext */
 uint32_t Plaintext[16]          =  {0x6BC1BEE2 ,0x2E409F96 ,0xE93D7E11 ,0x7393172A ,
                                     0xAE2D8A57 ,0x1E03AC9C ,0x9EB76FAC ,0x45AF8E51 ,
                                     0x30C81C46 ,0xA35CE411 ,0xE5FBC119 ,0x1A0A52EF ,
                                     0xF69F2445 ,0xDF4F9B17 ,0xAD2B417B ,0xE66C3710};

/* Expected ECB Ciphertext with AESKey128 */
uint32_t CiphertextAESECB128[16] = {0x3AD77BB4 ,0x0D7A3660 ,0xA89ECAF3 ,0x2466EF97 ,
                                    0xF5D3D585 ,0x03B9699D ,0xE785895A ,0x96FDBAAF ,
                                    0x43B1CD7F ,0x598ECE23 ,0x881B00E3 ,0xED030688 ,
                                    0x7B0C785E ,0x27E8AD3F ,0x82232071 ,0x04725DD4};

/* Expected CBC Ciphertext with AESKey192*/
uint32_t CiphertextAESCBC192[16] = {0x4F021DB2 ,0x43BC633D ,0x7178183A ,0x9FA071E8 ,
                                    0xB4D9ADA9 ,0xAD7DEDF4 ,0xE5E73876 ,0x3F69145A ,
                                    0x571B2420 ,0x12FB7AE0 ,0x7FA9BAAC ,0x3DF102E0 ,
                                    0x08B0E279 ,0x88598881 ,0xD920A9E6 ,0x4F5615CD};

/* Expected CTR Ciphertext with AESKey256 */
uint32_t CiphertextAESCTR256[16] = {0x601EC313 ,0x775789A5 ,0xB7A7F504 ,0xBBF3D228 ,
                                    0xF443E3CA ,0x4D62B59A ,0xCA84E990 ,0xCACAF5C5 ,
                                    0x2B0930DA ,0xA23DE94C ,0xE87017BA ,0x2D84988D ,
                                    0xDFC9C58D ,0xB67AADA6 ,0x13C2DD08 ,0x457941A6};

/* Used for storing Encrypted text */
static uint32_t Encryptedtext[16]={0};

/* Used for storing Decrypted text */
static uint32_t Decryptedtext[16]={0};

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

  /*## Initialize the CRYP IP  ###############################################*/

  /* Set the CRYP parameters */
  hcryp.Instance             = CRYP;
  hcryp.Init.DataType        = CRYP_DATATYPE_32B;
  hcryp.Init.KeySize         = CRYP_KEYSIZE_128B;
  hcryp.Init.pKey            = AESKey128;
  hcryp.Init.Algorithm       = CRYP_AES_ECB;
  hcryp.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ALWAYS;

  /* Initialize the CRYP  */
  HAL_CRYP_Init(&hcryp);

  /*##-1- AES ECB Encryption/Decryption in polling mode ######################*/

  /* Encryption, result in  Encryptedtext buffer */
  HAL_CRYP_Encrypt(&hcryp, Plaintext, 16, Encryptedtext, TIMEOUT_VALUE);

  /*compare with expected result */
  if(memcmp(Encryptedtext, CiphertextAESECB128, 64) != 0)
  {
    /* not expected result, wrong on Encryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }

  /* Decryption, result in  Decryptedtext buffer */
  HAL_CRYP_Decrypt(&hcryp, CiphertextAESECB128 , 16, Decryptedtext, TIMEOUT_VALUE);

  /*compare with expected result */
  if(memcmp(Decryptedtext, Plaintext, 64) != 0)
  {
    /* not expected result, wrong on Decryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }

  /*##-2- AES CBC Encryption/Decryption in DMA mode  #########################*/

  /* Get the CRYP parameters */
  HAL_CRYP_GetConfig(&hcryp, &Conf);

  /* Set the CRYP parameters */
  Conf.DataType  = CRYP_DATATYPE_32B;
  Conf.KeySize   = CRYP_KEYSIZE_192B;
  Conf.pKey      = AESKey192;
  Conf.Algorithm = CRYP_AES_CBC;
  Conf.pInitVect = AESIV_CBC;

  /* Configure the CRYP  */
  HAL_CRYP_SetConfig(&hcryp, &Conf);

  /* Encryption, result in  Encryptedtext buffer */
  HAL_CRYP_Encrypt_DMA(&hcryp, Plaintext, 16, Encryptedtext);

  /*wait until output transfer complete*/
  while(CrypCompleteDetected == 0)
  {
  }

  /*compare with expected result */
  if(memcmp(Encryptedtext, CiphertextAESCBC192, 64) != 0)
  {
    /* not expected result, wrong on Encryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }
  /* Reset Output Transfer Complete Detect */
  CrypCompleteDetected = 0;

  /* Decryption, result in  Decryptedtext buffer */
  HAL_CRYP_Decrypt_DMA(&hcryp, CiphertextAESCBC192, 16, Decryptedtext);

  /*wait until output transfer complete*/
  while(CrypCompleteDetected == 0)
  {
  }
  /* Reset Output Transfer Complete Detect */
  CrypCompleteDetected = 0;

  /*compare with expected result */
  if(memcmp(Decryptedtext, Plaintext, 64) != 0)
  {
    /* not expected result, wrong on Decryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }

  /*##-3- AES CTR Encryption/Decryption in Interrupt mode  ###################*/

  /* Set the CRYP parameters */
  Conf.pInitVect = AESIV_CTR;
  Conf.KeySize   = CRYP_KEYSIZE_256B;
  Conf.pKey      = AESKey256;
  Conf.Algorithm = CRYP_AES_CTR;

  /* Configure the CRYP  */
  HAL_CRYP_SetConfig(&hcryp, &Conf);

  /* Encryption, result in  Encryptedtext buffer */
  HAL_CRYP_Encrypt_IT(&hcryp, Plaintext, 16, Encryptedtext);

  /*wait until output transfer complete*/
  while(CrypCompleteDetected == 0)
  {
  }

  /* Reset Output Transfer Complete Detect */
  CrypCompleteDetected = 0;

  /*compare with expected result */
  if(memcmp(Encryptedtext, CiphertextAESCTR256, 64) != 0)
  {
    /* not expected result, wrong on Encryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }

  /* Decryption, result in  Decryptedtext buffer */
  HAL_CRYP_Decrypt_IT(&hcryp, CiphertextAESCTR256, 16, Decryptedtext);

  /*wait until output transfer complete*/
  while(CrypCompleteDetected == 0)
  { }

  /* Reset Output Transfer Complete Detect */
  CrypCompleteDetected = 0;

  /*compare with expected result */
  if(memcmp(Decryptedtext, Plaintext, 64) != 0)
  {
    /* not expected result, wrong on Decryptedtext buffer: Turn LED3 on */
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
  * @brief  Output FIFO transfer completed callbacks.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval 1 if output FIFO transfer completed
  */
 void HAL_CRYP_OutCpltCallback(CRYP_HandleTypeDef *hcryp)
 {
     CrypCompleteDetected = 1;
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
