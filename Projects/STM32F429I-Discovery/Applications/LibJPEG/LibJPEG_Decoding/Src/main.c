/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Decoding/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  *          This sample code shows how to decompress JPEG file.
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
typedef enum
{
  APPLICATION_IDLE = 0,  
  APPLICATION_START    
} MSC_ApplicationTypeDef;  

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS USBDISK_FatFs;  /* File system object for USB logical drive */
FIL MyFile;           /* File object */
char USBDISKPath[4];  /* USB disk logical drive path */

uint32_t offset = 0xD0000000;
RGB_typedef *RGB_matrix;
uint8_t _aucLine[2048];

USBH_HandleTypeDef  hUSBHost;  

/* Variable to save the state of USB */
MSC_ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength);
static void USBH_UserProcess (USBH_HandleTypeDef *phost, uint8_t id );
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
  
  /* Configure the system clock to 180 MHz */
  SystemClock_Config();
  
  /*##-1- LCD Configuration ##################################################*/
  LCD_Config();
  
  /*##-2- Link the USB Host disk I/O driver ##################################*/
  if(FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
  {
    /*##-3- Init Host Library ################################################*/
    USBH_Init(&hUSBHost, USBH_UserProcess, 0);
    
    /*##-4- Add Supported Class ##############################################*/
    USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);
    
    /*##-5- Start Host Process ###############################################*/
    USBH_Start(&hUSBHost);
    
    /*##-6- Run Application (Blocking mode) ##################################*/
    while (1)
    {
      /* USB Host Background task */
      USBH_Process(&hUSBHost);
      
      /* Mass Storage Application State Machine */
      switch(Appli_state)
      {
      case APPLICATION_START:
          
        /* Open the JPG image with read access */
        if(f_open(&MyFile, "image.jpg", FA_READ) == FR_OK)
        {
          /* Decode the jpg image file */
          jpeg_decode(&MyFile, IMAGE_WIDTH, _aucLine, Jpeg_CallbackFunction);
            
          /* Close the JPG image */
          f_close(&MyFile);
        }
        
        Appli_state = APPLICATION_IDLE;
        break;
        
      case APPLICATION_IDLE:
      default:
        break;      
      }
    }
  }  
  
  /* TrueStudio compilation error correction */
  while (1)
  {
  }  
}

/**
  * @brief  LCD Configuration.
  * @param  None  
  * @retval None
  */
static void LCD_Config(void)
{
  /* Initialize the LCD */  
  BSP_LCD_Init();
  
  /* Background Layer Initialization */
  BSP_LCD_LayerDefaultInit(0, LCD_BUFFER);
  
  /* Set Foreground Layer */
  BSP_LCD_SelectLayer(0);
  
  /* Enable the LCD */
  BSP_LCD_DisplayOn();  
  
  /* Set the layer window */
  BSP_LCD_SetLayerWindow(0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
  
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
   
#ifdef DONT_USE_DMA2D
  RGB_matrix =  (RGB_typedef*)Row;
  uint32_t  ARGB32Buffer[IMAGE_WIDTH];
  uint32_t counter = 0;
   
  for(counter = 0; counter < IMAGE_WIDTH; counter++)
  {
    ARGB32Buffer[counter]  = (uint32_t)
    (
     ((RGB_matrix[counter].B << 16)|
      (RGB_matrix[counter].G << 8)|
      (RGB_matrix[counter].R) | 0xFF000000)
    );

    *(__IO uint32_t *)(LCD_BUFFER + (counter*4) + (offset - LCD_BUFFER)) = ARGB32Buffer[counter];
  }
#endif

#ifdef USE_DMA2D  
  static DMA2D_HandleTypeDef DMA2DHandle;
  
/* Configure the DMA2D Mode, Color Mode and output offset */
  DMA2DHandle.Init.Mode         = DMA2D_M2M_PFC;
  DMA2DHandle.Init.ColorMode    = DMA2D_ARGB8888;
  DMA2DHandle.Init.OutputOffset = 0;     
  
  /* Foreground Configuration */
  DMA2DHandle.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  DMA2DHandle.LayerCfg[1].InputAlpha = 0xFF;
  DMA2DHandle.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB888;
  DMA2DHandle.LayerCfg[1].InputOffset = 0;
  
  DMA2DHandle.Instance = DMA2D; 
  
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&DMA2DHandle) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&DMA2DHandle, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&DMA2DHandle, (uint32_t)Row, (uint32_t)offset, IMAGE_WIDTH, 1) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&DMA2DHandle, 10);
      }
    }
  }   
#endif

#ifdef SWAP_RB 
  uint32_t pixel = 0, width_counter, result = 0, result1 = 0;
  
  for(width_counter = 0; width_counter < IMAGE_WIDTH; width_counter++)
  {
    pixel = *(__IO uint32_t *)(LCD_BUFFER + (width_counter*4) + (offset - LCD_BUFFER)); 
    result1 = (((pixel & 0x00FF0000) >> 16) | ((pixel & 0x000000FF) << 16));
    pixel = pixel & 0xFF00FF00;
    result = (result1 | pixel);
    *(__IO uint32_t *)(LCD_BUFFER + (width_counter*4) + (offset - LCD_BUFFER)) = result;
    
  }  
#endif
  
  offset += (DataLength + IMAGE_WIDTH);
  return 0;
}

/**
  * @brief  User Process
  * @param  None
  * @retval None
  */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{  
  switch (id)
  { 
  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_IDLE;
    if (f_mount(0, "", 0) != FR_OK)
    {
      /* FatFs Initialization Error */
    }
    break;
  
  case HOST_USER_CONNECTION:
    Appli_state = APPLICATION_IDLE;
    if (f_mount(&USBDISK_FatFs, "", 0) != FR_OK)
    {
      /* FatFs Initialization Error */
    }
    break;    
    
  case HOST_USER_CLASS_ACTIVE:
    Appli_state = APPLICATION_START;
    break;
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
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig (&RCC_OscInitStruct);
  
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
