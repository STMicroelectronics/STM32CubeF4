/**
  ******************************************************************************
  * @file    Camera/Camera_To_USBDisk/Src/main.c 
  * @author  MCD Application Team
  * @brief   This application describes how to configure the camera in continuous mode
             and save picture under USBDisk.
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
#define PRESSED_FIRST    0x00

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t ubPressedButton = PRESSED_FIRST;

FATFS MSC_FatFs;  /* File system object for USB disk logical drive */
FIL MyFile;       /* File object */
char MSC_Path[4]; /* USB Host logical drive path */
USBH_HandleTypeDef  hUSBHost;

/* Image header */  
const uint32_t aBMPHeader[14]=
{0xFA364D42, 0x00000005, 0x00360000, 0x00280000, 0x01E00000, 0x01100000, 0x00010000, 
 0x00000018, 0xF5400000, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0000};

typedef enum {
  STORAGE_IDLE = 0,  
  STORAGE_READY,    
}MSC_ApplicationTypeDef;

MSC_ApplicationTypeDef Appli_state = STORAGE_IDLE;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void PicturePrepare(void);
static void Error_Handler(void);
static void SavePicture(void);
static void LCD_LL_ConvertLineToARGB8888(void *pSrc, void *pDst);
static void CAMERA_Capture(void);
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

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
  
  /*##-1- Init Host Library ##################################################*/
  USBH_Init(&hUSBHost, USBH_UserProcess, 0);
  
  /* Add Supported Class */
  USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);
  
  /* Start Host Process */
  USBH_Start(&hUSBHost);
  
  /*##-2- Disable SAI1_SDA signal ############################################*/  
  /* Note: In STM324x9I-EVAL RevB, PE6 pin is shared between data_7 of camera 
           and SAI1_SDA of codec WM8994, after power on, SAI1_SDA pin of codec WM8994 
           is in output state, thus preventing MCU from receiving correct signal 
           from camera, so we need to configure SAI1_SDA pin of codec WM8994 
           in tri-state */
  
  /* Initialize the Control interface of the Audio Codec */
  BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_SPEAKER, 70, AUDIO_FREQUENCY_48K); 
  /* ADCDAT1 is tri-stated */
  AUDIO_IO_Write(AUDIO_I2C_ADDRESS, 0x200, 0);
  AUDIO_IO_Write(AUDIO_I2C_ADDRESS, 0x300, 0x6010);  

  /*##-3- Configure TAMPER Button ############################################*/
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);
  
  /*##-4- Link the USB Host disk I/O driver ##################################*/
  FATFS_LinkDriver(&USBH_Driver, MSC_Path);
  
  /*##-5- Initialize the LCD #################################################*/
  BSP_LCD_Init();
  
  /* Foreground Layer Initialization */
  BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER_LAYER1);
  /* Set Foreground Layer */
  BSP_LCD_SelectLayer(1);
  /* Clear the LCD Foreground layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetLayerVisible(1, DISABLE);
  
  /* Background Layer Initialization */
  BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
  
  /* Enable the LCD */
  BSP_LCD_DisplayOn();   
  
  /* Select the LCD Foreground layer */
  BSP_LCD_SelectLayer(0);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /*##-6- Camera Initialization and start capture ############################*/
  /* Initialize the Camera */
  BSP_CAMERA_Init(RESOLUTION_R480x272);
  
  /* Start the Camera Capture */
  BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FRAME_BUFFER);
  
  /*##-7- Run Application ####################################################*/
  while (1)
  {
    /* USB Host Background task */
    USBH_Process(&hUSBHost);
    
    switch(Appli_state)
    {
    case STORAGE_READY:
      CAMERA_Capture();
      break;
      
    case STORAGE_IDLE:
      break;      
    } 
  }
}

/**
  * @brief  Saves Picture into USB disk.
  * @param  None
  * @retval None
  */
static void SavePicture(void)
{
  FRESULT res1, res2;     /* FatFs function common result code */
  uint32_t byteswritten;  /* File write count */
  static uint32_t counter = 0;
  uint8_t str[30];
  
  /* Suspend the camera capture */
  BSP_CAMERA_Suspend();
  
  /* Prepare the image to be saved */
  PicturePrepare();
  
  /* Set foreground Layer */
  BSP_LCD_SetLayerVisible(1, ENABLE);
  BSP_LCD_SetColorKeying(1, LCD_COLOR_WHITE);
  BSP_LCD_SelectLayer(1);
  BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
  BSP_LCD_SetFont(&Font24);
  
  /* Format the string */
  sprintf((char *)str,"image_%d.bmp", (int)counter);
  
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"Saving ..", RIGHT_MODE);
  
  /* Create and Open a new text file object with write access */
  if(f_open(&MyFile, (const char*)str, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) 
  {
    Error_Handler();
  }
  else
  {
    /* Write data to the BMP file */
    res1 = f_write(&MyFile, (uint32_t *)aBMPHeader, 54, (void *)&byteswritten);
    res2 = f_write(&MyFile, (uint32_t *)CONVERTED_FRAME_BUFFER, (BSP_LCD_GetXSize()*BSP_LCD_GetYSize()*3), (void *)&byteswritten);
    
    if((res1 != FR_OK) || (res2 != FR_OK) || (byteswritten == 0))
    {
      Error_Handler();
    }
    else
    {
      /* Close the open BMP file */
      f_close(&MyFile);
      
      /* Success of the demo: no error occurrence */
      BSP_LED_On(LED1);
      BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
      BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize()-24), (uint8_t *)"   Saved  ", RIGHT_MODE);
      /* Wait for 2s */
      HAL_Delay(2000);
      /* Disable the Layer 2 */
      BSP_LCD_SetLayerVisible(1, DISABLE);
      /* Select Layer 1 */
      BSP_LCD_SelectLayer(0); 
      counter++;
      BSP_LED_Off(LED1);
      
      /* Resume the camera capture */
      BSP_CAMERA_Resume();      
    }
  }
}

/**
  * @brief  Main routine for Camera capture
  * @param  None
  * @retval None
  */
static void CAMERA_Capture(void)
{
  while(1)
  {
    if(BSP_PB_GetState(BUTTON_TAMPER) != GPIO_PIN_SET) 
    {
      SavePicture();
    }
  }
}

/**
  * @brief  Line event callback.
  * @param  None
  * @retval None
  */
void BSP_CAMERA_LineEventCallback(void)
{
  static uint32_t camera_datapointer, lcd_datapointer, line_number;
  
  if(BSP_LCD_GetYSize() > line_number)
  {
    LCD_LL_ConvertLineToARGB8888((uint32_t *)(CAMERA_FRAME_BUFFER + camera_datapointer), (uint32_t *)(LCD_FRAME_BUFFER + lcd_datapointer));
    camera_datapointer = camera_datapointer + BSP_LCD_GetXSize() * (sizeof(uint16_t)); 
    lcd_datapointer = lcd_datapointer + BSP_LCD_GetXSize() * (sizeof(uint32_t));
    line_number++;
  }
  else
  {
    camera_datapointer = 0;
    lcd_datapointer = 0;
    line_number = 0;
  }
}

/**
  * @brief  Prepares the picture to be Saved in USB.
  * @param  None
  * @retval None
  */
static void PicturePrepare(void) 
{ 
  static DMA2D_HandleTypeDef hdma2d_eval;
  uint32_t address1 = CONVERTED_FRAME_BUFFER;
  uint32_t address2 = LCD_FRAME_BUFFER;
  uint32_t index = 0;
  
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_RGB888;
  hdma2d_eval.Init.OutputOffset = 0;     
  
  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;
  
  hdma2d_eval.Instance = DMA2D; 

  /* Go to the end of the bitmap file address */
  address2 += ((BSP_LCD_GetXSize() * (BSP_LCD_GetYSize() - 1)) * 4);  
  
  /* Convert picture to RGB888 pixel format */
  for(index = 0; index < (BSP_LCD_GetYSize()); index++)
  { 
    /* DMA2D Initialization */
    if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK) 
    {
      if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK) 
      {
        if (HAL_DMA2D_Start(&hdma2d_eval, address2, address1, (BSP_LCD_GetXSize()), 1) == HAL_OK)
        {
          /* Polling For DMA transfer */  
          HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
        }
      }
    }    
    /* Increment the source and destination buffers */
    address1 += ((BSP_LCD_GetXSize())*3);
    address2 -= BSP_LCD_GetXSize()*4;
  }
}

/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color  
  * @retval None
  */
static void LCD_LL_ConvertLineToARGB8888(void *pSrc, void *pDst)
{ 
  static DMA2D_HandleTypeDef hdma2d_eval;  
  
  /* Enable DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();
  
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_ARGB8888;
  hdma2d_eval.Init.OutputOffset = 0;     
  
  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;
  
  hdma2d_eval.Instance = DMA2D; 
  
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&hdma2d_eval, (uint32_t)pSrc, (uint32_t)pDst, BSP_LCD_GetXSize(), 1) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
      }
    }
  } 
}

/**
  * @brief  User Process
  * @param  phost: Host handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{  
  switch (id)
  { 
  case HOST_USER_SELECT_CONFIGURATION:
    break;
    
  case HOST_USER_DISCONNECTION:
    Appli_state = STORAGE_IDLE;
    f_mount(NULL, (TCHAR const*)"", 0);      
    break;
    
  case HOST_USER_CLASS_ACTIVE:
    /* Register the file system object to the FatFs module */
    if(f_mount(&MSC_FatFs, (TCHAR const*)MSC_Path, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      Error_Handler();
    }
    else
    {
      Appli_state = STORAGE_READY;
    }
    break;
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
  BSP_LCD_SelectLayer(0); 
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
