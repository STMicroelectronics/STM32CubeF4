/**
  ******************************************************************************
  * @file    Wifi/WiFi_Client_Server/src/main.c
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

#define CONNECTION_TRIAL_MAX          10
/* Private variables ---------------------------------------------------------*/
const uint8_t RemoteIP[] = {192,168,43, 246};
uint8_t RxData [500];

uint8_t TxData[] = "STM32 : Hello!\n";
uint16_t RxLen;
uint8_t  MAC_Addr[6]; 
uint8_t  IP_Addr[4]; 
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
  int32_t Socket = -1;
  uint16_t Datalen;
  uint16_t Trials = CONNECTION_TRIAL_MAX;
#if defined (LCD_USE)
  uint8_t modulestr[20] = {0};
#endif /* LCD_USE */  
  
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();
  /* Configure LED4 */
  BSP_LED_Init(LED4);
#if defined (LCD_USE)
  BSP_LCD_InitEx(LCD_ORIENTATION_LANDSCAPE_ROT180);
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), (BSP_LCD_GetYSize() -200));
  
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
  BSP_LCD_DisplayStringAt(10, 10, (uint8_t *)"ES-WIFI Module in TCP", CENTER_MODE );
  BSP_LCD_DisplayStringAt(10, 23, (uint8_t *)"Client mode demonstration", CENTER_MODE );  
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
  BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"TCP Client Instructions:");
  BSP_LCD_DisplayStringAtLine(6, (uint8_t *)"1-Make sure your Phone connected ");
  BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"to the network that you configured");
  BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"using Configuration AccessPoint.");
  BSP_LCD_DisplayStringAtLine(10,(uint8_t *)"2-Create a server by using the ");
  BSP_LCD_DisplayStringAtLine(11,(uint8_t *)"android application TCP Server");
  BSP_LCD_DisplayStringAtLine(12,(uint8_t *)"with port(8002).");
  BSP_LCD_DisplayStringAtLine(14,(uint8_t *)"3-Get the Network Name or IP Address");
  BSP_LCD_DisplayStringAtLine(15,(uint8_t *)"of your Android from the step 2.");
  
  HAL_Delay(2000);
#endif /* LCD_USE */
  /*Initialize  WIFI module */
  if(WIFI_Init() ==  WIFI_STATUS_OK)
  {
#if defined (LCD_USE)
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), (BSP_LCD_GetYSize() -200));
    
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_DisplayStringAt(10, 10, (uint8_t *)"ES-WIFI Module in TCP", CENTER_MODE );
    BSP_LCD_DisplayStringAt(10, 23, (uint8_t *)"Client mode demonstration", CENTER_MODE );  
    
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
    BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"ES-WIFI Module Initialized.");
#endif /* LCD_USE */   
    if(WIFI_GetMAC_Address(MAC_Addr) == WIFI_STATUS_OK)
    {
#if defined (LCD_USE)    
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
      BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"Es-wifi module MAC Address:");
      sprintf((char*)modulestr,"%X:%X:%X:%X:%X:%X.", MAC_Addr[0],
              MAC_Addr[1],
              MAC_Addr[2],
              MAC_Addr[3],
              MAC_Addr[4],
              MAC_Addr[5]);
      
      BSP_LCD_DisplayStringAtLine(10, (uint8_t *) modulestr);
#endif /* LCD_USE */       
    }
    else
    {
#if defined (LCD_USE)  
      BSP_LCD_SetTextColor(LCD_COLOR_RED); 
      BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"ERROR : CANNOT get MAC address");
#endif /* LCD_USE */  
      BSP_LED_On(LED4);      
    }
    
    if( WIFI_Connect(SSID, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK)
    {
#if defined (LCD_USE)  
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
      BSP_LCD_DisplayStringAtLine(11, (uint8_t *)"Es-wifi module connected");
#endif /* LCD_USE */       
      if(WIFI_GetIP_Address(IP_Addr) == WIFI_STATUS_OK)
      {
#if defined (LCD_USE)        
        BSP_LCD_DisplayStringAtLine(12, (uint8_t *)"es-wifi module got IP Address :");
        sprintf((char*)modulestr,"%d.%d.%d.%d",  IP_Addr[0],
                IP_Addr[1],
                IP_Addr[2],
                IP_Addr[3]); 
        
        BSP_LCD_DisplayStringAtLine(13, (uint8_t *) modulestr);
        
        BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"Trying to connect to Server:");
        sprintf((char*)modulestr,"%d.%d.%d.%d:8002",  RemoteIP[0],
                RemoteIP[1],
                RemoteIP[2],
                RemoteIP[3]); 
        
        BSP_LCD_DisplayStringAtLine(15, (uint8_t *) modulestr);
        
#endif /* LCD_USE */  
        while (Trials--)
        { 
          if( WIFI_OpenClientConnection(0, WIFI_TCP_PROTOCOL, "TCP_CLIENT", (char *)RemoteIP, 8002, 0) == WIFI_STATUS_OK) 
          {
#if defined (LCD_USE) 
            BSP_LCD_DisplayStringAtLine(16, (uint8_t *)"TCP Connection opened successfully.");
#endif /* LCD_USE */  
            Socket = 0;
          }
        }
        if(!Trials)
        {
#if defined (LCD_USE)  
          BSP_LCD_SetTextColor(LCD_COLOR_RED); 
          BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"ERROR : Cannot open Connection");
#endif /* LCD_USE */
          BSP_LED_On(LED4);    
        }
      }
      else
      {
#if defined (LCD_USE)  
        BSP_LCD_SetTextColor(LCD_COLOR_RED); 
        BSP_LCD_DisplayStringAtLine(12, (uint8_t *)"ERROR: es-wifi module CANNOT get IP address");
#endif /* LCD_USE */
        BSP_LED_On(LED4);      
      }
    }
    else
    {
#if defined (LCD_USE)  
      BSP_LCD_SetTextColor(LCD_COLOR_RED); 
      BSP_LCD_DisplayStringAtLine(11, (uint8_t *)"ERROR:es-wifi module NOT connected\n");
#endif /* LCD_USE */ 
      BSP_LED_On(LED4);
    }
  }
  else
  {
#if defined (LCD_USE)  
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"ERROR : WIFI Module cannot be initialized.\n");
#endif /* LCD_USE */
    BSP_LED_On(LED4);
  }
  
  while(1)
  {
    if(Socket != -1)
    {
      if(WIFI_ReceiveData(Socket, RxData, sizeof(RxData), &Datalen) == WIFI_STATUS_OK)
      {
        if(Datalen > 0)
        {
          if(WIFI_SendData(Socket, TxData, sizeof(TxData), &Datalen) != WIFI_STATUS_OK)
          {
#if defined (LCD_USE)  
            BSP_LCD_SetTextColor(LCD_COLOR_RED); 
            BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"ERROR : Failed to send Data.");
#endif /* LCD_USE */ 
          } 
        }
      }
      else
      {
#if defined (LCD_USE)  
            BSP_LCD_SetTextColor(LCD_COLOR_RED); 
            BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"ERROR : Failed to Receive Data.");
#endif /* LCD_USE */  
      }
    }
  }
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
    while(1);
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
    while(1);
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
