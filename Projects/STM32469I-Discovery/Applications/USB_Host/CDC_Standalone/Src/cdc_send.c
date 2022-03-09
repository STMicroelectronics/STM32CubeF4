/**
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/Src/cdc_send.c 
  * @author  MCD Application Team
  * @brief   CDC Send state machine
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

/* Private define ------------------------------------------------------------*/
/* The size of the buffer depends on the user's needs */
#define TX_BUFF_SIZE  (64 * 1024)

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t CDC_TX_Buffer[TX_BUFF_SIZE];
__IO uint8_t CDC_SendState = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Handles CDC Send Menu. 
  * @param  None
  * @retval None
*/
void CDC_Handle_Send_Menu(void)
{
  switch(CdcDemo.Send_state)
  {
  case CDC_SEND_IDLE:
    CdcDemo.Send_state = CDC_SEND_WAIT;
    CDC_SendState = 0;
    USBH_CDC_Stop(&hUSBHost);
    memset(CDC_TX_Buffer, 0x5A, TX_BUFF_SIZE);
    LCD_LOG_ClearTextZone();
    LCD_DbgLog("Sending data ...\n"); 
    USBH_CDC_Transmit(&hUSBHost, CDC_TX_Buffer, TX_BUFF_SIZE);
    break;
  
  case CDC_SEND_WAIT:
    if(CDC_SendState == 1) /* Data sent */
    {
      BSP_LCD_ClearStringLine(19);
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_DisplayStringAtLine(19, (uint8_t *)"Press User button to start receiving data ");
      
      /* Wait for User Input */
      while((BSP_PB_GetState(BUTTON_WAKEUP) != SET) && (Appli_state != APPLICATION_DISCONNECT))
      {
      }
      HAL_Delay(400);
      
      CdcDemo.state = CDC_DEMO_RECEIVE;
      CdcDemo.Send_state = CDC_SEND_IDLE;
    }
    break;
    
  default:
    break;
  }
}

/**
  * @brief  CDC data transmit callback.
  * @param  phost: Host handle
  * @retval None
  */
void USBH_CDC_TransmitCallback(USBH_HandleTypeDef *phost)
{
  LCD_DbgLog (">> Data sent\n" );
  CDC_SendState = 1;
}
