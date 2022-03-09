/**
  ******************************************************************************
  * @file    FatFs/FatFs_MultiDrives/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
  *          This sample code shows how to use FatFs with multi drives.
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS RAMFatFs, SDFatFs;    /* File system objects logical drives */
FIL RAMFile, SDFile;        /* File objects */
char RAMpath[4], SDpath[4]; /* RAM disk and SD card logical drives paths */
static uint8_t buffer[_MAX_SS]; /* a work buffer for the f_mkfs() */

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
  FRESULT res1, res2;                                   /* FatFs function common result codes */
  uint32_t byteswritten1, byteswritten2;                /* File write counts */
  uint32_t bytesread1, bytesread2;                      /* File read counts */
  uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
  uint8_t rtext1[100], rtext2[100];                     /* File read buffers */
  
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 175 MHz */
  SystemClock_Config();
  
  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  
  /*##-1- Link the disk I/O drivers ##########################################*/
  if((FATFS_LinkDriver(&SDRAMDISK_Driver, RAMpath) == 0) && (FATFS_LinkDriver(&SD_Driver, SDpath) == 0))
  {  
    /*##-2- Register the file system object to the FatFs module ##############*/
    res1 = f_mount(&RAMFatFs, (TCHAR const*)RAMpath, 0);
    res2 = f_mount(&SDFatFs, (TCHAR const*)SDpath, 0);
    
    if((res1 != FR_OK) || (res2 != FR_OK))
    {
      /* FatFs Initialization Error */
      Error_Handler();
    }
    else
    {
      /*##-3- Create a FAT file system (format) on the logical drives ########*/
      /* WARNING: Formatting the uSD card will delete all content on the device */ 
      res1 = f_mkfs((TCHAR const*)RAMpath, FM_ANY, 0, buffer, sizeof(buffer));
      res2 = f_mkfs((TCHAR const*)SDpath, FM_ANY, 0, buffer, sizeof(buffer));
      
      if((res1 != FR_OK) || (res2 != FR_OK))
      {
        /* FatFs Format Error */
        Error_Handler();
      }
      else
      {
        /*##-4- Create and Open new text file objects with write access ######*/
        res1 = f_open(&RAMFile, "0:STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE);
        res2 = f_open(&SDFile, "1:STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE);
        
        if((res1 != FR_OK) || (res2 != FR_OK))
        {
          /* 'STM32.TXT' file Open for write Error */
          Error_Handler();
        }
        else
        {
          /*##-5- Write data to the text files ###############################*/
          res1 = f_write(&RAMFile, wtext, sizeof(wtext), (void *)&byteswritten1);
          res2 = f_write(&SDFile, wtext, sizeof(wtext), (void *)&byteswritten2);
          
          if((byteswritten1 == 0) || (byteswritten2 == 0) || (res1 != FR_OK) || (res2 != FR_OK))
          {
            /* 'STM32.TXT' file write Error */
            Error_Handler();
          }
          else
          {
            /*##-6- Close the open text files ################################*/
            f_close(&RAMFile);
            f_close(&SDFile);
            
            /*##-7- Open the text files object with read access ##############*/
            res1 = f_open(&RAMFile, "0:STM32.TXT", FA_READ);
            res2 = f_open(&SDFile, "1:STM32.TXT", FA_READ);
            
            if((res1 != FR_OK) || (res2 != FR_OK))
            {
              /* 'STM32.TXT' file Open for read Error */
              Error_Handler();
            }
            else
            {
              /*##-8- Read data from the text files ##########################*/
              res1 = f_read(&RAMFile, rtext1, sizeof(rtext1), (UINT*)&bytesread1);
              res2 = f_read(&SDFile, rtext2, sizeof(rtext2), (UINT*)&bytesread2);
              
              if((res1 != FR_OK) || (res2 != FR_OK))
              {
                /* 'STM32.TXT' file Read or EOF Error */
                Error_Handler();
              }
              else
              {
                /*##-9- Close the open text files ############################*/
                f_close(&RAMFile);
                f_close(&SDFile);
                
                /*##-10- Compare read data with the expected data ############*/
                if((bytesread1 != byteswritten1) || (bytesread2 != byteswritten2))
                {                
                  /* Read data is different from the expected data */
                  Error_Handler();
                }
                else
                {
                  /* Success of the demo: no error occurrence */
                  BSP_LED_On(LED1);
                }
              }
            }
          }
        }
      }
    }
  }
  
  /*##-11- Unlink the disk I/O drivers #######################################*/
  FATFS_UnLinkDriver(RAMpath);
  FATFS_UnLinkDriver(SDpath);
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 175000000
  *            HCLK(Hz)                       = 175000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 350
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
  RCC_OscInitStruct.PLL.PLLN = 350;
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
