/**
  ******************************************************************************
  * @file    SMARTCARD/SMARTCARD_T0/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F4xx SMARTCARD HAL API to communicate
  *          with card compatible with T0 protocol. 
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

/** @addtogroup SMARTCARD_T0
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Directories & Files ID */
const uint8_t MasterRoot[2] = {0x3F, 0x00};
const uint8_t GSMDir[2] = {0x7F, 0x20};
const uint8_t ICCID[2] = {0x2F, 0xE2};
const uint8_t IMSI[2] = {0x6F, 0x07};
const uint8_t CHV1[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};

/* APDU Transport Structures */
SC_ADPU_Commands SC_ADPU;
SC_ADPU_Response SC_Response;

__IO uint32_t CardInserted = 0;
uint32_t CHV1Status = 0, i = 0;
__IO uint8_t ICCID_Content[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
__IO uint8_t IMSI_Content[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* Private function prototypes -----------------------------------------------*/
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

  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();
  
  SC_State SCState = SC_POWER_OFF;     

  /* Configure Smartcard Interface GPIO pins */
  SC_IOConfig();
	  
/*-------------------------------- Idle task ---------------------------------*/
  while(1)
  {
    /* Loop while no Smartcard is detected */  
    while(CardInserted == 0)
    {
    }
  
    /* Start SC Demo ---------------------------------------------------------*/
    
    /* Wait A2R --------------------------------------------------------------*/
    SCState = SC_POWER_ON;

    SC_ADPU.Header.CLA = 0x00;
    SC_ADPU.Header.INS = SC_GET_A2R;
    SC_ADPU.Header.P1 = 0x00;
    SC_ADPU.Header.P2 = 0x00;
    SC_ADPU.Body.LC = 0x00;
   
    while(SCState != SC_ACTIVE_ON_T0) 
    {
      SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    }

    /* Apply the Procedure Type Selection (PTS) */
    SC_PTSConfig();

    /* Inserts delay(400ms) for Smartard clock resynchronisation */
    HAL_Delay(400);

    /* Select MF -------------------------------------------------------------*/
    SC_ADPU.Header.CLA = SC_CLA_GSM11;
    SC_ADPU.Header.INS = SC_SELECT_FILE;
    SC_ADPU.Header.P1 = 0x00;
    SC_ADPU.Header.P2 = 0x00;
    SC_ADPU.Body.LC = 0x02;

    for(i = 0; i < SC_ADPU.Body.LC; i++)
    {
      SC_ADPU.Body.Data[i] = MasterRoot[i];
    }
    while(i < LC_MAX) 
    {    
      SC_ADPU.Body.Data[i++] = 0;
    }
    SC_ADPU.Body.LE = 0;

    SC_Handler(&SCState, &SC_ADPU, &SC_Response);

    /* Get Response on MF ----------------------------------------------------*/
    if(SC_Response.SW1 == SC_DF_SELECTED)
    {
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_GET_RESPONCE;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x00;
      SC_ADPU.Body.LE = SC_Response.SW2;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    }

    /* Select ICCID ----------------------------------------------------------*/
    if(((SC_Response.SW1 << 8) | (SC_Response.SW2)) == SC_OP_TERMINATED)
    {
      /* Check if the CHV1 is enabled */   
      if((SC_Response.Data[13] & 0x80) == 0x00)
      {
        CHV1Status = 0x01;
      }
      /* Send APDU Command for ICCID selection */
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_SELECT_FILE;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x02;

      for(i = 0; i < SC_ADPU.Body.LC; i++)
      {
        SC_ADPU.Body.Data[i] = ICCID[i];
      }
      while(i < LC_MAX) 
      {    
        SC_ADPU.Body.Data[i++] = 0;
      }
      SC_ADPU.Body.LE = 0;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    }

    /* Read Binary in ICCID --------------------------------------------------*/
    if(SC_Response.SW1 == SC_EF_SELECTED)
    {
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_READ_BINARY;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x00;

      SC_ADPU.Body.LE = 10;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    }

    /* Select GSMDir ---------------------------------------------------------*/
    if(((SC_Response.SW1 << 8) | (SC_Response.SW2)) == SC_OP_TERMINATED)
    {
      /* Copy the ICCID File content into ICCID_Content buffer */
      for(i = 0; i < SC_ADPU.Body.LE; i++)
      {
        ICCID_Content[i] =  SC_Response.Data[i];
      }
      /* Send APDU Command for GSMDir selection */ 
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_SELECT_FILE;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x02;

      for(i = 0; i < SC_ADPU.Body.LC; i++)
      {
        SC_ADPU.Body.Data[i] = GSMDir[i];
      }
      while(i < LC_MAX) 
      {    
        SC_ADPU.Body.Data[i++] = 0;
      }
      SC_ADPU.Body.LE = 0;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    }

    /* Select IMSI -----------------------------------------------------------*/
    if(SC_Response.SW1 == SC_DF_SELECTED)
    {
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_SELECT_FILE;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x02;

      for(i = 0; i < SC_ADPU.Body.LC; i++)
      {
        SC_ADPU.Body.Data[i] = IMSI[i];
      }
      while(i < LC_MAX) 
      {    
        SC_ADPU.Body.Data[i++] = 0;
      }
      SC_ADPU.Body.LE = 0;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    }

    /* Get Response on IMSI File ---------------------------------------------*/
    if(SC_Response.SW1 == SC_EF_SELECTED)
    {
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_GET_RESPONCE;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x00;
      SC_ADPU.Body.LE = SC_Response.SW2;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    }

    /* Read Binary in IMSI ---------------------------------------------------*/
    if(CHV1Status == 0x00)
    {
      if(((SC_Response.SW1 << 8) | (SC_Response.SW2)) == SC_OP_TERMINATED)
      {
        /* Enable CHV1 (PIN1) ------------------------------------------------*/
        SC_ADPU.Header.CLA = SC_CLA_GSM11;
        SC_ADPU.Header.INS = SC_ENABLE;
        SC_ADPU.Header.P1 = 0x00;
        SC_ADPU.Header.P2 = 0x01;
        SC_ADPU.Body.LC = 0x08;
 
        for(i = 0; i < SC_ADPU.Body.LC; i++)
        {
          SC_ADPU.Body.Data[i] = CHV1[i];
        }
        while(i < LC_MAX) 
        {    
          SC_ADPU.Body.Data[i++] = 0;
        }
        SC_ADPU.Body.LE = 0;

        SC_Handler(&SCState, &SC_ADPU, &SC_Response);
      }
    }
    else
    {
      if(((SC_Response.SW1 << 8) | (SC_Response.SW2)) == SC_OP_TERMINATED)
      {
        /* Verify CHV1 (PIN1) ------------------------------------------------*/
        SC_ADPU.Header.CLA = SC_CLA_GSM11;
        SC_ADPU.Header.INS = SC_VERIFY;
        SC_ADPU.Header.P1 = 0x00;
        SC_ADPU.Header.P2 = 0x01;
        SC_ADPU.Body.LC = 0x08;

        for(i = 0; i < SC_ADPU.Body.LC; i++)
        {
          SC_ADPU.Body.Data[i] = CHV1[i];
        }
        while(i < LC_MAX) 
        {    
          SC_ADPU.Body.Data[i++] = 0;
        }
        SC_ADPU.Body.LE = 0;

        SC_Handler(&SCState, &SC_ADPU, &SC_Response);
      }
    }
    /* Read Binary in IMSI ---------------------------------------------------*/
    if(((SC_Response.SW1 << 8) | (SC_Response.SW2)) == SC_OP_TERMINATED)
    {
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_READ_BINARY;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x00;

      SC_ADPU.Body.LE = 9;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    }
    if(((SC_Response.SW1 << 8) | (SC_Response.SW2)) == SC_OP_TERMINATED)
    {
      /* Copy the IMSI File content into IMSI_Content buffer */
      for(i = 0; i < SC_ADPU.Body.LE; i++)
      {
        IMSI_Content[i] =  SC_Response.Data[i];
      }
    }
    /* Disable the Smartcard interface */
    SCState = SC_POWER_OFF;
    SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    CardInserted = 0;
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

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  /* STM32F405x/407x/415x/417x Revision Z and upper devices: prefetch is supported  */
  if (HAL_GetREVID() >= 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
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
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == SC_OFF_PIN)
  {
    /* Toggle LED1..4 */
    BSP_LED_Toggle(LED1);
    BSP_LED_Toggle(LED2);
    BSP_LED_Toggle(LED3);
    BSP_LED_Toggle(LED4);
    
    /* Smartcard detected */
    CardInserted = 1;
    
    /* Enable CMDVCC */
    SC_PowerCmd(ENABLE);
    
    /* Reset the card */
    SC_Reset(GPIO_PIN_RESET);
  }
}

/**
  * @brief SMARTCARD error callbacks
  * @param hsc: SMARTCARD handle
  * @retval None
  */
void HAL_SMARTCARD_ErrorCallback(SMARTCARD_HandleTypeDef *hsc)
{
  if(HAL_SMARTCARD_GetError(hsc) & HAL_SMARTCARD_ERROR_FE)
  {
    __HAL_SMARTCARD_FLUSH_DRREGISTER(hsc);
    /* Resend the byte that failed to be received (by the Smartcard) correctly */
    SC_ParityErrorHandler();
  }
  
  if(HAL_SMARTCARD_GetError(hsc) & HAL_SMARTCARD_ERROR_PE)
  {
    /* Enable SC_USART RXNE Interrupt (until receiving the corrupted byte) */
    __HAL_SMARTCARD_ENABLE_IT(hsc, SMARTCARD_IT_RXNE);
    /* Flush the SC_USART DR register */
    __HAL_SMARTCARD_FLUSH_DRREGISTER(hsc);
  }
  
  if(HAL_SMARTCARD_GetError(hsc) & HAL_SMARTCARD_ERROR_NE)
  {
    __HAL_SMARTCARD_FLUSH_DRREGISTER(hsc);
  }
  
  if(HAL_SMARTCARD_GetError(hsc) & HAL_SMARTCARD_ERROR_ORE)
  {
    __HAL_SMARTCARD_FLUSH_DRREGISTER(hsc);
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
