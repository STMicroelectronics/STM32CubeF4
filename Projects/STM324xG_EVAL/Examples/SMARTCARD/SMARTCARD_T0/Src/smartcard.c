/**
  ******************************************************************************
  * @file    SMARTCARD/SMARTCARD_T0/Src/smartcard.c 
  * @author  MCD Application Team
  * @brief   This file provides all the Smartcard firmware functions.
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

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup SMARTCARD_T0
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "smartcard.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables definition and initialization ----------------------------*/
SC_ATR SC_A2R;
uint8_t SC_ATR_Table[40] = {0};
static __IO uint8_t SCData = 0;
static uint32_t F_Table[16] = {372, 372, 558, 744, 1116, 1488, 1860, 0,
                               0, 512, 768, 1024, 1536, 2048, 0, 0};
static uint32_t D_Table[16] = {0, 1, 2, 4, 8, 16, 32, 0, 12, 20, 0, 0, 0, 0, 0, 0};
SMARTCARD_HandleTypeDef SCHandle;

/* Private function prototypes -----------------------------------------------*/
/* Transport Layer -----------------------------------------------------------*/
/*--------------APDU-----------*/
static void SC_SendData(SC_ADPU_Commands *SC_ADPU, SC_ADPU_Response *SC_ResponseStatus);

/*------------ ATR ------------*/
static void SC_AnswerReq(SC_State *SCState, uint8_t *card, uint8_t length);  /* Ask ATR */
static uint8_t SC_decode_Answer2reset(uint8_t *card);  /* Decode ATR */

/* Physical Port Layer -------------------------------------------------------*/
static void SC_Init(void);
static void SC_DeInit(void);
static void SC_VoltageConfig(uint32_t SC_Voltage);
static uint8_t SC_Detect(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Handles all Smartcard states and serves to send and receive all
  *         communication data between Smartcard and reader.
  * @param  SCState: pointer to an SC_State enumeration that will contain the 
  *         Smartcard state.
  * @param  SC_ADPU: pointer to an SC_ADPU_Commands structure that will be initialized.  
  * @param  SC_Response: pointer to a SC_ADPU_Response structure which will be initialized.
  * @retval None
  */
void SC_Handler(SC_State *SCState, SC_ADPU_Commands *SC_ADPU, SC_ADPU_Response *SC_Response)
{
  uint32_t i = 0;
  switch(*SCState)
  {
  case SC_POWER_ON:
    if (SC_ADPU->Header.INS == SC_GET_A2R)
    {
      /* Smartcard initialization --------------------------------------------*/
      SC_Init();
      
      /* Reset Data from SC buffer -------------------------------------------*/
      for (i = 0; i < 40; i++)
      {
        SC_ATR_Table[i] = 0;
      }
      
      /* Reset SC_A2R Structure ----------------------------------------------*/
      SC_A2R.TS = 0;
      SC_A2R.T0 = 0;
      for (i = 0; i < SETUP_LENGTH; i++)
      {
        SC_A2R.T[i] = 0;
      }
      for (i = 0; i < HIST_LENGTH; i++)
      {
        SC_A2R.H[i] = 0;
      }
      SC_A2R.Tlength = 0;
      SC_A2R.Hlength = 0;
      
      /* Next State ----------------------------------------------------------*/
      *SCState = SC_RESET_LOW;
    }
    break;

  case SC_RESET_LOW:
    if(SC_ADPU->Header.INS == SC_GET_A2R)
    {
      /* If card is detected then Power ON, Card Reset and wait for an answer) */
      if (SC_Detect())
      {
        while(((*SCState) != SC_POWER_OFF) && ((*SCState) != SC_ACTIVE))
        {
          SC_AnswerReq(SCState, (uint8_t *)&SC_ATR_Table, 40); /* Check for answer to reset */
        }
      }
      else
      {
        (*SCState) = SC_POWER_OFF;
      } 
    }
    break;

  case SC_ACTIVE:
    if (SC_ADPU->Header.INS == SC_GET_A2R)
    {
      if(SC_decode_Answer2reset(&SC_ATR_Table[0]) == T0_PROTOCOL)
      {
        (*SCState) = SC_ACTIVE_ON_T0;
      }
      else
      {
        (*SCState) = SC_POWER_OFF; 
      }
    }
    break;

  case SC_ACTIVE_ON_T0:
    SC_SendData(SC_ADPU, SC_Response);
    break;

  case SC_POWER_OFF:
    SC_DeInit(); /* Disable Smartcard interface */
    break;

  default: (*SCState) = SC_POWER_OFF;
  }
}

/**
  * @brief  Enables or disables the power to the Smartcard.
  * @param  NewState: new state of the Smartcard power supply. 
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SC_PowerCmd(FunctionalState NewState)
{
  if(NewState != DISABLE)
  {
    HAL_GPIO_WritePin(SC_CMDVCC_GPIO_PORT, SC_CMDVCC_PIN, GPIO_PIN_RESET);
  }
  else
  {
    HAL_GPIO_WritePin(SC_CMDVCC_GPIO_PORT, SC_CMDVCC_PIN, GPIO_PIN_SET);
  } 
}

/**
  * @brief  Sets or clears the Smartcard reset pin.
  * @param  ResetState: this parameter specifies the state of the Smartcard 
  *         reset pin. BitVal must be one of the GPIO_PinState enum values:
  *                 @arg GPIO_PIN_RESET: to clear the port pin.
  *                 @arg GPIO_PIN_SET: to set the port pin.
  * @retval None
  */
void SC_Reset(GPIO_PinState ResetState)
{
  HAL_GPIO_WritePin(SC_RESET_GPIO_PORT, SC_RESET_PIN, ResetState);
}

/**
  * @brief  Resends the byte that failed to be received (by the Smartcard) correctly.
  * @param  None
  * @retval None
  */
void SC_ParityErrorHandler(void)
{
  HAL_SMARTCARD_Transmit(&SCHandle, (uint8_t *)&SCData, 1, SC_TRANSMIT_TIMEOUT);
}

/**
  * @brief  Configures the IO speed (BaudRate) communication.
  * @param  None
  * @retval None
  */
void SC_PTSConfig(void)
{
  uint32_t workingbaudrate = 0, apbclock = 0;
  uint8_t PPSConfirmStatus = 1;
  uint8_t ptscmd[4] = {0x00};
  uint8_t ptsanswer[4] = {0x00};

  /* Reconfigure the USART Baud Rate -----------------------------------------*/
  apbclock = HAL_RCC_GetPCLK2Freq();
  apbclock /= ((SC_USART->GTPR & (uint16_t)0x00FF) * 2);

  /* Enable the DMA Receive (Set DMAR bit only) to enable interrupt generation
  in case of a framing error FE */  
  __HAL_SMARTCARD_DMA_REQUEST_ENABLE(&SCHandle, SMARTCARD_DMAREQ_RX);

  if((SC_A2R.T0 & (uint8_t)0x10) == 0x10)
  {
    if(SC_A2R.T[0] != 0x11)
    {
      /* PPSS identifies the PPS request or response and is equal to 0xFF */
      ptscmd[0] = 0xFF;
      
      /* PPS0 indicates by the bits b5, b6, b7 equal to 1 the presence of the optional
      bytes PPSI1, PPS2, PPS3 respectively */
      ptscmd[1] = 0x10;
      
      /* PPS1 allows the interface device to propose value of F and D to the card */
      ptscmd[2] = SC_A2R.T[0];
      
      /* PCK check character */
      ptscmd[3] = (uint8_t)0xFF^(uint8_t)0x10^(uint8_t)SC_A2R.T[0];
     
      /* Start the receive IT process: to receive the command answer from the card */
      HAL_SMARTCARD_Receive_IT(&SCHandle, (uint8_t *)ptsanswer, 4);
     
      /* Send command */
      HAL_SMARTCARD_Transmit(&SCHandle, (uint8_t *)ptscmd, 4, SC_TRANSMIT_TIMEOUT);
      
      /* Disable the DMA Receive Request */  
      __HAL_SMARTCARD_DMA_REQUEST_DISABLE(&SCHandle, SMARTCARD_DMAREQ_RX);
      
      /* Wait until receiving the answer from the card */
      while(HAL_SMARTCARD_GetState(&SCHandle) != HAL_SMARTCARD_STATE_READY)
      {}
      
      /* Check the received command answer */
      if((ptsanswer[0] != 0xFF) || (ptsanswer[1] != 0x10) || (ptsanswer[2] != SC_A2R.T[0]) || (ptsanswer[3] != ((uint8_t)0xFF^(uint8_t)0x10^(uint8_t)SC_A2R.T[0])))
      {
        /* PPSS, PPS0, PPS1 and PCK exchange unsuccessful */
        PPSConfirmStatus = 0x00;
      }        
      
      /* PPS exchange successful */
      if(PPSConfirmStatus == 0x01)
      {
        workingbaudrate = apbclock * D_Table[(SC_A2R.T[0] & (uint8_t)0x0F)];
        workingbaudrate /= F_Table[((SC_A2R.T[0] >> 4) & (uint8_t)0x0F)];

        SCHandle.Init.BaudRate = workingbaudrate;
        SCHandle.Init.WordLength = SMARTCARD_WORDLENGTH_9B;
        SCHandle.Init.StopBits = SMARTCARD_STOPBITS_1_5;
        SCHandle.Init.Parity = SMARTCARD_PARITY_EVEN;
        SCHandle.Init.Mode = SMARTCARD_MODE_TX_RX;
        SCHandle.Init.CLKPolarity = SMARTCARD_POLARITY_LOW;
        SCHandle.Init.CLKPhase = SMARTCARD_PHASE_1EDGE;
        SCHandle.Init.CLKLastBit = SMARTCARD_LASTBIT_ENABLE;
        SCHandle.Init.Prescaler = 12;
        SCHandle.Init.GuardTime = 16;
        SCHandle.Init.NACKState = SMARTCARD_NACK_ENABLE;
        SCHandle.Instance = SC_USART;
        
        if(HAL_SMARTCARD_Init(&SCHandle) != HAL_OK)
        {
          while (1);
        }
      }
    }
  }  
}

/**
  * @brief  Manages the Smartcard transport layer: send APDU commands and receives
  *         the APDU response.
  * @param  SC_ADPU: pointer to a SC_ADPU_Commands structure which will be initialized.  
  * @param  SC_ADPU_Response: pointer to a SC_ADPU_Response structure which will be initialized.
  * @retval None
  */
static void SC_SendData(SC_ADPU_Commands *SC_ADPU, SC_ADPU_Response *SC_ResponseStatus)
{
  uint32_t i = 0;

  uint8_t command[5] = {0x00};
  uint8_t answer[40] = {0x00};

  /* Reset response buffer ---------------------------------------------------*/
  for(i = 0; i < LC_MAX; i++)
  {
    SC_ResponseStatus->Data[i] = 0;
  }
  
  SC_ResponseStatus->SW1 = 0;
  SC_ResponseStatus->SW2 = 0;

  /* Enable the DMA Receive (Set DMAR bit only) to enable interrupt generation
     in case of a framing error FE */  
  __HAL_SMARTCARD_DMA_REQUEST_ENABLE(&SCHandle, SMARTCARD_DMAREQ_RX);
  
  /* Send header -------------------------------------------------------------*/
  command[0] = SC_ADPU->Header.CLA;
  command[1] = SC_ADPU->Header.INS;
  command[2] = SC_ADPU->Header.P1;
  command[3] = SC_ADPU->Header.P2;
  
  /* Send body length to/from SC ---------------------------------------------*/
  if(SC_ADPU->Body.LC)
  {
    command[4] = SC_ADPU->Body.LC;
    HAL_SMARTCARD_Transmit(&SCHandle, (uint8_t *)command, 5, SC_TRANSMIT_TIMEOUT);
  }
  else if(SC_ADPU->Body.LE)
  { 
    command[4] = SC_ADPU->Body.LE;
    HAL_SMARTCARD_Transmit(&SCHandle, (uint8_t *)command, 5, SC_TRANSMIT_TIMEOUT);
  }

  /* Flush the SC_USART DR */
  __HAL_SMARTCARD_FLUSH_DRREGISTER(&SCHandle);

  /* Start the receive IT process: to receive the command answer from the card */
  HAL_SMARTCARD_Receive_IT(&SCHandle, (uint8_t *)&answer[0], 1);
  
  /* Wait until receiving the answer from the card */
  while(HAL_SMARTCARD_GetState(&SCHandle) != HAL_SMARTCARD_STATE_READY)
  {}
  
  /* --------------------------------------------------------
    Wait Procedure byte from card:
    1 - ACK
    2 - NULL
    3 - SW1; SW2
  -------------------------------------------------------- */
  if(((answer[0] & (uint8_t)0xF0) == 0x60) || ((answer[0] & (uint8_t)0xF0) == 0x90))
  {
    /* SW1 received */
    SC_ResponseStatus->SW1 = answer[0];
    
    if(HAL_SMARTCARD_Receive_IT(&SCHandle, (uint8_t *)&answer[1], 1) == HAL_OK)
    {
      /* SW2 received */
      SC_ResponseStatus->SW2 = answer[1];
    }
  }
  else if(((answer[0] & (uint8_t)0xFE) == (((uint8_t)~(SC_ADPU->Header.INS)) & (uint8_t)0xFE)) || \
    ((answer[0] & (uint8_t)0xFE) == (SC_ADPU->Header.INS & (uint8_t)0xFE)))
  {
    SC_ResponseStatus->Data[0] = answer[0];/* ACK received */
  }
  
  /* If no status bytes received ---------------------------------------------*/
  if(SC_ResponseStatus->SW1 == 0x00)
  {
    /* Send body data to SC --------------------------------------------------*/
    if(SC_ADPU->Body.LC)
    {
      /* Send body data */
      HAL_SMARTCARD_Transmit(&SCHandle, (uint8_t *)&SC_ADPU->Body.Data[0], SC_ADPU->Body.LC, SC_TRANSMIT_TIMEOUT);
      
      /* Flush the SC_USART DR */
      __HAL_SMARTCARD_FLUSH_DRREGISTER(&SCHandle);
      
      /* Disable the DMA Receive (Reset DMAR bit only) */  
      __HAL_SMARTCARD_DMA_REQUEST_DISABLE(&SCHandle, SMARTCARD_DMAREQ_RX);
      
      /* Start the receive IT process: to receive the command answer from the card */
      HAL_SMARTCARD_Receive_IT(&SCHandle, (uint8_t *)&answer[0], 2);
      
      /* Wait until receiving the answer from the card */
      while(HAL_SMARTCARD_GetState(&SCHandle) != HAL_SMARTCARD_STATE_READY)
      {}
      
      /* Decode the SW1 */
      SC_ResponseStatus->SW1 = answer[0];
      
      /* Decode the SW2 */
      SC_ResponseStatus->SW2 = answer[1];
    }
    
    /* Or receive body data from SC ------------------------------------------*/
    else if(SC_ADPU->Body.LE)
    {
      /* Start the receive IT process: to receive the command answer from the card */
      HAL_SMARTCARD_Receive_IT(&SCHandle, (uint8_t *)&answer[0], (SC_ADPU->Body.LE +2));
      
      /* Wait until receiving the answer from the card */
      while(HAL_SMARTCARD_GetState(&SCHandle) != HAL_SMARTCARD_STATE_READY)
      {}
      
      /* Decode the body data */
      for(i = 0; i < SC_ADPU->Body.LE; i++)
      {
        SC_ResponseStatus->Data[i] = answer[i];
      }
      
      /* Decode the SW1 */
      SC_ResponseStatus->SW1 = answer[SC_ADPU->Body.LE];
      
      /* Decode the SW2 */
      SC_ResponseStatus->SW2 = answer[SC_ADPU->Body.LE +1];
    }
  }
}

/**
  * @brief  Requests the reset answer from card.
  * @param  SCState: pointer to an SC_State enumeration that will contain the Smartcard state.
  * @param  card: pointer to a buffer which will contain the card ATR.
  * @param  length: maximum ATR length
  * @retval None
  */
static void SC_AnswerReq(SC_State *SCState, uint8_t *card, uint8_t length)
{
  switch(*SCState)
  {
  case SC_RESET_LOW:
    /* Check response with reset low -----------------------------------------*/
    HAL_SMARTCARD_Receive(&SCHandle, card, length, SC_RECEIVE_TIMEOUT);

    if(card[0] != 0x00)
    {
      (*SCState) = SC_ACTIVE;
      SC_Reset(GPIO_PIN_SET);
    }
    else
    {
      (*SCState) = SC_RESET_HIGH;
    }
    break;

  case SC_RESET_HIGH:
    /* Check response with reset high ----------------------------------------*/
    SC_Reset(GPIO_PIN_SET); /* Reset High */
    HAL_SMARTCARD_Receive(&SCHandle, card, length, SC_RECEIVE_TIMEOUT);

    if(card[0])
    {
      (*SCState) = SC_ACTIVE;
    }
    else
    {
      (*SCState) = SC_POWER_OFF;
    }
    break;

  case SC_ACTIVE:
    break;

  case SC_POWER_OFF:
    /* Close Connection if no answer received --------------------------------*/
    SC_Reset(GPIO_PIN_SET); /* Reset high - a bit is used as level shifter from 3.3 to 5 V */
    SC_PowerCmd(DISABLE);
    break;

  default:
    (*SCState) = SC_RESET_LOW;
    break;
  }
}

/**
  * @brief  Decodes the Answer to reset received from card.
  * @param  card: pointer to the buffer containing the card ATR.
  * @retval Smartcard communication Protocol 
  */
static uint8_t SC_decode_Answer2reset(uint8_t *card)
{
 uint32_t i = 0, flag = 0, buf = 0, protocol = 0;

  SC_A2R.TS = card[0];  /* Initial character */
  SC_A2R.T0 = card[1];  /* Format character */

  SC_A2R.Hlength = SC_A2R.T0 & (uint8_t)0x0F;

  if ((SC_A2R.T0 & (uint8_t)0x80) == 0x80)
  {
    flag = 1;
  }

  for (i = 0; i < 4; i++)
  {
    SC_A2R.Tlength = SC_A2R.Tlength + (((SC_A2R.T0 & (uint8_t)0xF0) >> (4 + i)) & (uint8_t)0x1);
  }

  for (i = 0; i < SC_A2R.Tlength; i++)
  {
    SC_A2R.T[i] = card[i + 2];
  }
  
  if ((SC_A2R.T0 & (uint8_t)0x80) == 0x00)
  {
    protocol = 0;
  }
  else
  {
    protocol = SC_A2R.T[SC_A2R.Tlength - 1] & (uint8_t)0x0F;
  }
  
  while (flag)
  {
    if ((SC_A2R.T[SC_A2R.Tlength - 1] & (uint8_t)0x80) == 0x80)
    {
      flag = 1;
    }
    else
    {
      flag = 0;
    }

    buf = SC_A2R.Tlength;
    SC_A2R.Tlength = 0;

    for (i = 0; i < 4; i++)
    {
      SC_A2R.Tlength = SC_A2R.Tlength + (((SC_A2R.T[buf - 1] & (uint8_t)0xF0) >> (4 + i)) & (uint8_t)0x1);
    }

    for (i = 0;i < SC_A2R.Tlength; i++)
    {
      SC_A2R.T[buf + i] = card[i + 2 + buf];
    }
    SC_A2R.Tlength += (uint8_t)buf;
  }

  for (i = 0; i < SC_A2R.Hlength; i++)
  {
    SC_A2R.H[i] = card[i + 2 + SC_A2R.Tlength];
  }

  return (uint8_t)protocol;
}

/**
  * @brief  Initializes all peripheral used for Smartcard interface.
  * @param  None
  * @retval None
  */
static void SC_Init(void)
{
  /* SC_USART configuration --------------------------------------------------*/
  /* SC_USART configured as follows:
  - Word Length = 9 Bits
  - 1.5 Stop Bit
  - Even parity
  - BaudRate = 8064 baud
  - Hardware flow control disabled (RTS and CTS signals)
  - Tx and Rx enabled
  - USART Clock enabled
  */
  /* USART Clock set to 3,5 MHz (PCLK2 (84 MHz) / 24) */
  SCHandle.Instance = SC_USART;
  SCHandle.Init.BaudRate = 9408;  /* Starting baudrate = 3,5MHz / 372etu */
  SCHandle.Init.WordLength = SMARTCARD_WORDLENGTH_9B;
  SCHandle.Init.StopBits = SMARTCARD_STOPBITS_1_5;
  SCHandle.Init.Parity = SMARTCARD_PARITY_EVEN;
  SCHandle.Init.Mode = SMARTCARD_MODE_TX_RX;
  SCHandle.Init.CLKPolarity = SMARTCARD_POLARITY_LOW;
  SCHandle.Init.CLKPhase = SMARTCARD_PHASE_1EDGE;
  SCHandle.Init.CLKLastBit = SMARTCARD_LASTBIT_ENABLE;  
  SCHandle.Init.Prescaler = 12;
  SCHandle.Init.GuardTime = 16;
  SCHandle.Init.NACKState = SMARTCARD_NACK_ENABLE; 
  HAL_SMARTCARD_Init(&SCHandle);

  /* Set RSTIN HIGH */  
  SC_Reset(GPIO_PIN_SET);
}

/**
  * @brief  Deinitializes all resources used by the Smartcard interface.
  * @param  None
  * @retval None
  */
static void SC_DeInit(void)
{
  /* Disable CMDVCC */
  SC_PowerCmd(DISABLE);

  /* Deinitializes the SCHandle */
  HAL_SMARTCARD_DeInit(&SCHandle);
}

/**
  * @brief  Configures the card power voltage.
  * @param  SC_Voltage: specifies the card power voltage.
  *         This parameter can be one of the following values:
  *              @arg SC_VOLTAGE_5V: 5V cards.
  *              @arg SC_VOLTAGE_3V: 3V cards.
  * @retval None
  */
static void SC_VoltageConfig(uint32_t SC_Voltage)
{
  if(SC_Voltage == SC_VOLTAGE_5V)
  {
    /* Select Smartcard 5V */  
    HAL_GPIO_WritePin(SC_3_5V_GPIO_PORT, SC_3_5V_PIN, GPIO_PIN_SET);
  }
  else
  {
    /* Select Smartcard 3V */
    HAL_GPIO_WritePin(SC_3_5V_GPIO_PORT, SC_3_5V_PIN, GPIO_PIN_RESET);
  } 
}

/**
  * @brief  Configures GPIO hardware resources used for Samrtcard.
  * @param  None
  * @retval None
  */
void SC_IOConfig(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable GPIO clocks */
  SC_3_5V_CLK_ENABLE();
  SC_RESET_CLK_ENABLE();
  SC_CMDVCC_CLK_ENABLE();
  SC_OFF_CLK_ENABLE();
  
  /* Configure Smartcard CMDVCC pin */
  GPIO_InitStruct.Pin = SC_CMDVCC_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(SC_CMDVCC_GPIO_PORT, &GPIO_InitStruct);

  /* Configure Smartcard Reset pin */
  GPIO_InitStruct.Pin = SC_RESET_PIN;
  HAL_GPIO_Init(SC_RESET_GPIO_PORT, &GPIO_InitStruct);

  /* Configure Smartcard 3/5V pin */
  GPIO_InitStruct.Pin = SC_3_5V_PIN;
  HAL_GPIO_Init(SC_3_5V_GPIO_PORT, &GPIO_InitStruct);

  /* Select 5V */ 
  SC_VoltageConfig(SC_VOLTAGE_5V);
  
  /* Disable CMDVCC */
  SC_PowerCmd(DISABLE);

  /* Set RSTIN HIGH */  
  SC_Reset(GPIO_PIN_SET);

  /* Configure Smartcard OFF pin */
  GPIO_InitStruct.Pin = SC_OFF_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SC_OFF_GPIO_PORT, &GPIO_InitStruct);

  /* Configure the NVIC for Smartcard OFF pin */ 
  HAL_NVIC_SetPriority(SC_OFF_EXTI_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SC_OFF_EXTI_IRQn);
}

/**
  * @brief  Detects whether the Smartcard is present or not.
  * @param  None. 
  * @retval 0 - Smartcard inserted
  *         1 - Smartcard not inserted
  */
static uint8_t SC_Detect(void)
{
  return HAL_GPIO_ReadPin(SC_OFF_GPIO_PORT, SC_OFF_PIN);
}

/**
  * @}
  */

/**
  * @}
  */
