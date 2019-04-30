/**
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/Src/cdc_receive.c 
  * @author  MCD Application Team
  * @brief   CDC Receive state machine
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private define ------------------------------------------------------------ */
#define RX_BUFF_SIZE   0x400    /* Max Received data 1KB */

/* Private typedef ----------------------------------------------------------- */
uint8_t *DEMO_RECEIVE_menu[] = {
  (uint8_t *)
    "      1 - Start receiving data                                              ",
  (uint8_t *)
    "      2 - Return                                                            ",
  (uint8_t *)
    "                                                                            ",
};

/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
uint16_t xPos, yLinePos;

/* Private function prototypes ----------------------------------------------- */
uint8_t CDC_RX_Buffer[RX_BUFF_SIZE];

/* Private functions --------------------------------------------------------- */
static void DumpReceivedData(void);
static void ReturnFromReceiveMenu(void);

/**
  * @brief  Handles CDC Receive Menu. 
  * @param  None
  * @retval None
  */
void CDC_Handle_Receive_Menu(void)
{
  switch (CdcDemo.Receive_state)
  {
  case CDC_RECEIVE_IDLE:
    CdcDemo.Receive_state = CDC_RECEIVE_WAIT;
    CDC_SelectItem(DEMO_RECEIVE_menu, 0);
    CdcDemo.select = 0;
    USBH_CDC_Stop(&hUSBHost);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAtLine(14,
                                (uint8_t *)
                                "                                          ");
    BSP_LCD_DisplayStringAtLine(15,
                                (uint8_t *)
                                "Use [Buttons Left/Right] to scroll up/down");
    break;

  case CDC_RECEIVE_WAIT:
    if (CdcDemo.select != PrevSelect)
    {
      PrevSelect = CdcDemo.select;
      CDC_SelectItem(DEMO_RECEIVE_menu, CdcDemo.select & 0x7F);
      /* Handle select item */
      if (CdcDemo.select & 0x80)
      {
        switch (CdcDemo.select & 0x7F)
        {
        case 0:
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          /* Start Reception */
          LCD_ClearTextZone();
          BSP_LCD_DisplayStringAtLine(3, (uint8_t *) "Receiving data ...");
          xPos = 0;
          yLinePos = 4;
          memset(CDC_RX_Buffer, 0, RX_BUFF_SIZE);
          USBH_CDC_Receive(&hUSBHost, CDC_RX_Buffer, RX_BUFF_SIZE);
          CdcDemo.Receive_state = CDC_RECEIVE_WAIT;
          break;

        case 1:
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          USBH_CDC_Stop(&hUSBHost);
          ReturnFromReceiveMenu();
          break;

        default:
          break;
        }
      }
    }
    break;

  default:
    break;
  }
  CdcDemo.select &= 0x7F;
}

/**
  * @brief  Returns from Receive Menu 
  * @param  None
  * @retval None
  */
static void ReturnFromReceiveMenu(void)
{
  CdcDemo.state = CDC_DEMO_IDLE;
  CdcDemo.select = 0;

  /* Restore main menu */
  LCD_ClearTextZone();
  LCD_LOG_UpdateDisplay();
  Menu_Init();
}

/**
  * @brief  CDC data receive callback.
  * @param  phost: Host handle
  * @retval None
  */
void USBH_CDC_ReceiveCallback(USBH_HandleTypeDef * phost)
{
  DumpReceivedData();
  USBH_CDC_Receive(&hUSBHost, CDC_RX_Buffer, RX_BUFF_SIZE);
}

/**
  * @brief  Displays recieved data
  * @param  data: Keyboard data to be displayed
  * @retval None
  */
static void DumpReceivedData(void)
{
  uint16_t size;
  uint8_t *ptr = CDC_RX_Buffer;

  size = USBH_CDC_GetLastReceivedDataSize(&hUSBHost);
  BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
  while (size--)
  {
    if ((*ptr != '\n') && (*ptr != '\r'))
    {
      if (*ptr == '\t')
      {
        BSP_LCD_DisplayChar(xPos, LINE(yLinePos), ' ');
      }
      else
      {
        BSP_LCD_DisplayChar(xPos, LINE(yLinePos), *ptr);
      }
      xPos += 7;
    }
    else if (*ptr == '\n')
    {
      yLinePos++;
      xPos = 0;
    }

    ptr++;

    if (xPos > (BSP_LCD_GetXSize() - 7))
    {
      xPos = 0;
      yLinePos++;
    }

    if (yLinePos > 13)
    {
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_DisplayStringAtLine(15,
                                  (uint8_t *)
                                  "Use [User Key] to see more data");
      /* Key Button in polling */
      while (BSP_PB_GetState(BUTTON_KEY) != RESET)
      {
        /* Wait for User Input */
      }

      LCD_ClearTextZone();
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAtLine(3, (uint8_t *) "Receiving data ...");
      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
      xPos = 0;
      yLinePos = 4;
    }
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
