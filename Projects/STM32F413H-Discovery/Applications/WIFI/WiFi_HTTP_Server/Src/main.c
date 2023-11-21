/**
  ******************************************************************************
  * @file    Wifi/WiFi_HTTP_Server/src/main.c
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

/* Private defines -----------------------------------------------------------*/
/* Update SSID and PASSWORD with own Access point settings */
#define SSID     "MySSID" 
#define PASSWORD "MyPassword" 

#define LCD_USE

#define PORT           80
/* Private typedef------------------------------------------------------------*/
typedef enum
{
  WS_IDLE = 0,
  WS_CONNECTED,
  WS_DISCONNECTED,
  WS_ERROR,
} WebServerState_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/  
static   uint8_t http[1024];
static   uint8_t resp[1024];
uint16_t respLen;
uint8_t  IP_Addr[4]; 
uint8_t  MAC_Addr[6]; 
int32_t Socket = -1;
static   WebServerState_t  State = WS_ERROR;
static ADC_HandleTypeDef    AdcHandle;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static WIFI_Status_t SendWebPage(uint8_t ledIsOn, uint8_t temperature);
static void WebServerProcess(void);
static uint32_t TEMP_SENSOR_Init(void);
static uint32_t TEMP_SENSOR_GetValue(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
#if defined (LCD_USE)
  uint8_t modulestr[20] = {0};
#endif /* LCD_USE */ 

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();
  
  /* Configure LED3 and LED4 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  
  /*Initialize Temperature sensor */
  TEMP_SENSOR_Init();
  
  /* WIFI Web Server demonstration */
#if defined (LCD_USE)
  BSP_LCD_InitEx(LCD_ORIENTATION_LANDSCAPE_ROT180);
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), (BSP_LCD_GetYSize() -200));

  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
  BSP_LCD_DisplayStringAt(10, 10, (uint8_t *)"WIFI Web", CENTER_MODE );
  BSP_LCD_DisplayStringAt(10, 23, (uint8_t *)"Server demonstration", CENTER_MODE );   
#endif /* LCD_USE */
 
  /*Initialize and use WIFI module */
  if(WIFI_Init() ==  WIFI_STATUS_OK)
  {
#if defined (LCD_USE)
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"ES-WIFI Initialized.");
#endif /* LCD_USE */     
    
      if(WIFI_GetMAC_Address(MAC_Addr) == WIFI_STATUS_OK)
      {       
#if defined (LCD_USE)    
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
      BSP_LCD_DisplayStringAtLine(6, (uint8_t *)"es-wifi MAC Address:");
      sprintf((char*)modulestr,"%X:%X:%X:%X:%X:%X.", MAC_Addr[0],
              MAC_Addr[1],
              MAC_Addr[2],
              MAC_Addr[3],
              MAC_Addr[4],
              MAC_Addr[5]);
      
      BSP_LCD_DisplayStringAtLine(7, (uint8_t *) modulestr);
#endif /* LCD_USE */
      }
      else
      {
#if defined (LCD_USE)  
        BSP_LCD_SetTextColor(LCD_COLOR_RED); 
        BSP_LCD_DisplayStringAtLine(6, (uint8_t *)"CANNOT get MAC address");
#endif /* LCD_USE */  
        BSP_LED_On(LED4);
      }
    
   if( WIFI_Connect(SSID, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK)
    {
#if defined (LCD_USE)  
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
      BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"es-wifi connected");
#endif /* LCD_USE */ 
      
      if(WIFI_GetIP_Address(IP_Addr) == WIFI_STATUS_OK)
      {
#if defined (LCD_USE)
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);         
        BSP_LCD_DisplayStringAtLine(10, (uint8_t *)"es-wifi IP Address :");
        sprintf((char*)modulestr,"%d.%d.%d.%d",  IP_Addr[0],
                IP_Addr[1],
                IP_Addr[2],
                IP_Addr[3]); 
        
        BSP_LCD_DisplayStringAtLine(11, (uint8_t *) modulestr);
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
        BSP_LCD_DisplayStringAtLine(13, (uint8_t *)"Start HTTP Server... ");
        BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"Wait for connection...  ");
#endif /* LCD_USE */
        State = WS_IDLE;
        
      }
      else
      {    
#if defined (LCD_USE)  
        BSP_LCD_SetTextColor(LCD_COLOR_RED); 
        BSP_LCD_DisplayStringAtLine(12, (uint8_t *)"es-wifi CANNOT get IP address");
#endif /* LCD_USE */
        BSP_LED_On(LED4);
      }
    }
    else
    {
      
#if defined (LCD_USE)  
      BSP_LCD_SetTextColor(LCD_COLOR_RED); 
      BSP_LCD_DisplayStringAtLine(11, (uint8_t *)"es-wifi NOT connected\n");
#endif /* LCD_USE */ 
      BSP_LED_On(LED4);
    }
  }
  else
  {   
#if defined (LCD_USE)  
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"WIFI cannot be initialized.\n");
#endif /* LCD_USE */
    BSP_LED_On(LED4);
  }
    
  while(1)
  {
      WebServerProcess ();
  }
}

/**
  * @brief  Send HTML page
  * @param  None
  * @retval None
  */
static void WebServerProcess(void)
{
  uint8_t LedState = 0, temp;
  
  switch(State)
  {
  case WS_IDLE:
    Socket = 0;
    WIFI_StartServer(Socket, WIFI_TCP_PROTOCOL, "", PORT);
    
    if(Socket != -1)
    {
#if defined (LCD_USE) 
      BSP_LCD_ClearStringLine(14);
      BSP_LCD_DisplayStringAtLine(13, (uint8_t *)"HTTP Server Started");
#endif /* LCD_USE */
      State = WS_CONNECTED;
    }
    else
    {
#if defined (LCD_USE)  
      BSP_LCD_SetTextColor(LCD_COLOR_RED); 
      BSP_LCD_DisplayStringAtLine(13, (uint8_t *)"ERROR : Connection cannot be established");
#endif /* LCD_USE */
      State = WS_ERROR;
    }    
    break;
    
  case WS_CONNECTED:
    
    WIFI_ReceiveData(Socket, resp, 1200, &respLen);
    
    if( respLen > 0)
    {
      if(strstr((char *)resp, "GET")) /* GET: put web page */
      {
        temp = TEMP_SENSOR_GetValue();
        if(SendWebPage(LedState, temp) != WIFI_STATUS_OK)
        {
#if defined (LCD_USE)  
      BSP_LCD_SetTextColor(LCD_COLOR_RED); 
      BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"ERROR : Cannot send web page");
#endif /* LCD_USE */
          State = WS_ERROR;
        }
      }
      else if(strstr((char *)resp, "POST"))/* POST: received info */
      {
          if(strstr((char *)resp, "radio"))
          {          
            if(strstr((char *)resp, "radio=0"))
            {
              LedState = 0;
              BSP_LED_Off(LED3);
            }
            else if(strstr((char *)resp, "radio=1"))
            {
              LedState = 1;
              BSP_LED_On(LED3);
            } 
            
            temp = TEMP_SENSOR_GetValue();
            if(SendWebPage(LedState, temp) != WIFI_STATUS_OK)
            {
#if defined (LCD_USE)  
      BSP_LCD_SetTextColor(LCD_COLOR_RED); 
      BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"ERROR : Cannot send web page");
#endif /* LCD_USE */
              State = WS_ERROR;
          }
        }
      }
    }
    if(WIFI_StopServer(Socket) == WIFI_STATUS_OK)
    {
      WIFI_StartServer(Socket, WIFI_TCP_PROTOCOL, "", PORT);
    }
    else
    {
      State = WS_ERROR;  
    }
    break;
  case WS_ERROR:   
  default:
    break;
  }
}

/**
  * @brief  Send HTML page
  * @param  None
  * @retval None
  */
static WIFI_Status_t SendWebPage(uint8_t ledIsOn, uint8_t temperature)
{
  uint8_t  temp[50];
  uint16_t SentDataLength;
  WIFI_Status_t ret;
  
  /* construct web page content */
  strcpy((char *)http, (char *)"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n");
  strcat((char *)http, (char *)"<html>\r\n<body>\r\n");
  strcat((char *)http, (char *)"<title>STM32 Web Server</title>\r\n");
  strcat((char *)http, (char *)"<h2>InventekSys : Web Server using Es-Wifi with STM32</h2>\r\n");
  strcat((char *)http, (char *)"<br /><hr>\r\n");
  strcat((char *)http, (char *)"<p><form method=\"POST\"><strong>Temp: <input type=\"text\" size=2 value=\"");
  sprintf((char *)temp, "%d", temperature);
  strcat((char *)http, (char *)temp);
  strcat((char *)http, (char *)"\"> <sup>O</sup>C");
  
  if (ledIsOn) 
  {
    strcat((char *)http, (char *)"<p><input type=\"radio\" name=\"radio\" value=\"0\" >LED off");
    strcat((char *)http, (char *)"<br><input type=\"radio\" name=\"radio\" value=\"1\" checked>LED on");
  } 
  else 
  {
    strcat((char *)http, (char *)"<p><input type=\"radio\" name=\"radio\" value=\"0\" checked>LED off");
    strcat((char *)http, (char *)"<br><input type=\"radio\" name=\"radio\" value=\"1\" >LED on");
  }

  strcat((char *)http, (char *)"</strong><p><input type=\"submit\"></form></span>");
  strcat((char *)http, (char *)"</body>\r\n</html>\r\n");
  
  ret = WIFI_SendData(0, (uint8_t *)http, strlen((char *)http), &SentDataLength); 
  
  if((ret == WIFI_STATUS_OK) && (SentDataLength != strlen((char *)http)))
  {
    ret = WIFI_STATUS_ERROR;
  }
    
  return ret;
}


/**
  * @brief  Configure the ADC.
  * @param  None
  * @retval None
  */
static uint32_t TEMP_SENSOR_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;
  
   /* ADC1 Periph clock enable */
  __HAL_RCC_ADC1_CLK_ENABLE();
  
  /* Configure the ADC peripheral */
  AdcHandle.Instance          = ADC1;
  
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode enabled to have continuous conversion */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion   = 0;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;        /* Conversion start not triggered by an external event */
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.NbrOfConversion       = 1;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  AdcHandle.Init.EOCSelection          = DISABLE;
  
  if (HAL_ADC_Init(&AdcHandle)== HAL_OK)
  {
    /* Configure ADC Temperature Sensor Channel */
    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
    sConfig.Offset = 0;
    
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) == HAL_OK)
    {
      return 0;
    }
  }
  return 1;
}


/**
  * @brief  Get JunctionTemp level in 12 bits.
  * @retval JunctionTemp level(0..0xFFF) / 0xFFFFFFFF : Error
  */
static uint32_t TEMP_SENSOR_GetValue(void)
{   
 
  if(HAL_ADC_Start(&AdcHandle) == HAL_OK)
  {
    if(HAL_ADC_PollForConversion(&AdcHandle, 1000)== HAL_OK)
    {
      return ((((HAL_ADC_GetValue(&AdcHandle) * VREF)/MAX_CONVERTED_VALUE) - VSENS_AT_AMBIENT_TEMP) * 10 / AVG_SLOPE) + AMBIENT_TEMP;
    } 
  }
  return  0xFFFFFFFF;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 100000000
  *            HCLK(Hz)                       = 100000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 200
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);

  if(ret != HAL_OK)
  {
   Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
   Error_Handler();
  }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif
