/**
  ******************************************************************************
  * @file    CEC/CEC_MultiAddress/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use the CEC through 
  *          the STM32F4xx HAL API.
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

/** @addtogroup CEC_MultiAddress
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t  Tab_Rx[CEC_MAX_PAYLOAD];   /* Received data buffer. Max size = 16 bytes
                                     * header + opcode followed by up to 14 operands */
uint8_t  Tab_Tx[CEC_MAX_PAYLOAD-1]; /* Transmitted data buffer. 
                                     * header is not included in Tab_Tx.
                                     *  Max size = 15 bytes.                                   
                                     *  one opcode followed by up to 14 operands.
                                     *  When payload size = 0, only the header is sent
                                     *  (ping operation) */                                   
uint8_t ReceivedFrame       = 0x0;  /* Set when a reception occurs */
uint16_t NbOfReceivedBytes  = 0x0;  /* Number of received bytes in addition to the header. 
                                     * when a ping message has been received (header 
                                     * only), NbOfReceivedBytes = 0 */
uint8_t StartSending        = 0x0;  /* Set when a transmission is triggered by the user */
uint32_t TxSize             = 0x0;  /* Number of bytes to transmit in addition to the header. 
                                     * In case of ping operation (only the header sent),
                                     * TxSize = 0 */
uint8_t MyLogicalAddress1   = 0x0;  /* Transmitter logical address 1 */
uint8_t MyLogicalAddress2   = 0x0;  /* Transmitter logical address 2 (if applicable) */ 
uint8_t MyFollowerAddress1  = 0x0;  /* Destination logical address 1 */
uint8_t MyFollowerAddress2  = 0x0;  /* Destination logical address 2 (if applicable) */
uint8_t DestinationAddress  = 0x0;  /* Destination logical address, set on the
                                     * fly based on the button pushed by the user */
__IO uint8_t TxStatus = 0;
                           
CEC_HandleTypeDef hcec;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CEC_Config(CEC_HandleTypeDef *hcec);
static void CEC_FlushRxBuffer(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
 /* This sample code shows how to use STM32F4xx CEC HAL API to transmit and 
  * receive data. The device is set in waiting to receive mode and sends
  * messages when the evaluation board buttons are pushed by the user */
  
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  /* -1- Initialize LEDs mounted on EVAL board */
  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* -2- Configure User push-button in Interrupt mode */
  /* button-triggered interruptions initialization */
  BSP_PB_Init(BUTTON_TAMPER,BUTTON_MODE_EXTI);
  
  /* -3- Configure Joystick Selection push-button in Interrupt mode */
  BSP_JOY_Init(JOY_MODE_EXTI);

  
  /* CEC device initialization */
  /* -4- CEC configuration (transfer will take place in Interrupt mode) */
  
#if defined (DEVICE_1)
  /* Device 1: 
   * a single logical address ... */
  MyLogicalAddress1 = DEVICE_ADDRESS_1;
  /* ... and two different follower addresses */
  MyFollowerAddress1 = DEVICE_ADDRESS_2;
  MyFollowerAddress2 = DEVICE_ADDRESS_3;
#elif defined (DEVICE_2)
  /* Device 2: 
   * two different logical addresses ... */
  MyLogicalAddress1 = DEVICE_ADDRESS_2;
  MyLogicalAddress2 = DEVICE_ADDRESS_3;  
  /* ... and a single follower address */
  MyFollowerAddress1 = DEVICE_ADDRESS_1;
#endif /* DEVICE_1 */  
    
  /* -5- CEC configuration (transfer will take place in Interrupt mode) */
  hcec.Instance = CEC;
  /* Deinitialize CEC to reinitialize from scratch */
  HAL_CEC_DeInit(&hcec);
  /* IP configuration */ 
  CEC_Config(&hcec);
  
  /* -6- CEC transfer general variables initialization */
  ReceivedFrame = 0;
  StartSending = 0;
  NbOfReceivedBytes = 0;
  CEC_FlushRxBuffer();
  
 while (1)
  { 
    /* if no reception has occurred and no error has been detected,
     transmit a message if the user has pushed a button */
    while( (StartSending == 1) && (ReceivedFrame == 0))
    { 
      HAL_CEC_Transmit_IT(&hcec, MyLogicalAddress1 ,DestinationAddress, (uint8_t *)&Tab_Tx, TxSize);
      /* loop until TX ends or TX error reported */
      while (TxStatus != 1);
      StartSending = 0;
      TxStatus = 0;
    }  
    
    /* if a frame has been received */
    if (ReceivedFrame == 1)
    { 
      /* Test on the Destination Logical Address
       * (code applicable whatever the device):
       * if Receiver has address 0x01 */ 
      if ((Tab_Rx[0]&0x0F) == 0x01) 
      {
        /* Turn on LED1 */
        BSP_LED_On(LED1);
        /* Turn on LED4 */
        BSP_LED_On(LED4);
        /* Turn off LED3 */
        BSP_LED_Off(LED3);
        }
      /* if Receiver has address 0x03 */ 
      else if ((Tab_Rx[0]&0x0F) == 0x03) 
        {
        /* Turn on LED1 */
        BSP_LED_On(LED1);
        /* Turn off LED4 */
        BSP_LED_Off(LED4);
        /* Turn off LED3 */
        BSP_LED_Off(LED3);
        }
      /* if Receiver has address 0x05 */
      else if ((Tab_Rx[0]&0x0F) == 0x05) 
      {
        /* Turn on LED4 */
        BSP_LED_On(LED4);
        /* Turn off LED1 */
        BSP_LED_Off(LED1);
         /* Turn off LED3 */
        BSP_LED_Off(LED3);
      }
      /* if Broadcast message */
      else if ((Tab_Rx[0]&0x0F) == 0x0F) 
      {
        /* Turn off all LEDs */
        BSP_LED_Off(LED1);
        BSP_LED_Off(LED2);
        BSP_LED_Off(LED3);
        BSP_LED_Off(LED4);        
      }
      ReceivedFrame = 0;
    }
    else if (ReceivedFrame == 2) /* means CEC error detected */
    { 
      /* Turn on LED3 */
      BSP_LED_On(LED3);
      ReceivedFrame = 0;
    }
  }
}

/**
  * @brief  Configures the CEC peripheral.
  * @param  None
  * @retval None
  */
static void CEC_Config(CEC_HandleTypeDef *hcec)
{
  /* CEC configuration parameters */    
#if defined (DEVICE_1)
  hcec->Init.OwnAddress          = CEC_OWN_ADDRESS_1;
#elif defined (DEVICE_2)
  hcec->Init.OwnAddress          = CEC_OWN_ADDRESS_3|CEC_OWN_ADDRESS_5;
#endif  

  hcec->Init.SignalFreeTime            = CEC_DEFAULT_SFT;
  hcec->Init.Tolerance                 = CEC_STANDARD_TOLERANCE;
  hcec->Init.BRERxStop                 = CEC_NO_RX_STOP_ON_BRE;
  hcec->Init.BREErrorBitGen            = CEC_BRE_ERRORBIT_NO_GENERATION;
  hcec->Init.LBPEErrorBitGen           = CEC_LBPE_ERRORBIT_NO_GENERATION;
  hcec->Init.BroadcastMsgNoErrorBitGen = CEC_BROADCASTERROR_NO_ERRORBIT_GENERATION;
  hcec->Init.SignalFreeTimeOption      = CEC_SFT_START_ON_TXSOM;  
  hcec->Init.ListenMode                = CEC_REDUCED_LISTENING_MODE;
  hcec->Init.RxBuffer                  = Tab_Rx;
  
  HAL_CEC_Init(hcec);
}



/**
  * @brief Tx Transfer completed callback
  * @param hcec: CEC handle
  * @retval None
  */
void HAL_CEC_TxCpltCallback(CEC_HandleTypeDef *hcec)
{
  TxStatus = 1;
}


/**
  * @brief Rx Transfer completed callback
  * @param hcec: CEC handle
  * @retval None
  */
void HAL_CEC_RxCpltCallback(CEC_HandleTypeDef *hcec, uint32_t RxFrameSize)
{
    ReceivedFrame = 1;
}

/**
  * @brief CEC error callbacks
  * @param hcec: CEC handle
  * @retval None
  */
void HAL_CEC_ErrorCallback(CEC_HandleTypeDef *hcec)
{
  ReceivedFrame = 2;
}

/**
  * @brief  Reset CEC reception buffer
  * @param  None
  * @retval None
  */
static void CEC_FlushRxBuffer(void)
{
  uint32_t cpt;
  
  for (cpt = CEC_MAX_PAYLOAD; cpt > 0; cpt--)
  {
    Tab_Rx[cpt-1] = 0;
  }
}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint32_t ITstatus = 0;
  JOYState_TypeDef JoyState = JOY_NONE;

  if(GPIO_Pin == TAMPER_BUTTON_PIN)
  {
    /* Toggle GREEN LED1 */
    BSP_LED_Toggle(LED1);
    DestinationAddress = MyFollowerAddress1;
    TxSize    = 0x0; /* no payload, ping only */
    StartSending = 1;
  }

  if(GPIO_Pin == MFX_IRQOUT_PIN)  /* Interrupt received from MFX */
  {
    /* The different functionalities of MFX (TS, Joystick, SD detection, etc. )  
    can be configured in exti mode to generate an IRQ on given events.
    The MFX IRQ_OUT pin is unique and common to all functionalities, so if several 
    functionalities are configured in exit mode, the MCU has to enquire MFX about  
    the IRQ source (see BSP_IO_ITGetStatus). Communication with Mfx is done by I2C. 
    Often the sw requires ISRs (irq service routines) to be quick while communication 
    with I2C can be considered relatively long (hundreds of usec depending on I2C clk). 
    Considering that the features for human interaction like TS, Joystick, SD detection 
    don’t need immediate reaction, it is suggested to use POLLING instead of EXTI mode, 
    in order to avoid "blocking I2C communication" on interrupt service routines */
	
    ITstatus = BSP_IO_ITGetStatus(JOY_ALL_PINS);
    if (ITstatus)                 /* Checks if interrupt comes from joystick */
    {
      /* Get the Joystick State */
      JoyState = BSP_JOY_GetState();

      if(JoyState == JOY_UP)
      {
        /* Toggle RED LED3 */
        BSP_LED_Toggle(LED3);
      }
	  
      if(JoyState == JOY_DOWN)
      {
        /* Toggle BLUE LED4 */
        BSP_LED_Toggle(LED4);
#if defined (DEVICE_1)
        DestinationAddress = MyFollowerAddress2;
#elif defined (DEVICE_2)
        DestinationAddress = MyFollowerAddress1;
#endif /* DEVICE_1 */     
        TxSize    = 0x0; /* no payload, ping only */
        StartSending = 1;
      }
	  
      if(JoyState == JOY_SEL)
      {
        /* Toggle ORANGE LED2 */
        BSP_LED_Toggle(LED2);
        DestinationAddress = 0xF;  /* broadcast message indicator */
        TxSize    = 0x0;           /* no payload, ping only */
        StartSending = 1;
      }
    }
    BSP_IO_ITClear();
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
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 2
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
  HAL_StatusTypeDef ret = HAL_OK;

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
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 180 MHz Frequency */  
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    while(1) { ; }
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

/**
  * @}
  */ 
