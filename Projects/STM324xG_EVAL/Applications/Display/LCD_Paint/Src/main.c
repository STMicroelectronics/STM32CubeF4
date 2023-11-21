/**
  ******************************************************************************
  * @file    LCD_Paint/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
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
#include "color.h"
#include "save.h"

/** @addtogroup STM32F4xx_HAL_Applications
  * @{
  */

/** @addtogroup LCD_Paint
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;   /* File system object for SD card logical drive */
FIL MyFile;      /* File object */
char SDPath[4];  /* SD card logical drive path */
static uint8_t buffer[_MAX_SS]; /* a work buffer for the f_mkfs() */

static uint32_t Radius = 2;
static uint32_t x = 0, y = 0;
static TS_StateTypeDef  TS_State;

const uint32_t aBMPHeader[14]=         
{0x2C364D42, 0x00000001, 0x00360000, 0x00280000, 0x00F00000, 0x00A00000, 0x00010000, 
 0x00000010, 0xF5400000, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0000};
  
/* Private function prototypes -----------------------------------------------*/
static void Draw_Menu(void);
static void GetPosition(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void Save_Picture(void);
static void Prepare_Picture(void);
static void Update_Color(void);
static void Update_Size(uint8_t size);

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
    
  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  
  /*##-1- LCD Initialization #################################################*/ 
  /* Initialize the LCD */
  BSP_LCD_Init();
 
  /* Enable the LCD */
  BSP_LCD_DisplayOn();

  /* Clear the LCD Background layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /*##-2- Touch screen initialization ########################################*/
  Touchscreen_Calibration();
  BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
 
  /*##-3- Link the SD Card disk I/O driver ###################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) != 0) 
  {
    /* FatFs Initialization Error */
    Error_Handler();
  }
  
  /* Create a FAT file system (format) on the logical drive */
  f_mkfs((TCHAR const*)SDPath, FM_ANY, 0, buffer, sizeof(buffer));
  
  /*##-4- Register the file system object to the FatFs module ################*/
  if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
  {
    /* FatFs Initialization Error */
    Error_Handler();
  }  
  
  /*##-5- Draw the menu ######################################################*/
  Draw_Menu();  

  /* Infinite loop */  
  while (1)
  { 
  /*##-6- Configure the touch screen and Get the position ####################*/    
    GetPosition();
  }
}

/**
  * @brief  Configures and gets Touch screen position.
  * @param  None
  * @retval None
  */
static void GetPosition(void)
{
  static uint32_t color_width;  
  static uint32_t color;
  
  color_width = 18;
  
  /* Get Touch screen position */
  BSP_TS_GetState(&TS_State); 
  
  if(TS_State.TouchDetected)
  {
    /* Read the coordinate */
    x = Calibration_GetX(TS_State.x);
    y = Calibration_GetX(TS_State.y);
    
    if((x > (67 + Radius)) & (y > (7 + Radius) ) & ( x < (BSP_LCD_GetXSize() - (7 + Radius ))) & (y < (BSP_LCD_GetYSize() - (67 + Radius ))))
    {
      BSP_LCD_FillCircle((x), (y), Radius);
    }
    else if((x > 0 ) & ( x < 50 ))
    { 
      if((y > 0 ) & (y < color_width ))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        Update_Size(Radius);
      }
      else if((y > color_width ) & (y < (2 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
        Update_Size(Radius);
      }
      else if((y > (2 * color_width)) & (y < (3 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
        Update_Size(Radius);
      }
      else if((y > (3 * color_width)) & (y < (4 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_LIGHTMAGENTA);
        Update_Size(Radius);
      }
      else if((y > (4 * color_width)) & (y < (5 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
        Update_Size(Radius);
      }
      else if((y > (5 * color_width)) &(y < (6 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
        Update_Size(Radius);
      }
      else if((y > (6 * color_width)) &(y < (7 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_BROWN);
        Update_Size(Radius);
      }
      else if((y > (7 * color_width)) & (y < (8 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        Update_Size(Radius);
      }
      else if((y > (8 * color_width)) & (y < (9 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
        Update_Size(Radius);
      }
      else if((y > (9 * color_width)) & (y < (10 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
        Update_Size(Radius);
      }
      else if((y > (10 * color_width)) & (y < (11 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
        Update_Size(Radius);
      }
      else if((y > (11 * color_width)) & (y < (12 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        Update_Size(Radius);
      }    
      else if((y > (12 * color_width)) & (y < (13 * color_width)))
      {     
        /* Get the current text color */
        color = BSP_LCD_GetTextColor();
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        /* Clear the working window */
        BSP_LCD_FillRect(68, 8, 245, 164);
        BSP_LCD_SetTextColor(color);
      }
      else
      {
        x = 0;
        y = 0;
      }
      Update_Color();    
    }
    else if((x > 70) & (y > (BSP_LCD_GetYSize() - 50)) & (y < (BSP_LCD_GetYSize()) ) & ( x < 100))
    {    
      Radius = 10;
      Update_Size(Radius);
    }
    else if((x > 100) & (y > (BSP_LCD_GetYSize() - 50)) & (y < (BSP_LCD_GetYSize()) ) & ( x < 130))
    {    
      Radius = 5;
      Update_Size(Radius);
    }
    else if((x > 130) & (y > (BSP_LCD_GetYSize() - 50)) & (y < (BSP_LCD_GetYSize()) ) & ( x < 160))
    {    
      Radius = 2;
      Update_Size(Radius);
    }  
    else if(((x > (BSP_LCD_GetXSize()-5) ) & (y > (12 * color_width)) & (y < (13 * color_width))) | ((x < 55) & (y < 5)))
    {    
      TS_State.x = 0;
      TS_State.y = 0;
    }  
    else if((x > 160) & (y > (BSP_LCD_GetYSize() - 50)) & (x < 210) & (y < BSP_LCD_GetYSize()))
    {   
      Save_Picture();
    }
  }
}

/**
  * @brief  Draws the menu.
  * @param  None
  * @retval None
  */
static void Draw_Menu(void)
{   
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);  
  
  /* Draw color image */
  BSP_LCD_DrawBitmap(0, 0, (uint8_t *)color);
  
  /* Draw save image */
  BSP_LCD_DrawBitmap(160, (BSP_LCD_GetYSize() - 50), (uint8_t *)save);
  
  /* Set Black as text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  
  /* Draw working window */
  BSP_LCD_DrawRect(61, 0, (BSP_LCD_GetXSize()-61), (BSP_LCD_GetYSize()-60));
  BSP_LCD_DrawRect(63, 3, (BSP_LCD_GetXSize()-66), (BSP_LCD_GetYSize()-66));
  BSP_LCD_DrawRect(65, 5, (BSP_LCD_GetXSize()-70), (BSP_LCD_GetYSize()-70));
  BSP_LCD_DrawRect(67, 7, (BSP_LCD_GetXSize()-74), (BSP_LCD_GetYSize()-74));
  
  /* Draw size icons */
  BSP_LCD_FillRect(60, (BSP_LCD_GetYSize() - 48), 90, 48);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillCircle(80, (BSP_LCD_GetYSize() - 24), 10);
  BSP_LCD_FillCircle(110, (BSP_LCD_GetYSize() - 24), 5);
  BSP_LCD_FillCircle(135, (BSP_LCD_GetYSize() - 24), 2);  
  
  BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
  BSP_LCD_SetFont(&Font8);
  BSP_LCD_DisplayStringAt(210, (BSP_LCD_GetYSize() - 55), (uint8_t *)"Selected Color  Size", LEFT_MODE);  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
  BSP_LCD_FillRect(220, (BSP_LCD_GetYSize() - 40), 30, 30);  
  BSP_LCD_FillCircle(290, (BSP_LCD_GetYSize() - 24), Radius); 
}

/**
  * @brief  Saves the picture in microSD.
  * @param  None
  * @retval None
  */
void Save_Picture(void)
{ 
  FRESULT res1, res2;             /* FatFs function common result code */
  uint32_t byteswritten = 0;     /* File write count */
  static uint32_t counter = 0;
  uint8_t str[30];  
  
  /* Check if the SD card is plugged in the slot */
  if(BSP_SD_IsDetected() != SD_PRESENT)
  {
    Error_Handler();
  }
  else 
  {    
    /* Format the string */
    sprintf((char *)str, "image_%lu.bmp", counter);
    
    /*##-1- Prepare the image to be saved ####################################*/
    Prepare_Picture();
    
    /*##-2- Create and Open a new bmp file object with write access ##########*/
    if(f_open(&MyFile, (const char*)str, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
      /* 'image.bmp' file Open for write Error */
      Error_Handler();
    }
    else
    {
      /*##-3- Write data to the BMP file #####################################*/
      /* Write the BMP header */ 
      res1 = f_write(&MyFile, (uint32_t *)aBMPHeader, 54, (void *)&byteswritten);
      
      /* Write the BMP file */
      res2 = f_write(&MyFile, (uint32_t *)CONVERTED_FRAME_BUFFER, ((BSP_LCD_GetYSize() - 80)*(BSP_LCD_GetXSize() - 80)*2), (void *)&byteswritten);
      
      if((res1 != FR_OK) || (res2 != FR_OK) || (byteswritten == 0))
      {
        /* 'image.bmp' file Write or EOF Error */
        Error_Handler();
      }
      else
      {
        /*##-4- Close the open BMP file ######################################*/
        f_close(&MyFile);
        
        /* Success of the demo: no error occurrence */
        BSP_LED_On(LED1);
        
        /* Wait for 2s */
        HAL_Delay(2000);
        
        /* Select Layer 1 */
        BSP_LED_Off(LED1);
        counter++;      
      }
    }
  }
}

/**
  * @brief  Prepares the picture to be saved in microSD.
  * @param  None
  * @retval None
  */
static void Prepare_Picture(void) 
{ 
  uint32_t x_counter = 0, y_counter = 0;
  uint32_t address = SRAM_DEVICE_ADDR;
  uint16_t tmp = 0;
  
  /* Bypass the bitmap header */
  address += (((BSP_LCD_GetXSize() - 80) * (BSP_LCD_GetYSize() - 81)) * 2);  
  
  BSP_SRAM_Init();
  
  /* Prepare picture */
  for(y_counter = 10; y_counter < (BSP_LCD_GetYSize() - 70); y_counter++)
  { 
    for(x_counter = 70; x_counter < (BSP_LCD_GetXSize() - 10); x_counter++)
    {      
      /* Write data to the SRAM memory */
      tmp = BSP_LCD_ReadPixel(x_counter, y_counter);
      
      BSP_SRAM_WriteData(address, &tmp, 1);
      
      /* Increment the source and destination buffers */
      address += 2;      
    }
    address -= 4*(BSP_LCD_GetXSize() - 80);
  }
}

/**
  * @brief  Updates the selected color
  * @param  None
  * @retval None
  */
static void Update_Color(void)
{
  static uint32_t color;
  
  /* Get the current text color */
  color = BSP_LCD_GetTextColor();
  
  /* Update the selected color icon */
  BSP_LCD_FillRect(220, (BSP_LCD_GetYSize() - 40), 30, 30);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);    
  BSP_LCD_DrawRect(220, (BSP_LCD_GetYSize() - 40), 30, 30);
  BSP_LCD_SetTextColor(color);  
}

/**
  * @brief  Updates the selected size
  * @param  size: Size to be updated
  * @retval None
  */
static void Update_Size(uint8_t size)
{
  static uint32_t color;
  
  /* Get the current text color */ 
  color = BSP_LCD_GetTextColor();
  
  /* Update the selected size icon */
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillCircle(290, (BSP_LCD_GetYSize() - 24), 20);
  BSP_LCD_SetTextColor(color);  
  BSP_LCD_FillCircle(290, (BSP_LCD_GetYSize() - 24), size);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);    
  BSP_LCD_DrawCircle(290, (BSP_LCD_GetYSize() - 24), size);
  BSP_LCD_SetTextColor(color);  
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
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
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

/**
  * @}
  */
