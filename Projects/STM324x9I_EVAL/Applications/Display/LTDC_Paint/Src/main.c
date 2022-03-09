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
#include "save.h"
#include "color.h"
#include "color2.h"

/** @addtogroup STM32F4xx_HAL_Applications
  * @{
  */

/** @addtogroup LTDC_Paint
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum {
  APPLICATION_IDLE = 0,  
  APPLICATION_RUNNIG    
}MSC_ApplicationTypeDef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBH_HandleTypeDef  hUSBHost;
FATFS USBDISK_FatFs;  /* File system object for USB Disk logical drive */
FIL MyFile;           /* File object */
char USB_Path[4];     /* USB Disk logical drive path */

static uint32_t Radius = 2;
static uint32_t x = 0, y = 0;
static TS_StateTypeDef  TS_State;

const uint32_t aBMPHeader1[14]=         
{0x26E64D42, 0x0000000B, 0x00360000, 0x00280000, 0x02440000, 0x01A40000, 0x00010000, 
 0x00000018, 0xF5400000, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0000};

const uint32_t aBMPHeader2[14]=         
{0x13A64D42, 0x00000004, 0x00360000, 0x00280000, 0x01A40000, 0x00D40000, 0x00010000, 
 0x00000018, 0xF5400000, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0000};

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
  
  /* Background Layer Initialization */
  BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER_LAYER0);
  
  /* Set Foreground Layer */
  BSP_LCD_SelectLayer(0);
  
  /* Enable the LCD */
  BSP_LCD_DisplayOn();
  
  /* Clear the LCD Background layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /*##-2- Touch screen initialization ########################################*/
  Touchscreen_Calibration();
  
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
    /*##-7- Configure the touch screen and Get the position ##################*/    
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
    Appli_state = APPLICATION_RUNNIG;
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
  static uint32_t color_width;  
  static uint32_t color;
  
  if (BSP_LCD_GetXSize() == 640)
  {
    color_width = 36;
  }
  else
  {
    color_width = 19;
  }
  
 /* Get Touch screen position */
 BSP_TS_GetState(&TS_State); 

  /* Read the coordinate */
  x = Calibration_GetX(TS_State.x);
  y = Calibration_GetX(TS_State.y);

  if ((TS_State.TouchDetected) & (x > (67 + Radius)) & (y > (7 + Radius) ) & ( x < (BSP_LCD_GetXSize()-(7  + Radius )) ) & (y < (BSP_LCD_GetYSize()-(67 + Radius )) ))
  {
    BSP_LCD_FillCircle((x), (y), Radius);
  }
  else if ((TS_State.TouchDetected) & (x > 0 ) & ( x < 50 ))
  { 
    if ((TS_State.TouchDetected) & ( y > 0 ) & ( y < color_width ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      Update_Size(Radius);
    }
    else if ((TS_State.TouchDetected) & ( y > color_width ) & (y < (2 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
      Update_Size(Radius);
    }
    else if ((TS_State.TouchDetected) & (y > (2 * color_width)) & (y < (3 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
      Update_Size(Radius);
    }
    else if ((TS_State.TouchDetected) & (y > (3 * color_width)) & (y < (4 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_LIGHTMAGENTA);
      Update_Size(Radius);
    }
    else if ((TS_State.TouchDetected) & (y > (4 * color_width)) & (y < (5 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
      Update_Size(Radius);
    }
    else if ((TS_State.TouchDetected) & (y > (5 * color_width)) &(y < (6 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      Update_Size(Radius);
    }
    else if ((TS_State.TouchDetected) & (y > (6 * color_width)) &(y < (7 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_BROWN);
      Update_Size(Radius);
    }
    else if ((TS_State.TouchDetected) & (y > (7 * color_width)) & (y < (8 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      Update_Size(Radius);
    }
    else if ((TS_State.TouchDetected) & (y > (8 * color_width)) & (y < (9 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
      Update_Size(Radius);
    }
    else if ((TS_State.TouchDetected) & (y > (9 * color_width)) & (y < (10 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
      Update_Size(Radius);
    }
    else if ((TS_State.TouchDetected) & (y > (10 * color_width)) & (y < (11 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
      Update_Size(Radius);
    }
    else if ((TS_State.TouchDetected) & (y > (11 * color_width)) & (y < (12 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      Update_Size(Radius);
    }    
    else if ((TS_State.TouchDetected) &  (y > (12 * color_width)) & (y < (13 * color_width)))
    {
      /* Get the current text color */
      color = BSP_LCD_GetTextColor();
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      /* Clear the working window */
      BSP_LCD_FillRect(68, 8, (BSP_LCD_GetXSize() - 75), (BSP_LCD_GetYSize() - 75));
      BSP_LCD_SetTextColor(color);
    }
    else
    {
      x = 0;
      y = 0;
    }
    Update_Color();    
  }
  else if ((TS_State.TouchDetected) & (x > 70 ) & (y > (12 * color_width)) & (y < (13 * color_width)) & ( x < 120 ))
  {    
    Radius = 20;
    Update_Size(Radius);
  }
  else if ((TS_State.TouchDetected) & (x > 120 ) & (y > (12 * color_width)) & (y < (13 * color_width)) & ( x < 170 ))
  {    
    Radius = 15;
    Update_Size(Radius);
  }
  else if ((TS_State.TouchDetected) & (x > 170 ) & (y > (12 * color_width)) & (y < (13 * color_width)) & ( x < 220 ))
  {    
    Radius = 10;
    Update_Size(Radius);
  }
  else if ((TS_State.TouchDetected) & (x > 220 ) & (y > (12 * color_width)) & (y < (13 * color_width)) & ( x < 270 ))
  {    
    Radius = 5;
    Update_Size(Radius);
  }
  else if ((TS_State.TouchDetected) & (x > 270 ) & (y > (12 * color_width)) & (y < (13 * color_width)) & ( x < 320 ))
  {    
    Radius = 2;
    Update_Size(Radius);
  }  
  else if ((TS_State.TouchDetected) & (((x > (BSP_LCD_GetXSize()-5) ) & (y > (12 * color_width)) & (y < (13 * color_width))) | (( x < 55 ) & ( y < 5 ))))
  {    
  TS_State.x = 0;
  TS_State.y = 0;
  }  
  else if ((TS_State.TouchDetected) & (x > 320) & (y > (BSP_LCD_GetYSize() - 50)) & (x < 370) & (y < BSP_LCD_GetYSize() ))
  {   
    Save_Picture();
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

  if (BSP_LCD_GetXSize() == 640)
  {  
    /* Draw color image */
    BSP_LCD_DrawBitmap(0, 0, (uint8_t *)color);
  }
  else
  {
    /* Draw color image */
    BSP_LCD_DrawBitmap(0, 0, (uint8_t *)color2);
  }
  
  /* Draw save image */
  BSP_LCD_DrawBitmap(310, (BSP_LCD_GetYSize() - 50), (uint8_t *)save);

  /* Set Black as text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  
  /* Draw working window */
  BSP_LCD_DrawRect(61, 0, (BSP_LCD_GetXSize() - 61), (BSP_LCD_GetYSize() - 60));
  BSP_LCD_DrawRect(63, 3, (BSP_LCD_GetXSize() - 66), (BSP_LCD_GetYSize() - 66));
  BSP_LCD_DrawRect(65, 5, (BSP_LCD_GetXSize() - 70), (BSP_LCD_GetYSize() - 70));
  BSP_LCD_DrawRect(67, 7, (BSP_LCD_GetXSize() - 74), (BSP_LCD_GetYSize() - 74));
  
  /* Draw size icons */
  BSP_LCD_FillRect(60, (BSP_LCD_GetYSize() - 48), 250, 48);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillCircle(95, (BSP_LCD_GetYSize() - 24), 20);
  BSP_LCD_FillCircle(145, (BSP_LCD_GetYSize() - 24), 15);
  BSP_LCD_FillCircle(195, (BSP_LCD_GetYSize() - 24), 10);
  BSP_LCD_FillCircle(245, (BSP_LCD_GetYSize() - 24), 5);
  BSP_LCD_FillCircle(295, (BSP_LCD_GetYSize() - 24), 2);  
  
  BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
  BSP_LCD_SetFont(&Font8);
  BSP_LCD_DisplayStringAt(360, (BSP_LCD_GetYSize() - 55), (uint8_t *)"Selected Color  Size", LEFT_MODE);  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
  BSP_LCD_FillRect(380, (BSP_LCD_GetYSize() - 40), 30, 30);  
  BSP_LCD_FillCircle(450, (BSP_LCD_GetYSize()- 24), Radius); 
}

/**
  * @brief  Save the picture in USB Disk.
  * @param  None
  * @retval None
  */
void Save_Picture(void)
{ 
  FRESULT res1, res2;    /* FatFs function common result code */
  uint32_t byteswritten; /* File write count */
  static uint32_t color;

  /* Get the current text color */ 
  color = BSP_LCD_GetTextColor();  
  
  BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
  BSP_LCD_SetFont(&Font20);
  
  /* Turn LED1 and LED3 Off */
  BSP_LED_Off(LED1);
  BSP_LED_Off(LED3);
    
  if (Appli_state == APPLICATION_RUNNIG)
  {
    BSP_LCD_DisplayStringAt(0, (BSP_LCD_GetYSize()-35), (uint8_t *)"Saving ... ", RIGHT_MODE);
    
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
      if (BSP_LCD_GetXSize() == 640)
      {  
        /* if ampire 640x480 LCD is used */
        res1 = f_write(&MyFile, (uint32_t *)aBMPHeader1, 54, (void *)&byteswritten);
      }
      else
      {  
        /* if ampire 480x272 LCD is used */
        res1 = f_write(&MyFile, (uint32_t *)aBMPHeader2, 54, (void *)&byteswritten);
      }        
      /* Write the bmp file */
      res2 = f_write(&MyFile, (uint32_t *)CONVERTED_FRAME_BUFFER, ((BSP_LCD_GetYSize()-60)*(BSP_LCD_GetXSize()-60)*3), (void *)&byteswritten);
      
      if((res1 != FR_OK) || (res2 != FR_OK) || (byteswritten == 0))
      {
        /* 'image' file Write or EOF Error */
        BSP_LED_On(LED3);
        BSP_LCD_DisplayStringAt(0, (BSP_LCD_GetYSize()-35), (uint8_t *)" Aborted...", RIGHT_MODE);
        /* Wait for 2s */
        HAL_Delay(2000);
      }
      else
      {
        /*##-4- Close the open text file #####################################*/
        f_close(&MyFile);

        /* Success of the demo: no error occurrence */
        BSP_LED_On(LED1);
        BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
        BSP_LCD_DisplayStringAt(0, (BSP_LCD_GetYSize()-35), (uint8_t *)" Saved     ", RIGHT_MODE);
        /* Wait for 2s */
        HAL_Delay(2000);

        /* Draw save image */
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_FillRect(310, (BSP_LCD_GetYSize() - 35), (BSP_LCD_GetXSize() - 310), 35);
        BSP_LCD_DrawBitmap(310, (BSP_LCD_GetYSize() - 50), (uint8_t *)save);

        /* Reset text color */
        BSP_LCD_SetTextColor(color);

        /* Update color box */
        Update_Color();
      }
    }
  }
  else
  {
    /* USB not connected */
    BSP_LCD_DisplayStringAt(0, (BSP_LCD_GetYSize()-35), (uint8_t *)"USB KO... ", RIGHT_MODE);
    /* Wait for 2s */
    HAL_Delay(2000);
  }
    
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);     
  BSP_LCD_FillRect(480, 430, 160, 50);
  BSP_LCD_SetTextColor(color);
}

/**
  * @brief  Prepares the picture to be Saved in USB Disk.
  * @param  None
  * @retval None
  */
static void Prepare_Picture(void) 
{ 
  static DMA2D_HandleTypeDef hdma2d_eval;
  uint32_t address1 = CONVERTED_FRAME_BUFFER;
  uint32_t address2 = LCD_FRAME_BUFFER_LAYER0;
  uint32_t index = 0;
  
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_RGB888;
  hdma2d_eval.Init.OutputOffset = 0;     
  
  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d_eval.LayerCfg[1].InputOffset = 60;
  
  hdma2d_eval.Instance = DMA2D; 

  /* Bypass the bitmap header */
  address2 += ((BSP_LCD_GetXSize() * (BSP_LCD_GetYSize() - 61) + 60) * 4);  
  
  /* Convert picture to RGB888 pixel format */
  for(index=0; index < (BSP_LCD_GetYSize() - 60); index++)
  { 
    /* DMA2D Initialization */
    if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK) 
    {
      if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK) 
      {
        if (HAL_DMA2D_Start(&hdma2d_eval, address2, address1, (BSP_LCD_GetXSize() - 60), 1) == HAL_OK)
        {
          /* Polling For DMA transfer */  
          HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
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
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
  {
  }
}

/**
  * @brief  Update the selected color
  * @param  None
  * @retval None
  */
static void Update_Color(void)
{
  static uint32_t color;
  
  /* Get the current text color */
  color = BSP_LCD_GetTextColor();
  /* Update the selected color icon */
  BSP_LCD_FillRect(380, (BSP_LCD_GetYSize()-40), 30, 30);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);    
  BSP_LCD_DrawRect(380, (BSP_LCD_GetYSize()-40), 30, 30);
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
  BSP_LCD_FillCircle(450, (BSP_LCD_GetYSize()-24), 20);
  BSP_LCD_SetTextColor(color);  
  BSP_LCD_FillCircle(450, (BSP_LCD_GetYSize()-24), size);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);    
  BSP_LCD_DrawCircle(450, (BSP_LCD_GetYSize()-24), size);
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

  /* Enable HSE Oscillator and Activate PLL with HSE ad source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
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
