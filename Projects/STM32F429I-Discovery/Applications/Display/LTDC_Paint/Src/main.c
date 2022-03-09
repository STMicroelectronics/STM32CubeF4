/**
  ******************************************************************************
  * @file    Display/LTDC_Paint/Src/main.c 
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
#include "color2.h"
#include "save.h"

/** @addtogroup STM32F4xx_HAL_Applications
  * @{
  */

/** @addtogroup LTDC_Paint
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum {
  APPLICATION_IDLE = 0,  
  APPLICATION_START    
}MSC_ApplicationTypeDef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBH_HandleTypeDef  hUSBHost;
FATFS USBDISK_FatFs;  /* File system object for USB Disk logical drive */
FIL MyFile;           /* File object */
char USB_Path[4];     /* USB Disk logical drive path */

const uint32_t aBMPHeader[14]=         
{0x24A64D42, 
 0x00000002, /*0x224A6: (240-60)*(320-60)*3 + 54*/
 0x00360000, 0x00280000, 
 0x00B40000, /* X=(240-60)=0xB4*/
 0x01040000, /* Y=(320-60)=0x104*/
 0x00010000, 0x00000018, 0xF5400000, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0000};

/* Variable to save the current size of drawing */
static uint32_t radius = 2;  

/* Variable to save the state of USB */
MSC_ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/* Private function prototypes -----------------------------------------------*/
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
static void Draw_Menu(void);
static void GetPosition(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void Save_Picture(void);
static void Prepare_Picture(void);
static void Update_ColorSize(void);

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
    
  /* Configure LED3 and LED4 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  
  /*##-1- LCD Initialization #################################################*/ 
  /* Initialize the LCD */
  BSP_LCD_Init();

  /* Layer2 Init */
  BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER_LAYER1);
  /* Set Foreground Layer */
  BSP_LCD_SelectLayer(1);
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);  
  BSP_LCD_SetColorKeying(1, LCD_COLOR_WHITE);
  BSP_LCD_SetLayerVisible(1, DISABLE);
  
  /* Layer1 Init */
  BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER_LAYER0);
  
  /* Set Foreground Layer */
  BSP_LCD_SelectLayer(0);
 
  /* Enable The LCD */
  BSP_LCD_DisplayOn();

  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /*##-2- Touch screen initialization ########################################*/
  Touchscreen_Calibration();
  BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
 
  /*##-3- USB Initialization #################################################*/ 
  /* Init Host Library */
  if (USBH_Init(&hUSBHost, USBH_UserProcess, 0) != USBH_OK)
  {
    /* USB Initialization Error */
    Error_Handler();
  }
  
  /* Add Supported Class */
  USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);
  
  /* Start Host Process */
  if (USBH_Start(&hUSBHost) != USBH_OK)
  {
    /* USB Initialization Error */
    Error_Handler();
  }
  
  /*##-4- Link the USB Mass Storage disk I/O driver ##########################*/
  if(FATFS_LinkDriver(&USBH_Driver, USB_Path) != 0) 
  {
    /* FatFs Initialization Error */
    Error_Handler();
  }
  
  /*##-5- Register the file system object to the FatFs module ################*/
  if(f_mount(&USBDISK_FatFs, (TCHAR const*)USB_Path, 0) != FR_OK)
  {
    /* FatFs Initialization Error */
    Error_Handler();
  }
  
  /*##-6- Draw the menu ######################################################*/
  Draw_Menu();  

  /* Infinite loop */  
  while (1)
  { 
  /*##-7- Configure the touch screen and Get the position ####################*/    
    GetPosition();
    
    USBH_Process(&hUSBHost);
  }
}

/**
  * @brief  User Process
  * @param  None
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{  
  switch (id)
  { 
  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_IDLE;
    if (f_mount(&USBDISK_FatFs, "", 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      Error_Handler();
    }
    break;
    
  case HOST_USER_CLASS_ACTIVE:
    Appli_state = APPLICATION_START;
    break;
  }
}

/**
  * @brief  Configures and gets Touch screen position.
  * @param  None
  * @retval None
  */
static void GetPosition(void)
{
  static uint32_t x = 0, y = 0;
  static uint32_t color_heigh;
  static uint32_t color;
  static TS_StateTypeDef  TS_State;  
  
  /* Height of color pen */
  color_heigh = 20;
  
  /* Get Touch screen position */
  BSP_TS_GetState(&TS_State); 
  
  /* Read the coordinate */
  x = Calibration_GetX(TS_State.X);
  y = Calibration_GetX(TS_State.Y);
  
  if ((TS_State.TouchDetected) & ( x > 0 ) & ( x < 50 ))
  { 
    /* User selects one of the color pens */
    if ((TS_State.TouchDetected) & ( y > 0 ) & ( y < color_heigh ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    }
    else if ((TS_State.TouchDetected) & ( y > color_heigh ) & ( y < (2*color_heigh) ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    }
    else if ((TS_State.TouchDetected) & ( y > (2*color_heigh) ) & ( y < (3*color_heigh) ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
    }
    else if ((TS_State.TouchDetected) & ( y > (3*color_heigh) ) & ( y < (4*color_heigh) ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_LIGHTMAGENTA);
    }
    else if ((TS_State.TouchDetected) & ( y > (4*color_heigh) ) & ( y < (5*color_heigh) ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
    }
    else if ((TS_State.TouchDetected) & ( y > (5*color_heigh) ) &( y < (6*color_heigh) ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    }
    else if ((TS_State.TouchDetected) & ( y > (6*color_heigh) ) &( y < (7*color_heigh) ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_BROWN);
    }
    else if ((TS_State.TouchDetected) & ( y > (7*color_heigh) ) & ( y < (8*color_heigh) ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
    }
    else if ((TS_State.TouchDetected) & ( y > (8*color_heigh) ) & ( y < (9*color_heigh) ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
    }
    else if ((TS_State.TouchDetected) & ( y > (9*color_heigh) ) & ( y < (10*color_heigh) ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
    }
    else if ((TS_State.TouchDetected) & ( y > (10*color_heigh) ) & ( y < (11*color_heigh) ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
    }
    else if ((TS_State.TouchDetected) & ( y > (11*color_heigh) ) & ( y < (12*color_heigh) ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    }    
    else if ((TS_State.TouchDetected) &  ( y > (12*color_heigh) ) & ( y < (13*color_heigh) ))
    {
      /* Clear screen and reinitialize color and size */
      /* Get the current text color */
      color = BSP_LCD_GetTextColor();
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      /* Clear the working window */
      BSP_LCD_FillRect(68, 8, 164, 245);
      BSP_LCD_SetTextColor(color);
      radius = 2;
    }
    else
    {
      x = 0;
      y = 0;
    }
    Update_ColorSize();    
  }
  else if ((TS_State.TouchDetected) & ( x > 65 ) & ( y > (BSP_LCD_GetYSize()-48)) & ( y < (BSP_LCD_GetYSize()) ) & ( x < 85 ))
  {    
    radius = 10;
    Update_ColorSize();
  }
  else if ((TS_State.TouchDetected) & ( x > 100 ) & ( y > (BSP_LCD_GetYSize()-48) ) & ( y < (BSP_LCD_GetYSize()) ) & ( x < 110 ))
  {    
    radius = 5;
    Update_ColorSize();
  }
  else if ((TS_State.TouchDetected) & ( x > 133 ) & ( y > (BSP_LCD_GetYSize()-48) ) & ( y < (BSP_LCD_GetYSize()) ) & ( x < 137 ))
  {    
    radius = 2;
    Update_ColorSize();
  }  
  else if (((TS_State.TouchDetected) & ( x > (BSP_LCD_GetXSize()-5) ) & ( y > (12*color_heigh) ) & ( y < (13*color_heigh) )) | (( x < 55 ) & ( y < 5 )))
  {    
    x = 0;
    y = 0;
  }  
  else if ((TS_State.TouchDetected) & ( x > (67 + radius)) & ( y > (7 + radius) ) & ( x < (BSP_LCD_GetXSize()-(7  + radius )) ) & ( y < (BSP_LCD_GetYSize()-(67 + radius )) ))
  {
    BSP_LCD_FillCircle((x), (y), radius);
  }
  else if ((TS_State.TouchDetected) & ( (x > 150) & ( y > (BSP_LCD_GetYSize() - 50) )) & ( x < 200 ) & ( y < (BSP_LCD_GetYSize()) ))
  {   
    Save_Picture();
  }    
}

/**
  * @brief  Draw the menu.
  * @param  None
  * @retval None
  */
static void Draw_Menu(void)
{ 
  /* Set background Layer */
  BSP_LCD_SelectLayer(0);
  
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);  
  
  /* Draw color image */
  BSP_LCD_DrawBitmap(0, 0, (uint8_t *)color2);
  
  /* Draw save image */
  BSP_LCD_DrawBitmap(150, (BSP_LCD_GetYSize() - 50), (uint8_t *)save);
  
  /* Set Black as text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  
  /* Draw working window */
  BSP_LCD_DrawRect(61, 0, (BSP_LCD_GetXSize()-61), (BSP_LCD_GetYSize()-60));
  BSP_LCD_DrawRect(63, 3, (BSP_LCD_GetXSize()-66), (BSP_LCD_GetYSize()-66));
  BSP_LCD_DrawRect(65, 5, (BSP_LCD_GetXSize()-70), (BSP_LCD_GetYSize()-70));
  BSP_LCD_DrawRect(67, 7, (BSP_LCD_GetXSize()-74), (BSP_LCD_GetYSize()-74));
  
  /* Draw size icons */
  BSP_LCD_FillRect(60, (BSP_LCD_GetYSize()-48), 90, 48);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillCircle(75, (BSP_LCD_GetYSize()-24), 10);
  BSP_LCD_FillCircle(105, (BSP_LCD_GetYSize()-24), 5);
  BSP_LCD_FillCircle(135, (BSP_LCD_GetYSize()-24), 2);  
  
  BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
  BSP_LCD_SetFont(&Font8);
  BSP_LCD_DisplayStringAt(210, (BSP_LCD_GetYSize()-55), (uint8_t *)"Color", LEFT_MODE);  
  BSP_LCD_DisplayStringAt(210, (BSP_LCD_GetYSize()-45), (uint8_t *)"Size", LEFT_MODE);  
  BSP_LCD_SetTextColor(LCD_COLOR_LIGHTMAGENTA); 
  BSP_LCD_FillCircle(220, (BSP_LCD_GetYSize()-24), 2);  
}

/**
  * @brief  Save the picture in USB Disk.
  * @param  None
  * @retval None
  */
void Save_Picture(void)
{ 
  FRESULT res;           /* FatFs function common result code */
  uint32_t bytesWritten; /* File write count */
  
  BSP_LCD_SetLayerVisible(1, ENABLE);
  BSP_LCD_SetColorKeying(1, LCD_COLOR_WHITE);
  /* Set foreground Layer */
  BSP_LCD_SelectLayer(1);
  BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
  BSP_LCD_SetFont(&Font20);
  
  if (Appli_state == APPLICATION_START)
  {
    BSP_LCD_DisplayStringAt(10, (BSP_LCD_GetYSize()-100), (uint8_t *)"Saving ... ", RIGHT_MODE);
    
    /*##-1- Prepare the image to be saved ####################################*/
    Prepare_Picture();
    
    /*##-2- Create and Open a new bmp file object with write access ##########*/
    if(f_open(&MyFile, "image.bmp", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
      /* 'image.bmp' file Open for write Error */
      Error_Handler();
    }
    else
    {
      /*##-3- Write data to the BMP file #####################################*/
      /* Write the BMP header */
      res = f_write(&MyFile, (uint32_t *)aBMPHeader, 54, (void *)&bytesWritten);
      /* Write the bmp file */
      res = f_write(&MyFile, (uint32_t *)CONVERTED_FRAME_BUFFER, ((BSP_LCD_GetYSize()-60)*(BSP_LCD_GetXSize()-60)*3), (void *)&bytesWritten);
      
      if((bytesWritten == 0) || (res != FR_OK))
      {
        /* 'image' file Write or EOF Error */
        Error_Handler();
      }
      else
      {
        /*##-4- Close the open text file #####################################*/
        f_close(&MyFile);
        
        /* Success of the demo: no error occurrence */
        BSP_LED_On(LED3);
        BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
        BSP_LCD_DisplayStringAt(10, (BSP_LCD_GetYSize()-100), (uint8_t *)"Save       ", RIGHT_MODE);
        /* Wait for 2s */
        HAL_Delay(2000);
        /* Disable the Layer 2 */
        BSP_LCD_SetLayerVisible(1, DISABLE);
        /* Select Layer 1 */
        BSP_LCD_SelectLayer(0);
      }
    }
  }
  else
  {
    /* USB not connected */
    BSP_LCD_DisplayStringAt(10, (BSP_LCD_GetYSize()-100), (uint8_t *)"USB KO... ", RIGHT_MODE);
    /* Wait for 2s */
    HAL_Delay(2000);
    /* Disable the Layer 2 */
    BSP_LCD_SetLayerVisible(1, DISABLE);
    /* Select Layer 1 */
    BSP_LCD_SelectLayer(0);
  }
}

/**
  * @brief  Prepares the picture to be Saved in USB Disk.
  * @param  None
  * @retval None
  */
static void Prepare_Picture(void) 
{ 
  static DMA2D_HandleTypeDef hdma2d_dk;
  uint32_t address1 = CONVERTED_FRAME_BUFFER;
  uint32_t address2 = LCD_FRAME_BUFFER_LAYER0;
  uint32_t index = 0;
  
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_dk.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_dk.Init.ColorMode    = DMA2D_RGB888;
  hdma2d_dk.Init.OutputOffset = 0;     
  
  /* Foreground Configuration */
  hdma2d_dk.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_dk.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_dk.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d_dk.LayerCfg[1].InputOffset = 60;
  
  hdma2d_dk.Instance = DMA2D; 

  /* Bypass the bitmap header */
  address2 += ((BSP_LCD_GetXSize() * (BSP_LCD_GetYSize() - 61) + 60) * 4);  
  
  /* Convert picture to RGB888 pixel format */
  for(index=0; index < (BSP_LCD_GetYSize() - 60); index++)
  { 
    /* DMA2D Initialization */
    if(HAL_DMA2D_Init(&hdma2d_dk) == HAL_OK) 
    {
      if(HAL_DMA2D_ConfigLayer(&hdma2d_dk, 1) == HAL_OK) 
      {
        if (HAL_DMA2D_Start(&hdma2d_dk, address2, address1, (BSP_LCD_GetXSize() - 60), 1) == HAL_OK)
        {
          /* Polling For DMA transfer */  
          HAL_DMA2D_PollForTransfer(&hdma2d_dk, 10);
        }
      }
    }    
    /* Increment the source and destination buffers */
    address1 += ((BSP_LCD_GetXSize() - 60)*3);
    address2 -= BSP_LCD_GetXSize()*4;
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED4 on */
  BSP_LED_On(LED4);
  while(1)
  {
  }
}

/**
  * @brief  Updates the selected Color and Size
  * @param  None
  * @retval None
  */
static void Update_ColorSize(void)
{
  static uint32_t color;
  
  /* Clear the current circle */
  color = BSP_LCD_GetTextColor();
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillCircle(220, (BSP_LCD_GetYSize()-24), 10);  
  BSP_LCD_SetTextColor(color);  
  
  /* Update the selected color icon */
  BSP_LCD_FillCircle(220, (BSP_LCD_GetYSize()-24), radius);  
  
  /* Draw black circle */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);    
  BSP_LCD_DrawCircle(220, (BSP_LCD_GetYSize() - 24), radius);
  BSP_LCD_SetTextColor(color);  
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
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = 2;
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
