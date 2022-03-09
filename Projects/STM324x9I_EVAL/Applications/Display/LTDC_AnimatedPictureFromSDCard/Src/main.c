/**
  ******************************************************************************
  * @file    Display/LTDC_AnimatedPictureFromSDCard/Src/main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
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

/** @addtogroup STM32F4xx_HAL_Applications
  * @{
  */

/** @addtogroup LTDC_AnimatedPictureFromSDCard
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SD_FatFs;  /* File system object for SD card logical drive */
char SD_Path[4]; /* SD card logical drive path */
DIR directory;
char* pDirectoryFiles[MAX_BMP_FILES];
uint8_t ubNumberOfFiles = 0;
uint32_t uwBmplen = 0;

/* Internal Buffer defined in SDRAM memory */
uint8_t *uwInternelBuffer;

/* Private function prototypes -----------------------------------------------*/
static void LCD_Config(void);
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
  uwInternelBuffer = (uint8_t *)0xC0260000;
  uint32_t counter = 0;
  uint8_t str[30];
  
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 175 MHz */
  SystemClock_Config();
  
  /* Configure LED3 */
  BSP_LED_Init(LED3);
  
  /*##-1- Configure LCD ######################################################*/
  LCD_Config();

  /*##-2- Initialization SD card ######################################################*/
  if(BSP_SD_Init() != MSD_OK)
  {
    /* SD Initialization Error */
    /* Set the Text Color */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);

    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"  Please insert SD Card ");
  }
  else
  {
    /*##-3- Link the SD Card disk I/O driver ###################################*/
    if(FATFS_LinkDriver(&SD_Driver, SD_Path) != 0)
    {
      Error_Handler();
    }
    else
    {
      /*##-4- Initialize the Directory Files pointers (heap) ###################*/
      for (counter = 0; counter < MAX_BMP_FILES; counter++)
      {
        pDirectoryFiles[counter] = malloc(MAX_BMP_FILE_NAME);
        if(pDirectoryFiles[counter] == NULL)
        {
          /* Set the Text Color */
          BSP_LCD_SetTextColor(LCD_COLOR_RED);

          BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"  Cannot allocate memory ");
          while(1)
          {
          }
        }
      }

      /*##-5- Display Background picture #######################################*/
      /* Select Background Layer  */
      BSP_LCD_SelectLayer(0);

      /* Register the file system object to the FatFs module */
      if(f_mount(&SD_FatFs, (TCHAR const*)SD_Path, 0) != FR_OK)
      {
        /* FatFs Initialization Error */
        /* Set the Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_RED);

        BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"  FatFs Initialization Error ");
      }
      else
      {    
        /* Open directory */
        if (f_opendir(&directory, (TCHAR const*)"/BACK") != FR_OK)
        {
          /* Set the Text Color */
          BSP_LCD_SetTextColor(LCD_COLOR_RED);

          BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    Open directory.. fails");
          while(1)
          {
          }
        }
      }

      if (Storage_CheckBitmapFile("BACK/image.bmp", &uwBmplen) == 0)
      {
        /* Format the string */
        Storage_OpenReadFile(uwInternelBuffer, "BACK/image.bmp");
        /* Write bmp file on LCD frame buffer */
        BSP_LCD_DrawBitmap(0, 0, uwInternelBuffer);
      }
      else
      {
        /* Set the Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_RED);

        BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    File type not supported. "); 
        while(1)
        {
        }  
      }

      /*##-6- Display Foreground picture #######################################*/
      /* Select Foreground Layer  */
      BSP_LCD_SelectLayer(1);

      /* Decrease the foreground transparency */
      BSP_LCD_SetTransparency(1, 200); 

      /* Get the BMP file names on root directory */
      ubNumberOfFiles = Storage_GetDirectoryBitmapFiles("/TOP", pDirectoryFiles);

      if (ubNumberOfFiles == 0)
      {
        for (counter = 0; counter < MAX_BMP_FILES; counter++)
        {
          free(pDirectoryFiles[counter]);
        }
        /* Set the Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_RED);

        BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    No Bitmap files...      ");
        while(1)
        {
        }
      } 
    }
  }

  /* Infinite loop */
  while(1)
  { 
    counter = 0;
    
    while (counter < ubNumberOfFiles)
    {
      /* Format the string */
      sprintf ((char*)str, "TOP/%-11.11s", pDirectoryFiles[counter]); 
      
      if (Storage_CheckBitmapFile((const char*)str, &uwBmplen) == 0)
      {
        /* Format the string */
        sprintf ((char*)str, "TOP/%-11.11s", pDirectoryFiles[counter]);
        
        /* Open a file and copy its content to a buffer */
        Storage_OpenReadFile(uwInternelBuffer, (const char*)str);
        
        HAL_Delay(100);
        
        /* Write bmp file on LCD frame buffer */
        BSP_LCD_DrawBitmap(0, 0, uwInternelBuffer);
        
        /* Jump to next image */
        counter++;   
      }
      else
      {
        /* Set the Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_RED); 
        
        BSP_LCD_DisplayStringAtLine(7, (uint8_t *) str);        
        BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    File type not supported. "); 
        while(1)
        {
        }    
      }
    }
  }
}

/**
  * @brief  LCD configuration.
  * @param  None
  * @retval None
  */
static void LCD_Config(void)
{
  /* LCD Initialization */
  /* Two layers are used in this application simultaneously 
     so "LCD_MIN_PCLK" is recommended to programme the PCLK at 18 MHz */    
  BSP_LCD_InitEx(LCD_MIN_PCLK);

  /* LCD Layers Initialization */ 
  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
  BSP_LCD_LayerDefaultInit(1, (LCD_FB_START_ADDRESS+(BSP_LCD_GetXSize()*BSP_LCD_GetYSize()*4)));
  
  /* Enable the LCD */ 
  BSP_LCD_DisplayOn();

  /* Set LCD Background Layer  */
  BSP_LCD_SelectLayer(0);
  /* Clear the Background Layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Foreground Layer  */
  BSP_LCD_SelectLayer(1);
  /* Clear the Foreground Layer */ 
  BSP_LCD_Clear(LCD_COLOR_BLACK); 

  /* Configure and enable the Color Keying feature */
  BSP_LCD_SetColorKeying(1, 0); 

  /* Configure the transparency for foreground: Increase the transparency */
  BSP_LCD_SetTransparency(1, 100);
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

/**
  * @}
  */

/**
  * @}
  */ 
