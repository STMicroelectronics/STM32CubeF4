/**
  ******************************************************************************
  * @file    font_app.c
  * @author  MCD Application Team
  * @brief   This file provides main GUI application program functions
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
#include <stddef.h>
#include "main.h"
#include "GUI.h"
#include "WM.h"
#include "IMAGE.h"
#include "DIALOG.h"
#include "BUTTON.h"

/* Logo resources */
#include "info.c"
#include "STLogo.c"
#include "french.c"
#include "italian.c"
#include "chinese.c"
#include "arabic.c"
#include "english.c"
#include "world.c"

/* Fonts */
#include "chinese25B.c"
#include "chinese100R.c"
#include "chineseI60B.c"

#include "eng25.c"
#include "eng60.c"
#include "eng100.c"

#include "arabic25.c"
#include "arabic60.c"
#include "arabic100.c"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ID_WINDOW_0             (GUI_ID_USER + 0x00)
#define ID_WINDOW_1             (GUI_ID_USER + 0x10)
#define ID_TEXT_1               (GUI_ID_USER + 0x01)
#define ID_TEXT_2               (GUI_ID_USER + 0x02)
#define ID_TEXT_3               (GUI_ID_USER + 0x03)
#define ID_TEXT_4               (GUI_ID_USER + 0x04)

#define GUI_ID_IMAGE_FRENCH     (GUI_ID_USER + 0x05)
#define GUI_ID_IMAGE_ITALIAN    (GUI_ID_USER + 0x06)
#define GUI_ID_IMAGE_CHINESE    (GUI_ID_USER + 0x07)
#define GUI_ID_IMAGE_ENGLISH    (GUI_ID_USER + 0x08)  
#define GUI_ID_IMAGE_ARABIC     (GUI_ID_USER + 0x09)

#define GUI_ID_IMAGE_WORLD      (GUI_ID_USER + 0x0A)

#define GUI_ID_TEXT_ENGLISH1    (GUI_ID_USER + 0x10)
#define GUI_ID_TEXT_ARABIC1     (GUI_ID_USER + 0x11)
#define GUI_ID_TEXT_ARABIC2     (GUI_ID_USER + 0x12)
#define GUI_ID_TEXT_ARABIC3     (GUI_ID_USER + 0x13)
#define GUI_ID_TEXT_FRENCH1     (GUI_ID_USER + 0x14)
#define GUI_ID_TEXT_FRENCH2     (GUI_ID_USER + 0x15)
#define GUI_ID_TEXT_CHINESE1    (GUI_ID_USER + 0x16)
#define GUI_ID_TEXT_CHINESE2    (GUI_ID_USER + 0x17)
#define GUI_ID_TEXT_CHINESE3    (GUI_ID_USER + 0x18)
#define GUI_ID_TEXT_ITALIAN1    (GUI_ID_USER + 0x19)
#define GUI_ID_TEXT_ITALIAN2    (GUI_ID_USER + 0x1A)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef struct {
	int xSize, ySize;
	int xPart, yPart;
	int xPos, yPos;
	int Dir;
	int Size;
	int ObjectSize;
} ANIM_DATA;

WM_HWIN  hinfo, hfont, happli, hlanguage;

static const GUI_WIDGET_CREATE_INFO _aDialog[] =
{
	{ WINDOW_CreateIndirect, "", ID_WINDOW_0, 0, 0, 800, 630, 0, 0x64, 0 },
};

static const char * _apStrings[] = {
	"\xe6\xac\xa2\xe8\xbf\x8e",
	"\xd8\xa3\xd9\x87\xd9\x84\xd8\xa7",
};

int step = 0;

uint8_t toggle = 0;
uint8_t toggle_previous = 1;
uint8_t toggle_flesh = 0;
uint8_t toggle_arabic = 0;
uint8_t toggle_chinese = 0;
uint8_t toggle_french = 0;
uint8_t toggle_italian = 0;
uint8_t toggle_english = 0;

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Callback routine of the font dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog_font(WM_MESSAGE * pMsg) {
  WM_HWIN hItem, hItem1, hItem2, hItem3, hItem4;
  int Id, NCode;
  static int toggle_drapeau = 0;
  switch (pMsg->MsgId) {
    
  case WM_CREATE:
    
    hItem = IMAGE_CreateEx(750, 10, 40, 70, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_IMAGE_ARABIC);
    IMAGE_SetBitmap(hItem, &bmarabic);
    hItem = IMAGE_CreateEx(750, 10, 40, 70, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_IMAGE_ITALIAN);
    IMAGE_SetBitmap(hItem, &bmitalian);
    hItem = IMAGE_CreateEx(750, 10, 40, 70, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_IMAGE_CHINESE);
    IMAGE_SetBitmap(hItem, &bmchinese);
    hItem = IMAGE_CreateEx(750, 10, 40, 70, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_IMAGE_FRENCH);
    IMAGE_SetBitmap(hItem, &bmfrench);
    hItem = IMAGE_CreateEx(750, 10, 40, 70, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_IMAGE_ENGLISH);
    IMAGE_SetBitmap(hItem, &bmenglish);
    
    hItem = IMAGE_CreateEx(745, 5, 55, 70, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_IMAGE_WORLD);
    IMAGE_SetGIF(hItem, _acworld, sizeof(_acworld));
    
    GUI_UC_SetEncodeUTF8();
    
    /* English */
    hItem = TEXT_CreateEx(260, 190, 380, 90, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_TEXT_ENGLISH1, "welcome");
    TEXT_SetTextColor(hItem, 0xFFE51602);
    TEXT_SetFont(hItem, &GUI_Fonteng100);
    
    /* French */
    hItem = TEXT_CreateEx(270, 290, 260, 60, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_TEXT_FRENCH1, "bienvenue");
    TEXT_SetTextColor(hItem, 0x6B6704);
    TEXT_SetFont(hItem, &GUI_Fonteng60);
    
    hItem = TEXT_CreateEx(500, 120, 115, 25, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_TEXT_FRENCH2, "bienvenue");
    TEXT_SetTextColor(hItem, 0xF89D2D);
    TEXT_SetFont(hItem, &GUI_Fonteng25);
    
    /* Chinese */
    hItem = TEXT_CreateEx(95, 250, 160, 90, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_TEXT_CHINESE1, _apStrings[0]);
    TEXT_SetTextColor(hItem, 0xF94508);
    TEXT_SetFont(hItem, &GUI_Fontchinese100R);
    
    hItem = TEXT_CreateEx(320, 100, 100, 60, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_TEXT_CHINESE2, _apStrings[0]);
    TEXT_SetTextColor(hItem, 0x08CFF9);
    TEXT_SetFont(hItem, &GUI_FontchineseI60B);
    
    hItem = TEXT_CreateEx(600, 160, 40, 25, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_TEXT_CHINESE3, _apStrings[0]);
    TEXT_SetTextColor(hItem, 0x08CFF9);
    
    TEXT_SetFont(hItem, &GUI_Fontchinese25B);
    
    /* Italian */
    hItem = TEXT_CreateEx(320, 150, 270, 60, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_TEXT_ITALIAN1, "benvenuto");
    TEXT_SetTextColor(hItem, 0x8ABC0B);
    TEXT_SetFont(hItem, &GUI_Fonteng60);
    
    hItem = TEXT_CreateEx(200, 200, 115, 25, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_TEXT_ITALIAN2, "benvenuto");
    TEXT_SetTextColor(hItem, 0x0000FF);
    TEXT_SetFont(hItem, &GUI_Fonteng25);
    
    /* ARABIC */
    /* Enable bidirectional functionality */
    GUI_UC_EnableBIDI(1);
    
    hItem = TEXT_CreateEx(200, 105, 200, 90, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_TEXT_ARABIC1, _apStrings[1]);
    TEXT_SetTextColor(hItem, 0x0000FF);
    TEXT_SetFont(hItem, &GUI_Fontarabic100);
    
    hItem = TEXT_CreateEx(560, 280, 150, 60, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_TEXT_ARABIC2, _apStrings[1]);
    TEXT_SetTextColor(hItem, 0x2B3C00);
    TEXT_SetFont(hItem, &GUI_Fontarabic60);
    
    hItem = TEXT_CreateEx(450, 140, 100, 25, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_TEXT_ARABIC3, _apStrings[1]);
    TEXT_SetTextColor(hItem, 0xA82573);
    TEXT_SetFont(hItem, &GUI_Fontarabic25);
    
    break;
    
  case WM_PAINT:
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    
    /* English */
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ENGLISH1);
    if ((!toggle_arabic) && (!toggle_italian) && (!toggle_french) && (!toggle_chinese))
    {
      TEXT_SetTextColor(hItem, 0xFFE51602);
    }
    else
    {
      TEXT_SetTextColor(hItem, GUI_GRAY_AA);
    }
    
    /* Italian */
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ITALIAN1);
    if ((!toggle_arabic) && (!toggle_french) && (!toggle_chinese) && (!toggle_english))
    {
      TEXT_SetTextColor(hItem, 0x8ABC0B);
    }
    else
    {
      TEXT_SetTextColor(hItem, GUI_GRAY_AA);
    }
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ITALIAN2);
    if ((!toggle_arabic) && (!toggle_french) && (!toggle_chinese) && (!toggle_english))
    {
      TEXT_SetTextColor(hItem, 0x0000FF);
    }
    else
    {
      TEXT_SetTextColor(hItem, GUI_GRAY_AA);
    }
    
    /* French */
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_FRENCH1);
    if ((!toggle_arabic) && (!toggle_italian) && (!toggle_chinese) && (!toggle_english))
    {
      TEXT_SetTextColor(hItem, 0x6B6704);
    }
    else
    {
      TEXT_SetTextColor(hItem, GUI_GRAY_AA);
    }
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_FRENCH2);
    if ((!toggle_arabic) && (!toggle_italian) && (!toggle_chinese) && (!toggle_english))
    {
      TEXT_SetTextColor(hItem, 0xF89D2D);
    }
    else
    {
      TEXT_SetTextColor(hItem, GUI_GRAY_AA);
    }
    
    /* Arabic */
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC1);
    if ((!toggle_italian) && (!toggle_french) && (!toggle_chinese) && (!toggle_english))
    {
      TEXT_SetTextColor(hItem, 0x1868FA);
    }
    else
    {
      TEXT_SetTextColor(hItem, GUI_GRAY_AA);
    }
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC2);
    if ((!toggle_italian) && (!toggle_french) && (!toggle_chinese) && (!toggle_english))
    {
      TEXT_SetTextColor(hItem, 0xFA6D18);
    }
    else
    {
      TEXT_SetTextColor(hItem, GUI_GRAY_AA);
    }
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC3);
    if ((!toggle_italian) && (!toggle_french) && (!toggle_chinese) && (!toggle_english))
    {
      TEXT_SetTextColor(hItem, 0x18FA93);
    }
    else
    {
      TEXT_SetTextColor(hItem, GUI_GRAY_AA);
    }
    
    /* CHINESE */
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE1);
    if ((!toggle_arabic) && (!toggle_italian) && (!toggle_french) && (!toggle_english))
    {
      TEXT_SetTextColor(hItem, 0xF94508);
    }
    else
    {
      TEXT_SetTextColor(hItem, GUI_GRAY_AA);
    }
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE2);
    if ((!toggle_arabic) && (!toggle_italian) && (!toggle_french) && (!toggle_english))
    {
      TEXT_SetTextColor(hItem, 0x08CFF9);
    }
    else
    {
      TEXT_SetTextColor(hItem, GUI_GRAY_AA);
    }
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE3);
    if ((!toggle_arabic) && (!toggle_italian) && (!toggle_french) && (!toggle_english))
    {
      TEXT_SetTextColor(hItem, 0x08CFF9);
    }
    else
    {
      TEXT_SetTextColor(hItem, GUI_GRAY_AA);
    }
    
    break;
  case WM_NOTIFY_PARENT:
    Id = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    
    switch (Id) {
      
      /* Notification sent by "Button_Close" */
    case GUI_ID_IMAGE_WORLD:
      if (NCode == WM_NOTIFICATION_RELEASED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_IMAGE_ENGLISH);
        hItem1 = WM_GetDialogItem(pMsg->hWin, GUI_ID_IMAGE_FRENCH);
        hItem2 = WM_GetDialogItem(pMsg->hWin, GUI_ID_IMAGE_CHINESE);
        hItem3 = WM_GetDialogItem(pMsg->hWin, GUI_ID_IMAGE_ITALIAN);
        hItem4 = WM_GetDialogItem(pMsg->hWin, GUI_ID_IMAGE_ARABIC);
        
        if (!toggle_drapeau)
        {
          for (step = 1; step < 11; step++)
          {
            if (step < 3)
            {
              WM_MoveTo(hItem, (750 - (step * 25)), 10);
              WM_MoveTo(hItem1, (750 - (step * 25)), 10);
              WM_MoveTo(hItem2, (750 - (step * 25)), 10);
              WM_MoveTo(hItem3, (750 - (step * 25)), 10);
              WM_MoveTo(hItem4, (750 - (step * 25)), 10);
              GUI_Exec();
            }
            else if (step < 5)
            {
              WM_MoveTo(hItem1, (750 - (step * 25)), 10);
              WM_MoveTo(hItem2, (750 - (step * 25)), 10);
              WM_MoveTo(hItem3, (750 - (step * 25)), 10);
              WM_MoveTo(hItem4, (750 - (step * 25)), 10);
              GUI_Exec();
            }
            else if (step < 7)
            {
              WM_MoveTo(hItem2, (750 - (step * 25)), 10);
              WM_MoveTo(hItem3, (750 - (step * 25)), 10);
              WM_MoveTo(hItem4, (750 - (step * 25)), 10);
              GUI_Exec();
            }
            else if(step < 9)
            {
              WM_MoveTo(hItem3, (750 - (step * 25)), 10);
              WM_MoveTo(hItem4, (750 - (step * 25)), 10);
              GUI_Exec();
            }
            else
            {
              WM_MoveTo(hItem4, (750 - (step * 25)), 10);
              GUI_Exec();
            }
          }
        }
        else
        {
          for (step = 1; step < 10; step++)
          {
            if (step < 3)
            {
              WM_MoveTo(hItem4, (525 + (step * 25)), 10);
              GUI_Exec();
            }
            else if (step < 5)
            {
              WM_MoveTo(hItem4, (525 + (step * 25)), 10);
              WM_MoveTo(hItem3, (525 + (step * 25)), 10);
              GUI_Exec();
            }
            else if (step < 7)
            {
              WM_MoveTo(hItem4, (525 + (step * 25)), 10);
              WM_MoveTo(hItem3, (525 + (step * 25)), 10);
              WM_MoveTo(hItem2, (525 + (step * 25)), 10);
              GUI_Exec();
            }
            else if (step < 9)
            {
              WM_MoveTo(hItem4, (525 + (step * 25)), 10);
              WM_MoveTo(hItem3, (525 + (step * 25)), 10);
              WM_MoveTo(hItem2, (525 + (step * 25)), 10);
              WM_MoveTo(hItem1, (525 + (step * 25)), 10);
              GUI_Exec();
            }
            else
            {
              WM_MoveTo(hItem4, (525 + (step * 25)), 10);
              WM_MoveTo(hItem3, (525 + (step * 25)), 10);
              WM_MoveTo(hItem2, (525 + (step * 25)), 10);
              WM_MoveTo(hItem1, (525 + (step * 25)), 10);
              WM_MoveTo(hItem, (525 + (step * 25)), 10);
              GUI_Exec();
            }
          }
        }
        toggle_drapeau ^= 1;
        toggle_arabic = 0;
        toggle_chinese = 0;
        toggle_french = 0;
        toggle_italian = 0;
        toggle_english = 0;
        WM_Invalidate(pMsg->hWin);
      }
      break;
    case GUI_ID_IMAGE_ARABIC:
      if (NCode == WM_NOTIFICATION_RELEASED)
      {
        
        toggle_arabic ^= 1;
        toggle_chinese = 0;
        toggle_italian = 0;
        toggle_french = 0;
        toggle_english = 0;
        WM_InvalidateWindow(happli);
        WM_InvalidateWindow(hfont);
        WM_Update(hfont);
        GUI_Exec();
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ENGLISH1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_FRENCH1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_FRENCH2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ITALIAN1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ITALIAN2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
      }
      break;
    case GUI_ID_IMAGE_ENGLISH:
      if (NCode == WM_NOTIFICATION_RELEASED)
      {
        toggle_english ^= 1;
        toggle_arabic = 0;
        toggle_chinese = 0;
        toggle_italian = 0;
        toggle_french = 0;
        
        WM_InvalidateWindow(happli);
        WM_InvalidateWindow(hfont);
        WM_Update(hfont);
        GUI_Exec();
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ENGLISH1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_FRENCH1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_FRENCH2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ITALIAN1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ITALIAN2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
      }
      break;
      
    case GUI_ID_IMAGE_FRENCH:
      if (NCode == WM_NOTIFICATION_RELEASED)
      {
        toggle_french ^= 1;
        toggle_arabic = 0;
        toggle_italian = 0;
        toggle_chinese = 0;
        toggle_english = 0;
        WM_InvalidateWindow(happli);
        WM_InvalidateWindow(hfont);
        WM_Update(hfont);
        GUI_Exec();
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ENGLISH1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_FRENCH1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_FRENCH2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ITALIAN1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ITALIAN2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
      }
      break;
    case GUI_ID_IMAGE_ITALIAN:
      if (NCode == WM_NOTIFICATION_RELEASED)
      {
        toggle_italian ^= 1;
        toggle_arabic = 0;
        toggle_chinese = 0;
        toggle_french = 0;
        toggle_english = 0;
        WM_InvalidateWindow(happli);
        WM_InvalidateWindow(hfont);
        WM_Update(hfont);
        GUI_Exec();
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ENGLISH1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_FRENCH1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_FRENCH2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ITALIAN1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ITALIAN2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
      }
      break;
    case GUI_ID_IMAGE_CHINESE:
      if (NCode == WM_NOTIFICATION_RELEASED)
      {
        toggle_chinese ^= 1;
        toggle_arabic = 0;
        toggle_italian = 0;
        toggle_french = 0;
        toggle_english = 0;
        WM_InvalidateWindow(happli);
        WM_InvalidateWindow(hfont);
        WM_Update(hfont);
        GUI_Exec();
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ENGLISH1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_FRENCH1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_FRENCH2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ITALIAN1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ITALIAN2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_CHINESE3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT_ARABIC3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Callback routine of the main dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {

  const GUI_PID_STATE * pState;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    break;
    
  case WM_PAINT:
    GUI_SetBkColor(0xFF040D44);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetPenSize(5);
    if (toggle_flesh == 0)
    {
      GUI_AA_DrawLine(380, 25, 400, 15);
      GUI_AA_DrawLine(400, 15, 420, 25);
    }
    else
    {
      GUI_AA_DrawLine(380, 15, 400, 25);
      GUI_AA_DrawLine(400, 25, 420, 15);
    }
    
    GUI_SetFont(GUI_FONT_20B_1);
    GUI_SetColor(GUI_WHITE);
        
    GUI_DispStringAt("STemWin Library supports bidirectional languages with different fonts.", 80, 50);
    GUI_DispStringAt("This example shows how to decode text with Unicode characters.", 80, 70);
    GUI_DispStringAt("Press on top right button to select the language and highlight the text.", 80, 110);
    
    GUI_DrawBitmap(&bminfo, 765, 10);
    GUI_DrawBitmap(&bmSTLogo, 0, 150);
    
    break;
    
  case  WM_TOUCH:
    WM_DisableWindow(hinfo);
    pState = (const GUI_PID_STATE *)pMsg->Data.p;
    if (pState)
    {
      if (pState->Pressed == 1)
      {
        if (toggle == 0)
        {
          toggle_flesh ^= 1;
          toggle ^= 1;
          for (step = 1; step < 5; step++)
          {
            WM_MoveTo(hinfo, 0, (415 - (step * 30)));
            GUI_Exec();
          }
        }
        else
        {
          toggle_flesh ^= 1;
          toggle ^= 1;
          for (step = 1; step < 5; step++)
          {
            WM_MoveTo(hinfo, 0, (310 + (step * 30)));
            GUI_Exec();
          }
        }
        WM_InvalidateWindow(hinfo);
        GUI_Exec();
      }
      
    }
    WM_EnableWindow(hinfo);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  Callback routine of the main dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMainDialog(WM_MESSAGE * pMsg) {
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    happli = WM_CreateWindowAsChild(0, 0, 800, 430, hfont, WM_CF_SHOW, _cbDialog_font, 0);
    hinfo = WM_CreateWindowAsChild(0, 430, 800, 200, hfont, WM_CF_SHOW, _cbDialog, 0);
    
    break;
    
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}


/**
  * @brief  main application
  * @param  None
  * @retval None
  */
void MainTask(void) {
      
  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();
  
  hfont = GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbMainDialog, WM_GetDesktopWindowEx(0), 0, 0);
  while (1)
  {
    GUI_Delay(100);
    GUI_Exec();
    WM_PaintWindowAndDescs(hfont);
  }
}
