/**
  ******************************************************************************
  * @file    FLASH/FLASH_WriteProtection/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a description of how to erase and program the 
  *	     STM32F4xx FLASH.
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

/** @addtogroup FLASH_WriteProtection
  * @{
  */ 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_WRP_SECTORS   (OB_WRP_SECTOR_3 | OB_WRP_SECTOR_4 | OB_WRP_SECTOR_10) /* sectors 3, 4 and 10  */


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FLASH_OBProgramInitTypeDef OBInit; 
__IO uint32_t SectorsWRPStatus = 0xFFF;
   
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

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
  
  /* Configure TAMPER Button */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  BSP_LCD_Init();
  
  /* Set LCD font */
  BSP_LCD_SetFont(&Font20);
   
  /* LCD Layer Initialization */
  BSP_LCD_LayerDefaultInit(1, 0xC0130000); 
  
  BSP_LCD_SelectLayer(1);
  
  BSP_LCD_DisplayOn();
    
  /*##-2- Display messages on LCD ############################################*/  
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  

  /* Display test name on LCD */  
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"Flash Write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 20, (uint8_t*)"protection test", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 40, (uint8_t*)"Press User", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"Tamper/Key-button", CENTER_MODE);
  
  /* Infinite loop */
  while (1)
  {
    /* Wait for TAMPER/KEY button to be pushed */
    while(BSP_PB_GetState(BUTTON_TAMPER) != RESET)
    {
    }
    
    /* Get FLASH_WRP_SECTORS write protection status */
    HAL_FLASHEx_OBGetConfig(&OBInit);
    SectorsWRPStatus = OBInit.WRPSector & FLASH_WRP_SECTORS;
    
    if (SectorsWRPStatus == 0)
    {
      /* If FLASH_WRP_SECTORS are write protected, disable the write protection */
      
      /* Allow Access to option bytes sector */ 
      HAL_FLASH_OB_Unlock();
    
      /* Allow Access to Flash control registers and user Flash */
      HAL_FLASH_Unlock();
      
      /* Disable FLASH_WRP_SECTORS write protection */
      OBInit.OptionType = OPTIONBYTE_WRP;
      OBInit.WRPState   = OB_WRPSTATE_DISABLE;
      OBInit.Banks      = FLASH_BANK_1;
      OBInit.WRPSector  = FLASH_WRP_SECTORS;
      HAL_FLASHEx_OBProgram(&OBInit);
      
      /* Start the Option Bytes programming process */  
      if (HAL_FLASH_OB_Launch() != HAL_OK)
      {
        /* User can add here some code to deal with this error */
        while (1)
        {
        }
      }
      
      /* Prevent Access to option bytes sector */ 
      HAL_FLASH_OB_Lock();
    
      /* Disable the Flash option control register access (recommended to protect 
      the option Bytes against possible unwanted operations) */
      HAL_FLASH_Lock();
      
      /* Get FLASH_WRP_SECTORS write protection status */
      HAL_FLASHEx_OBGetConfig(&OBInit);
      SectorsWRPStatus = OBInit.WRPSector & FLASH_WRP_SECTORS;
      
      /* Check if FLASH_WRP_SECTORS write protection is disabled */
      if (SectorsWRPStatus == FLASH_WRP_SECTORS)
      {
         /* Set the LCD Text Color */
         BSP_LCD_SetTextColor(LCD_COLOR_GREEN);  

         BSP_LCD_DisplayStringAt(0, 100, (uint8_t*)"Write", CENTER_MODE);
         BSP_LCD_DisplayStringAt(0, 120, (uint8_t*)"protection is", CENTER_MODE);
         BSP_LCD_DisplayStringAt(0, 140, (uint8_t*)"disabled", CENTER_MODE);
      }
      else
      {
         /* Set the LCD Text Color */
         BSP_LCD_SetTextColor(LCD_COLOR_RED);  

         BSP_LCD_DisplayStringAt(0, 100, (uint8_t*)"Write", CENTER_MODE);
         BSP_LCD_DisplayStringAt(0, 120, (uint8_t*)"protection is", CENTER_MODE);
         BSP_LCD_DisplayStringAt(0, 140, (uint8_t*)"not disabled", CENTER_MODE);
      }
    }
    else
    { /* If FLASH_WRP_SECTORS are not write protected, enable the write protection */
      
      /* Allow Access to option bytes sector */ 
      HAL_FLASH_OB_Unlock();
    
      /* Allow Access to Flash control registers and user Flash */
      HAL_FLASH_Unlock();
      
      /* Enable FLASH_WRP_SECTORS write protection */
      OBInit.OptionType = OPTIONBYTE_WRP;
      OBInit.WRPState   = OB_WRPSTATE_ENABLE;
      OBInit.Banks      = FLASH_BANK_1;
      OBInit.WRPSector  = FLASH_WRP_SECTORS;
      HAL_FLASHEx_OBProgram(&OBInit);  
      
      /* Start the Option Bytes programming process */  
      if (HAL_FLASH_OB_Launch() != HAL_OK)
      {
        /* User can add here some code to deal with this error */
        while (1)
        {
        }
      }
      
      /* Prevent Access to option bytes sector */ 
      HAL_FLASH_OB_Lock();
    
      /* Disable the Flash option control register access (recommended to protect 
      the option Bytes against possible unwanted operations) */
      HAL_FLASH_Lock();

      /* Get FLASH_WRP_SECTORS write protection status */
      HAL_FLASHEx_OBGetConfig(&OBInit);
      SectorsWRPStatus = OBInit.WRPSector & FLASH_WRP_SECTORS;      
      
      /* Check if FLASH_WRP_SECTORS are write protected */
      if (SectorsWRPStatus == 0)
      {
         /* Set the LCD Text Color */
         BSP_LCD_SetTextColor(LCD_COLOR_GREEN);  

         BSP_LCD_DisplayStringAt(0, 100, (uint8_t*)"Write", CENTER_MODE);
         BSP_LCD_DisplayStringAt(0, 120, (uint8_t*)"protection is", CENTER_MODE);
         BSP_LCD_DisplayStringAt(0, 140, (uint8_t*)"  enabled  ", CENTER_MODE);
      }
      else
      {
         /* Set the LCD Text Color */
         BSP_LCD_SetTextColor(LCD_COLOR_RED);  

         BSP_LCD_DisplayStringAt(0, 100, (uint8_t*)"Write", CENTER_MODE);
         BSP_LCD_DisplayStringAt(0, 120, (uint8_t*)"protection is", CENTER_MODE);
         BSP_LCD_DisplayStringAt(0, 140, (uint8_t*)"not enabled", CENTER_MODE);
      }
    }
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
