/**
  ******************************************************************************
  * @file    USB_Host/MTP_Standalone/Src/menu.c 
  * @author  MCD Application Team
  * @brief   This file implements Menu Functions
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
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
MTP_DEMO_StateMachine       mtp_demo;
uint8_t                     prev_select = 0;
MTP_DEMO_SelectMode         mtp_select_mode;
AUDIO_PLAYBACK_StateTypeDef audio_state;

uint8_t *MTP_main_menu[] = 
{
  (uint8_t *)"      1 - Explore audio file                                         ",
  (uint8_t *)"      2 - Start audio Player                                         ",
  (uint8_t *)"      3 - Re-Enumerate                                               ",
};

/* Private function prototypes -----------------------------------------------*/
static void MTP_SelectItem(uint8_t **menu, uint8_t item);
static void MTP_MenuProbeKey(JOYState_TypeDef state);
static void MTP_ChangeSelectMode(MTP_DEMO_SelectMode select_mode);
static void LCD_ClearTextZone(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Demo state machine.
  * @param  None
  * @retval None
  */
void MTP_MenuInit(void)
{
  mtp_demo.state = MTP_DEMO_IDLE;
  mtp_select_mode = MTP_SELECT_MENU;
  MTP_SelectItem(MTP_main_menu, 0); 
}

/**
  * @brief  Manages MTP Menu Process.
  * @param  None
  * @retval None
  */
void MTP_MenuProcess(void)
{
  switch(mtp_demo.state)
  {
  case MTP_DEMO_IDLE:
    MTP_SelectItem(MTP_main_menu, 0); 
    mtp_demo.state = MTP_DEMO_WAIT;
    mtp_demo.select = 0;
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAtLine(15, (uint8_t *)"Use [Joystick Left/Right] to scroll up/down");
    BSP_LCD_DisplayStringAtLine(16, (uint8_t *)"Use [Joystick Up/Down] to scroll MTP menu");
    break;    
    
  case MTP_DEMO_WAIT:
    
    if(mtp_demo.select != prev_select)
    {
      prev_select = mtp_demo.select;
      MTP_SelectItem(MTP_main_menu, mtp_demo.select & 0x7F);
      
      /* Handle select item */
      if(mtp_demo.select & 0x80)
      {
        
        switch(mtp_demo.select & 0x7F)
        {
        case 0:
          mtp_demo.state = MTP_DEMO_EXPLORE;  
          break;
        
        case 1:         
          /* Display HMI messages */
          BSP_LCD_SetTextColor(LCD_COLOR_GREEN);          
          BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"                                             ");
          BSP_LCD_DisplayStringAtLine(15, (uint8_t *)"                                             ");          
          BSP_LCD_DisplayStringAtLine(16, (uint8_t *)"Use [User Key] To Stop and return from player");
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
          
          /* Set PLAYBACK state and start playing 1st file */ 
          audio_state = AUDIO_STATE_IDLE;
          mtp_demo.state = MTP_DEMO_PLAYBACK;
          MTP_ChangeSelectMode(MTP_PLAYBACK_CONTROL);           
          break;
        
        case 2:
          mtp_demo.state = MTP_REENUMERATE;  
          break;
        
        default:
          break;
        }
      }
    }
    break;
    
  case MTP_DEMO_EXPLORE:  
    if(Appli_state == APPLICATION_READY)
    {
      if(MTP_Init() == 0)
      {
        if(MTP_ExploreWavFile() > 0)
        {
          LCD_ErrLog("There is no WAV file on the microSD.\n" );         
          MTP_ChangeSelectMode(MTP_SELECT_MENU); 
          mtp_demo.state = MTP_DEMO_IDLE;
        }
        else
        {
          mtp_demo.state = MTP_DEMO_WAIT;
        }
      }
    }
    else
    {
      mtp_demo.state = MTP_DEMO_WAIT;
    }
    break; 
    
  case MTP_DEMO_PLAYBACK:
    if(Appli_state == APPLICATION_READY)
    {
      if(MTP_Init() == 0)
      {
        if(audio_state == AUDIO_STATE_IDLE)
        {
          /* Start Playing */
          audio_state = AUDIO_STATE_INIT;
          if(AUDIO_Start(0) == AUDIO_ERROR_IO)
          {
            MTP_ChangeSelectMode(MTP_SELECT_MENU); 
             mtp_demo.state = MTP_DEMO_IDLE;
          }
          else
          {
            BSP_LCD_SetTextColor(LCD_COLOR_YELLOW); 
            BSP_LCD_DisplayStringAtLine(10, (uint8_t *)"[  UP   ] : Volume +");
            BSP_LCD_DisplayStringAtLine(11, (uint8_t *)"[ DOWN  ] : Volume -");
            BSP_LCD_DisplayStringAtLine(12, (uint8_t *)"[ LEFT  ] : Previous");
            BSP_LCD_DisplayStringAtLine(13, (uint8_t *)"[ RIGHT ] : Next");
            BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"[  SEL  ] : Pause/Resume");          
            BSP_LCD_SetTextColor(LCD_COLOR_WHITE);           
          }
        }
        else /* Not idle */
        {
          if(AUDIO_Process() == AUDIO_ERROR_IO)
          {
            MTP_ChangeSelectMode(MTP_SELECT_MENU);  
            mtp_demo.state = MTP_DEMO_IDLE;
          }
        }
      }
    }
    else
    {
      mtp_demo.state = MTP_DEMO_WAIT;
    }
    break; 
    
  case MTP_REENUMERATE:
    /* Force MTP Device to re-enumerate */
    USBH_ReEnumerate(&hUSBHost); 
    mtp_demo.state = MTP_DEMO_WAIT;
    break;

  default:
    break;
  }

  if(Appli_state == APPLICATION_DISCONNECT)
  {
    Appli_state = APPLICATION_IDLE;     
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
    MTP_ChangeSelectMode(MTP_SELECT_MENU);     
  }
   
  mtp_demo.select &= 0x7F;
} 

/**
  * @brief  Manages the menu on the screen.
  * @param  menu: Menu table
  * @param  item: Selected item to be highlighted
  * @retval None
  */
static void MTP_SelectItem(uint8_t **menu, uint8_t item)
{
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  
  switch(item)
  {
  case 0: 
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
    BSP_LCD_DisplayStringAtLine(17, menu [0]);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);    
    BSP_LCD_DisplayStringAtLine(18, menu [1]);
    BSP_LCD_DisplayStringAtLine(19, menu [2]);
    break;
    
  case 1: 
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(17, menu [0]);
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);    
    BSP_LCD_DisplayStringAtLine(18, menu [1]);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);  
    BSP_LCD_DisplayStringAtLine(19, menu [2]); 
    break;
    
  case 2: 
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(17, menu [0]);  
    BSP_LCD_DisplayStringAtLine(18, menu [1]);
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);  
    BSP_LCD_DisplayStringAtLine(19, menu [2]); 
    break;
     
  default:
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(17, menu [0]);
    BSP_LCD_DisplayStringAtLine(18, menu [1]);
    BSP_LCD_DisplayStringAtLine(19, menu [2]); 
    break; 
  }
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK); 
}

/**
  * @brief  Probes the MTP joystick state.
  * @param  state: Joystick state
  * @retval None
  */
static void MTP_MenuProbeKey(JOYState_TypeDef state)
{
  /* Handle Menu inputs */
  if((state == JOY_UP) && (mtp_demo.select > 0))
  {
    mtp_demo.select--;
  }
  else if((state == JOY_DOWN) && (mtp_demo.select < 2))
  {
    mtp_demo.select++;
  }
  else if(state == JOY_SEL)
  {
    if(USBH_MTP_IsReady(&hUSBHost) > 0)
    {    
      mtp_demo.select |= 0x80;
    }
  }  
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static JOYState_TypeDef JoyState = JOY_NONE;
  static uint32_t debounce_time = 0;
  
  if(GPIO_Pin == GPIO_PIN_2)
  {    
    /* Get the Joystick State */
    JoyState = BSP_JOY_GetState();
    
    /* Clear joystick interrupt pending bits */
    BSP_IO_ITClear(JOY_ALL_PINS);
    
    if(mtp_select_mode == MTP_SELECT_MENU)
    {  
      MTP_MenuProbeKey(JoyState); 
      
      switch(JoyState)
      {
      case JOY_LEFT:
        LCD_LOG_ScrollBack();
        break;
             
      case JOY_RIGHT:
        LCD_LOG_ScrollForward();
        break;          
        
      default:
        break;           
      }
    }
    else if(mtp_select_mode == MTP_PLAYBACK_CONTROL)
    {
      AUDIO_PlaybackProbeKey(JoyState);
    }
  }

  if(mtp_demo.state == MTP_DEMO_PLAYBACK)
  {
    if(GPIO_Pin == KEY_BUTTON_PIN)
    { 
      /* Prevent debounce effect for user key */
      if((HAL_GetTick() - debounce_time) > 50)
      {
        debounce_time = HAL_GetTick();
      }
      else
      {
        return;
      }
      
      /* Change the selection type */
      if(mtp_select_mode == MTP_SELECT_MENU)
      {
        MTP_ChangeSelectMode(MTP_PLAYBACK_CONTROL); 
      }
      else if(mtp_select_mode == MTP_PLAYBACK_CONTROL)
      {       
       AUDIO_Stop();
      }
    }
  }
}

/**
  * @brief  Changes the selection mode.
  * @param  select_mode: Selection mode
  * @retval None
  */
static void MTP_ChangeSelectMode(MTP_DEMO_SelectMode select_mode)
{
  if(select_mode == MTP_SELECT_MENU)
  {
    MTP_SelectItem(MTP_main_menu, 0x00);
    LCD_ClearTextZone();
    LCD_LOG_UpdateDisplay(); 
  }
  else if(select_mode == MTP_PLAYBACK_CONTROL)
  {
    LCD_ClearTextZone();
    MTP_SelectItem(MTP_main_menu, 0xFF);     
  }
  mtp_select_mode = select_mode; 
  mtp_demo.select = 0;
}

/**
  * @brief  Clears the text zone.
  * @param  None
  * @retval None
  */
static void LCD_ClearTextZone(void)
{
  uint8_t i = 0;

  for(i= 0; i < 13; i++)
  {
    BSP_LCD_ClearStringLine(i + 3);
  }
}
