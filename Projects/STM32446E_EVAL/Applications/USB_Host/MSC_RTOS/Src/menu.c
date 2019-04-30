/**
  ******************************************************************************
  * @file    USB_Host/MSC_RTOS/Src/menu.c 
  * @author  MCD Application Team
  * @brief   This file implements Menu Functions
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MENU_UPDATE_EVENT            0x10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
MSC_DEMO_StateMachine msc_demo;
osSemaphoreId MenuEvent;
osSemaphoreId osJoySemaphore;

uint8_t *MSC_main_menu[] = 
{
  (uint8_t *)"      1 - File Operations                                                   ",
  (uint8_t *)"      2 - Explorer Disk                                                     ",
  (uint8_t *)"      3 - Re-Enumerate                                                      ",
};

/* Private function prototypes -----------------------------------------------*/
static void MSC_SelectItem(uint8_t **menu, uint8_t item);
static void MSC_DEMO_ProbeKey(JOYState_TypeDef state);
void Joystick_MscMenu(void);
static void MSC_MenuThread(void const *argument);

/* Private functions ---------------------------------------------------------*/
 
/**
  * @brief  Demo state machine.
  * @param  None
  * @retval None
  */
void Menu_Init(void)
{
  /* Create Menu Semaphore */
  osSemaphoreDef(osSem);

  MenuEvent = osSemaphoreCreate(osSemaphore(osSem), 1); 
  
  /* Force menu to show Item 0 by default */
  osSemaphoreRelease(MenuEvent);

  /* Menu task */
  osThreadDef(Menu_Thread, MSC_MenuThread, osPriorityHigh, 0, 8 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Menu_Thread), NULL);
  
  /* Define used semaphore fot Joystick*/
  osSemaphoreDef(JOY_SEM);
  
  /* Create the semaphore used by the two threads. */
  osJoySemaphore = osSemaphoreCreate(osSemaphore(JOY_SEM) , 1);
  
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_DisplayStringAtLine(15, (uint8_t *)"Use [Joystick Left/Right] to scroll up/down");
  BSP_LCD_DisplayStringAtLine(16, (uint8_t *)"Use [Joystick Up/Down] to scroll MSC menu");  
}

/**
  * @brief  User task
  * @param  pvParameters not used
  * @retval None
  */
static void MSC_MenuThread(void const *argument)
{
  uint32_t ITstatus = 0;

  for(;;)
  {
    if(osSemaphoreWait(MenuEvent, 100) == osOK)
    {
      switch(msc_demo.state)
      {
      case MSC_DEMO_IDLE:
        MSC_SelectItem(MSC_main_menu, 0); 
        msc_demo.state = MSC_DEMO_WAIT;
        msc_demo.select = 0;
        osSemaphoreRelease(MenuEvent);
        break;    
        
      case MSC_DEMO_WAIT:
        MSC_SelectItem(MSC_main_menu, msc_demo.select & 0x7F);
        
        /* Handle select item */
        if(msc_demo.select & 0x80)
        {
          switch(msc_demo.select & 0x7F)
          {
          case 0:
            msc_demo.state = MSC_DEMO_FILE_OPERATIONS; 
            osSemaphoreRelease(MenuEvent);
            break;
            
          case 1:
            msc_demo.state = MSC_DEMO_EXPLORER;  
            osSemaphoreRelease(MenuEvent);
            break;
            
          case 2:
            msc_demo.state = MSC_REENUMERATE; 
            osSemaphoreRelease(MenuEvent);
            break;
            
          default:
            break;
          }
        }
        break;
        
      case MSC_DEMO_FILE_OPERATIONS:  
        /* Read and Write File Here */
        if(Appli_state == APPLICATION_READY)
        {
          MSC_File_Operations();
        }
        msc_demo.state = MSC_DEMO_WAIT;
        break; 
        
      case MSC_DEMO_EXPLORER:
        /* Display disk content */
        if(Appli_state == APPLICATION_READY)
        {        
          Explore_Disk("0:/", 1);
        }
        msc_demo.state = MSC_DEMO_WAIT;
        break; 
        
      case MSC_REENUMERATE:
        /* Force MSC Device to re-enumerate */
        USBH_ReEnumerate(&hUSBHost); 
        msc_demo.state = MSC_DEMO_WAIT;
        break;
        
      default:
        break;
      } 
      msc_demo.select &= 0x7F;
    }

    if (osJoySemaphore != NULL)
    {      
      if(osSemaphoreWait(osJoySemaphore , 0) == osOK)
      {
        ITstatus = BSP_IO_ITGetStatus(JOY_ALL_PINS);
        if (ITstatus)
        {
          Joystick_MscMenu();
          /* Clear joystick interrupt pending bits */
          BSP_IO_ITClear();
        }
      }
    }
  }
}

/**
  * @brief  Joystick Msc menu
  * @param  None
  * @retval None
  */
void Joystick_MscMenu(void)
{
  static JOYState_TypeDef JoyState = JOY_NONE;
  
    /* Get the Joystick State */
    JoyState = BSP_JOY_GetState();
    
    MSC_DEMO_ProbeKey(JoyState); 
    
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

    osSemaphoreRelease(MenuEvent);
}


/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == MFX_IRQOUT_PIN)
  {
    /* The different functionalities of MFX (TS, Joystick, SD detection, etc. )  
    can be configured in exti mode to generate an IRQ on given events.
    The MFX IRQ_OUT pin is unique and common to all functionalities, so if several 
    functionalities are configured in exit mode, the MCU has to enquire MFX about  
    the IRQ source (see BSP_IO_ITGetStatus). Communication with Mfx is done by I2C. 
    Often the sw requires ISRs (irq service routines) to be quick while communication 
    with I2C can be considered relatively long (hundreds of usec depending on I2C clk). 
    In order to avoid "blocking I2C communication" on interrupt service routines 
	it is suggested to use a semaphore */
    osSemaphoreRelease(osJoySemaphore);
  }
}

/**
  * @brief  Manages the menu on the screen.
  * @param  menu: Menu table
  * @param  item: Selected item to be highlighted
  * @retval None
  */
static void MSC_SelectItem(uint8_t **menu, uint8_t item)
{
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  
  switch(item)
  {
  case 0: 
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
    BSP_LCD_DisplayStringAtLine(17, menu [0]);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);    
    BSP_LCD_DisplayStringAtLine(18 , menu [1]);
    BSP_LCD_DisplayStringAtLine(19 , menu [2]);
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
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);    
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
  * @brief  Probes the MSC joystick state.
  * @param  state: Joystick state
  * @retval None
  */
static void MSC_DEMO_ProbeKey(JOYState_TypeDef state)
{
  msc_demo.select &= 0x7F;

  /* Handle Menu inputs */
  if((state == JOY_UP) && (msc_demo.select > 0))
  {
    msc_demo.select--;
  }
  else if((state == JOY_DOWN) && (msc_demo.select < 2))
  {
    msc_demo.select++;
  }
  else if(state == JOY_SEL)
  {
    msc_demo.select |= 0x80;
  }  
}

      

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
