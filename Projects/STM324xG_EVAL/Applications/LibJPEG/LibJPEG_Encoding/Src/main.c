/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Encoding/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  *          This sample code shows how to compress BMP file to JPEG file.
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
FATFS SDFatFs;       /* File system object for SD card logical drive */
FIL MyFile, MyFile1; /* File objects */
char SDPath[4];      /* SD card logical drive path */

uint32_t offset = 0xC0000000;
RGB_typedef *RGB_matrix;

uint8_t Image_BMP[1024];
uint32_t counter, bytesread;
  
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength);
static void LCD_Config(void);
  
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
  
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();
  
  /*##-1- LCD Configuration ##################################################*/ 
  LCD_Config();
  
  /*##-2- Link the micro SD disk I/O driver ##################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
    /*##-3- Register the file system object to the FatFs module ##############*/
    if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) == FR_OK)
    {
      /*##-4- Create and Open a new jpg image file with write access #########*/
      if(f_open(&MyFile1, "image.jpg", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
      {
        /*##-5- Open the BMP image with read access ##########################*/
        if(f_open(&MyFile, "image.bmp", FA_READ) == FR_OK)
        {
          /*##-6- Jpeg encoding ##############################################*/
          jpeg_encode(&MyFile, &MyFile1, IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_QUALITY, Image_BMP);
          
          /* Close the BMP and JPEG files */
          f_close(&MyFile1);
          f_close(&MyFile);
          
          /*##-7- Jpeg decoding ##############################################*/
          /* Open the BMP file for read */
          if(f_open(&MyFile1, "image.jpg", FA_READ) == FR_OK)
          { 
            /* Jpeg Decoding for display to LCD */
            jpeg_decode(&MyFile1, IMAGE_WIDTH, Image_BMP, Jpeg_CallbackFunction);
            
            /* Close the BMP file */
            f_close(&MyFile1);    
          } 
        }
      }      
    }
  }
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  LCD Configuration.
  * @Param  None
  * @retval None
  */
static void LCD_Config(void)
{
  /* Initialize the SRAM */  
  BSP_SRAM_Init(); 
  BSP_LCD_Init();
  
  /* Enable the LCD */
  BSP_LCD_DisplayOn();
  
  /* Clear the LCD Background layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE); 
}

/**
  * @brief  Copy decompressed data to display buffer.
  * @param  Row: Output row buffer
  * @param  DataLength: Row width in output buffer
  * @retval None
  */
static uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength)
{
  uint32_t i = 0;
  RGB_matrix =  (RGB_typedef*)Row;
  uint16_t  RGB16Buffer[IMAGE_WIDTH];
  
  for(i = 0; i < IMAGE_WIDTH; i++)
  {
    RGB16Buffer[i]  = (uint16_t)
                      (((RGB_matrix[i].B & 0x00F8) >> 3)|
                       ((RGB_matrix[i].G & 0x00FC) << 3)|
                       ((RGB_matrix[i].R & 0x00F8) << 8));
    BSP_LCD_DrawPixel(i, counter, RGB16Buffer[i]);
  }
  counter++;
  return 0;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
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
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  /* STM32F405x/407x/415x/417x Revision Z and upper devices: prefetch is supported  */
  if (HAL_GetREVID() >= 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
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
