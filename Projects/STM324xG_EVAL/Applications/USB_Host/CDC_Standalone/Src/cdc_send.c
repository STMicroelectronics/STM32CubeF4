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
uint8_t *DEMO_SEND_menu[] = 
{
  (uint8_t *)"      1 - Send Dummy Data                                                   ",
  (uint8_t *)"      2 - Send File                                                         ",
  (uint8_t *)"      3 - Return                                                            ",
  (uint8_t *)"                                                                            ",
};

uint8_t BLANK_LINE [] = "                                                                             ";

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FIL MyFile = {0};
uint8_t FilePos = 0;
uint8_t FileOffset = 0;
uint8_t PrevPos = 0;
uint8_t CDC_TX_Buffer[TX_BUFF_SIZE];
uint32_t EnableSendData = 0;
uint32_t use_file = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void CDC_SendFile(uint8_t fileidx);
static void ReturnFromSendMenu(void);
static void CDC_ShowFiles(uint8_t offset, uint8_t select);
static void SendFileMenu_Init(void);

/**
  * @brief  Handles CDC Send Menu. 
  * @param  None
  * @retval None
  */
void CDC_Handle_Send_Menu(void)
{ 
  if(CdcSelectMode == CDC_SELECT_MENU)
  {
    switch(CdcDemo.Send_state)
    {
    case CDC_SEND_IDLE:
      CdcDemo.Send_state = CDC_SEND_WAIT;
      CDC_SelectItem (DEMO_SEND_menu, 0);
      CdcDemo.select = 0; 

      PrevPos = 0;
      USBH_CDC_Stop(&hUSBHost);
      break;
      
    case CDC_SEND_WAIT:
      if(CdcDemo.select != PrevSelect)
      {
        PrevSelect = CdcDemo.select;
        CDC_SelectItem (DEMO_SEND_menu, CdcDemo.select & 0x7F);
        
        /* Handle select item */
        if(CdcDemo.select & 0x80)
        {
          switch(CdcDemo.select & 0x7F)
          {
          case 0:
            memset(CDC_TX_Buffer, 0x5A, TX_BUFF_SIZE);
            LCD_DbgLog("Sending data ...\n"); 
            USBH_CDC_Transmit(&hUSBHost, CDC_TX_Buffer, TX_BUFF_SIZE);
            use_file = 0;
            break;
            
          case 1:
            if(FileList.ptr > 0)
            {
              SendFileMenu_Init();
              if(FilePos >= 9)
              {
                CDC_ShowFiles(FileOffset, 9);
              }
              
              else
              {
                CDC_ShowFiles(FileOffset, FilePos);
              }
              CdcDemo.Send_state = CDC_SEND_SELECT_FILE;
            }
            else
            {
             LCD_ErrLog("No file on the microSD\n"); 
            }
            break;
            
          case 2: /* Return */
            ReturnFromSendMenu();
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
  }
  else if(CdcSelectMode == CDC_SELECT_FILE)
  {
    switch(CdcDemo.Send_state)
    {
    case  CDC_SEND_SELECT_FILE: 
      if(CdcDemo.select & 0x80)
      {
        CdcDemo.select &= 0x7F;
        CdcDemo.Send_state = CDC_SEND_TRANSMIT_FILE;        
      }
      break;
      
    case CDC_SEND_TRANSMIT_FILE: 
      LCD_DbgLog("Sending file ...\n"); 
      use_file = 1;
      CDC_SendFile(FilePos);
      CDC_ChangeSelectMode(CDC_SELECT_MENU);
      LCD_LOG_UpdateDisplay();
      CdcDemo.Send_state = CDC_SEND_WAIT;
      break;
      
    default:
      break;      
    }
    
    if(FilePos != PrevPos)
    {
      
      if(((FilePos > 9)&&(FilePos > PrevPos)) ||
         ((FilePos >= 9)&&(FilePos < PrevPos)))
      {
        if(FilePos > PrevPos)
        {
          FileOffset++;
        }
        else
        {
          FileOffset--;
        }
        CDC_ShowFiles(FileOffset, 9);        
      }
      else
      {
        CDC_ShowFiles(0, FilePos);
      }
      PrevPos = FilePos;      
    }
  }
  CdcDemo.select &= 0x7F;
}

/**
  * @brief  Probes the CDC joystick state.
  * @param  state: Joystick state
  * @retval None
  */
void CDC_SendFile_ProbeKey(JOYState_TypeDef state)
{
  /* Handle File List Selection */ 
  if((state == JOY_UP) && (FilePos > 0))
  {
    FilePos --;
  }
  else if ((state == JOY_DOWN)&& (FilePos < (FileList.ptr - 1)))
  {
    FilePos ++;
  }
  else if (state == JOY_SEL)
  {
    CdcDemo.select |= 0x80;
  }    
}

/**
  * @brief  Demo_Application 
  *         Demo state machine
  * @param  None
  * @retval None
  */

static void SendFileMenu_Init(void)
{
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
  BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"                                           ");
  BSP_LCD_DisplayStringAtLine(15, (uint8_t *)"[User Key] to switch menu                  ");
  BSP_LCD_DisplayStringAtLine(16, (uint8_t *)"[Joystick Up/Down] to change settings items");
  CDC_ChangeSelectMode(CDC_SELECT_FILE);
}

/**
  * @brief  Returns from Send Menu 
  * @param  None
  * @retval None
  */
static void ReturnFromSendMenu(void)
{
  CdcDemo.state = CDC_DEMO_IDLE;
  CdcDemo.select = 0; 
  
  /* Restore main menu */
  LCD_ClearTextZone();
  LCD_LOG_UpdateDisplay();
  Menu_Init();  
}

/**
  * @brief  Sends file to the root.
  * @param  fileidx: File index
  * @retval None
  */
static void CDC_SendFile(uint8_t fileidx)
{
  uint32_t bytesread;
  
  if(FileList.ptr > 0)
  {
    /* Close any opened file */
    f_close(&MyFile);
    
    if(f_open(&MyFile, (char *)FileList.file[fileidx].name, FA_OPEN_EXISTING | FA_READ) == FR_OK) 
    {
      /* Fill the buffer to Send */
      f_read(&MyFile, CDC_TX_Buffer, TX_BUFF_SIZE, (void *)&bytesread);
      
      /* Send File */
      USBH_CDC_Transmit(&hUSBHost, CDC_TX_Buffer, bytesread);
    }
    else
    {
      LCD_ErrLog("Cannot open %s file\n", FileList.file[0].name);
    }
  }
  else
  {
    LCD_ErrLog("No file on the microSD\n");
  }
}

/**
  * @brief  Shows files on the root.
  * @param  None
  * @retval None
  */
static void CDC_ShowFiles(uint8_t offset, uint8_t select)
{
  uint8_t i = 0;
  
  if(offset < FileList.ptr)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    for (i = 4; i < 14; i++)
    {
      BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
      BSP_LCD_DisplayStringAtLine(i, BLANK_LINE);
      
      if((i - 4) < FileList.ptr - offset)
      {
        if(i == (select + 4))
        {
          BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
        }
        BSP_LCD_DisplayStringAtLine(i, FileList.file[i-4 + offset].name);    
      }
    }
  }
}

/**
  * @brief  CDC data transmit callback.
  * @param  phost: Host handle
  * @retval None
  */
void USBH_CDC_TransmitCallback(USBH_HandleTypeDef *phost)
{
   uint32_t bytesread;
  
  if(use_file == 1)
  {
if( MyFile.fptr >= f_size(& MyFile))
    {
      f_close(&MyFile);
      LCD_DbgLog (">> File sent\n" );
      use_file = 0;
    }
    else
    {
      /* Fill the buffer to Send */
      f_read(&MyFile, CDC_TX_Buffer, TX_BUFF_SIZE, (void *)&bytesread);
      /* Send File */
      USBH_CDC_Transmit(&hUSBHost, CDC_TX_Buffer, bytesread);
    }
  }
  else
  {
    LCD_DbgLog (">> Data sent\n" );
  }
}
