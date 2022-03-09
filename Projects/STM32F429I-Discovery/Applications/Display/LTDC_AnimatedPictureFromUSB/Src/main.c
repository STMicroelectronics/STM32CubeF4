/**
  ******************************************************************************
  * @file    Display/LTDC_AnimatedPictureFromUSB/Src/main.c
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

/** @addtogroup STM32F4xx_HAL_Applications
  * @{
  */

/** @addtogroup LTDC_AnimatedPictureFromUSB
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  APPLICATION_IDLE = 0,  
  APPLICATION_START    
}
MSC_ApplicationTypeDef;

/* Private define ------------------------------------------------------------*/
#define IMAGE_BUFFER_SIZE    512

#define USH_USR_FS_INIT       0
#define USH_USR_FS_READLIST   1
#define USH_USR_FS_WRITEFILE  2
#define USH_USR_FS_DRAW       3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBH_HandleTypeDef  hUSBHost;
FATFS USBDISK_FatFs;
uint8_t USBDISK_Driver_Num;  /* USB Host logical drive number */
FIL file;
uint8_t Image_Buf[IMAGE_BUFFER_SIZE];
uint32_t BytesRead = 0;

MSC_ApplicationTypeDef Appli_state = APPLICATION_IDLE;
uint8_t USBH_USR_ApplicationState = USH_USR_FS_INIT;
__IO uint8_t line_idx = 0;   

/* Private function prototypes -----------------------------------------------*/
static void LCD_Config(void);
static void MSC_Application(void);
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
static uint8_t Explore_Disk(char *path , uint8_t recu_level);
static uint8_t Image_Browser(char *path);
static void Show_Image(void);
static uint32_t Storage_OpenReadFile(uint32_t Address);
static void Toggle_Leds(void);
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
  
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();

  /* Configure LED3 and LED4 */
  BSP_LED_Init(LED3); 
  BSP_LED_Init(LED4); 
  
  /* Configure USER Button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
  
  /* Initialize LCD driver */
  LCD_Config();
  
  /* Link the USB Host disk I/O driver */
  USBDISK_Driver_Num = FATFS_LinkDriver(&USBH_Driver, "");
  
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

  /* Infinite loop */
  while (1)
  {
    if (Appli_state == APPLICATION_START)
    {
      MSC_Application();
    }
    Toggle_Leds();
    USBH_Process(&hUSBHost);
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
  BSP_LCD_Init();

  /* LCD Layers Initialization */ 
  BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, (LCD_FRAME_BUFFER + BUFFER_OFFSET));
  
  /* Configure the transparency for foreground : Increase the transparency */
  BSP_LCD_SetTransparency(LCD_BACKGROUND_LAYER, 0);
  BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);

  /* LCD Log initialization */
  LCD_LOG_Init(); 

  LCD_LOG_SetHeader((uint8_t *)"LTDC Application");
  LCD_UsrLog("> USB Host library started.\n"); 
  LCD_LOG_SetFooter ((uint8_t *)"     USB Host Library V3.2.0" );
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
  * @brief  MSC class application
  * @param  None
  * @retval None
  */
static void MSC_Application(void)
{
  switch(USBH_USR_ApplicationState)
  {
  case USH_USR_FS_INIT: 
    /* Register work area for logical drives */
    if (f_mount(&USBDISK_FatFs, "", 0) != FR_OK)
    {
      LCD_ErrLog("> File System NOT initialized.\n");
      Error_Handler();
    }
    else
    {
      LCD_UsrLog("> File System initialized.\n");
      USBH_USR_ApplicationState = USH_USR_FS_READLIST;
    }
    break;
    
  case USH_USR_FS_READLIST:
    LCD_UsrLog("> Exploring disk flash ...\n");
    if (Explore_Disk("0:/", 1) != FR_OK)
    {
      LCD_ErrLog("> File cannot be explored.\n");
      Error_Handler();
    }
    else 
    {
      line_idx = 0;   
      USBH_USR_ApplicationState = USH_USR_FS_DRAW; 
      
      LCD_UsrLog("To start Image Slideshow\n");
      LCD_UsrLog("Press Key\n");
      while(BSP_PB_GetState (BUTTON_KEY) != SET)
      {
        Toggle_Leds();
      }
    }
    break;
    
  case USH_USR_FS_DRAW:
    /* USER Button in polling */
    while(BSP_PB_GetState (BUTTON_KEY) != RESET)
    {
      Toggle_Leds();
    }
    
    while(Appli_state == APPLICATION_START)
    {
      Image_Browser("0:/");
      return;
    }
    break;
    
  default: 
    break;
  }
  return;
}

/**
  * @brief  Displays disk content
  * @param  path: pointer to root path
  * @param  recu_level: explorer level
  * @retval None
  */
static uint8_t Explore_Disk(char* path , uint8_t recu_level)
{
  FRESULT res;
  FILINFO fno;
  DIR dir;
  char *fn;
  char tmp[14];
  
  res = f_opendir(&dir, path);
  
  if (res == FR_OK) 
  {
    /* USER Button in polling */
    LCD_UsrLog("To see the disk root content:\n" );
    LCD_UsrLog("Press Key...\n");
    while((BSP_PB_GetState (BUTTON_KEY) != SET))          
    {
      Toggle_Leds();
    }
    while((BSP_PB_GetState (BUTTON_KEY) != RESET))          
    {
      Toggle_Leds();
    }
    while(Appli_state == APPLICATION_START) 
    {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0) 
      {
        break;
      }
      if (fno.fname[0] == '.')
      {
        continue;
      }
      
      fn = fno.fname;
      strcpy(tmp, fn); 
      
      line_idx++;
      if(line_idx > 12)
      {
        line_idx = 0;
        LCD_UsrLog("Press Key to continue...\n");
        
        /* USER Button in polling */
        while((Appli_state == APPLICATION_START) && \
          (BSP_PB_GetState (BUTTON_KEY) != SET))
        {
          Toggle_Leds();
        }
      } 
      if(recu_level == 1)
      {
        LCD_DbgLog("   |__");
      }
      else if(recu_level == 2)
      {
        LCD_DbgLog("   |   |__");
      }
      if((fno.fattrib & AM_DIR) == AM_DIR)
      {
        strcat(tmp, "\n"); 
        LCD_UsrLog((void *)tmp);
      }
      else
      {
        strcat(tmp, "\n"); 
        LCD_DbgLog((void *)tmp);
      }
      
      if(((fno.fattrib & AM_DIR) == AM_DIR)&&(recu_level == 1))
      {
        Explore_Disk(fn, 2);
      }
    }
  }
  f_closedir(&dir);
  return res;
}

/**
  * @brief  Image browser
  * @param  path: pointer to root path
  * @retval None
  */
static uint8_t Image_Browser(char *path)
{
  FRESULT res;
  uint8_t ret = 1;
  FILINFO fno;
  DIR dir;
  char *fn;
  
  res = f_opendir(&dir, path);
  if (res != FR_OK) 
  {
    Error_Handler();
  }
  else
  {    
    for (;;) {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0) break;
      if (fno.fname[0] == '.') continue;
      
      fn = fno.fname;
      
      if (fno.fattrib & AM_DIR) 
      {
        continue;
      } 
      else 
      {
        if((strstr(fn, "bmp")) || (strstr(fn, "BMP")))
        {
          res = f_open(&file, fn, FA_OPEN_EXISTING | FA_READ);
          Show_Image();
          USBH_Delay(100);
          ret = 0;
          while((Appli_state == APPLICATION_START) && \
            (BSP_PB_GetState (BUTTON_KEY) != SET))
          {
            Toggle_Leds();
          }
          f_close(&file);
          
        }
      }
    }  
  }
  
    /* LCD Log initialization */
  LCD_LOG_Init(); 

  LCD_LOG_SetHeader((uint8_t *)"LTDC Application"); 
  LCD_LOG_SetFooter ((uint8_t *)"     USB Host Library V3.2.0" );
  USBH_USR_ApplicationState = USH_USR_FS_READLIST;
  
  f_closedir(&dir);
  return ret;
}

/**
  * @brief  Displays Image
  * @param  None
  * @retval None
  */
static void Show_Image(void)
{
  Storage_OpenReadFile(LCD_FRAME_BUFFER);
  BSP_LCD_DrawBitmap(0, 0, (uint8_t*)LCD_FRAME_BUFFER);
}

/**
  * @brief  Open a file and copy its content to a buffer
  * @param  Address: the Directory name to open
  * @retval err: Error status (0=> success, 1=> fail)
  */
static uint32_t Storage_OpenReadFile(uint32_t Address)
{
  uint32_t index = 0, size = 0, i1 = 0;
  uint32_t BmpAddress;

  f_read(&file, &Image_Buf, 30, 0);
  BmpAddress = (uint32_t)Image_Buf;

  /* Read bitmap size */
  size = *(uint16_t *) (BmpAddress + 2);
  size |= (*(uint16_t *) (BmpAddress + 4)) << 16;  
 
  /* Get bitmap data address offset */
  index = *(uint16_t *) (BmpAddress + 10);
  index |= (*(uint16_t *) (BmpAddress + 12)) << 16;  
  
  f_lseek (&file, 0);

  do
  {
    if (size < 256*2)
    {
      i1 = size;
    }
    else
    {
      i1 = 256*2;
    }
    size -= i1;
    f_read(&file, &Image_Buf, i1, (UINT *)&BytesRead);

    for (index = 0; index < i1; index++)
    {
      *(__IO uint8_t*) (Address) = *(__IO uint8_t *)BmpAddress;
      
      BmpAddress++;  
      Address++;
    }  
    
    BmpAddress = (uint32_t)Image_Buf;
  }
  while (size > 0);
  
  return 1;
}

/**
  * @brief  Toggle LEDs to show user input state.
  * @param  None
  * @retval None
  */
static void Toggle_Leds(void)
{
  static uint32_t i;
  if (i++ == 0x10000)
  {
    BSP_LED_Toggle(LED3);
    BSP_LED_Toggle(LED4);
    i = 0;
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

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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
