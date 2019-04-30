/*********************************************************************
*          Portions COPYRIGHT 2016 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.28 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDEMO.c
Purpose     : Several GUIDEMO routines
----------------------------------------------------------------------
*/

/**
  ******************************************************************************
  * @file    GUIDEMO.c
  * @author  MCD Application Team
  * @brief   Several GUIDEMO routines
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

#include "GUIDEMO.h"

/*********************************************************************
*
*       Dialog resources
*
**********************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aFrameWinControl[] = {
  { FRAMEWIN_CreateIndirect, "Control", 0,                0,  0, CONTROL_SIZE_X, CONTROL_SIZE_Y, 0,          0 },
  { BUTTON_CreateIndirect,   "Halt",    GUI_ID_HALT,      2, 24, BUTTON_SIZE_X,  BUTTON_SIZE_Y,  0,          0 },
  { BUTTON_CreateIndirect,   "Next",    GUI_ID_NEXT,     36, 24, BUTTON_SIZE_X,  BUTTON_SIZE_Y,  0,          0 },
  { PROGBAR_CreateIndirect,  0,         GUI_ID_PROGBAR0,  2, 11, PROGBAR_SIZE_X, PROGBAR_SIZE_Y, WM_CF_HIDE, 0 },
  { TEXT_CreateIndirect,     0,         GUI_ID_TEXT0,     2,  2, TEXT_SIZE_X,    TEXT_SIZE_Y,    0,          0 }
};

static const GUI_WIDGET_CREATE_INFO _aFrameWinInfo[] = {
  { FRAMEWIN_CreateIndirect, "STemWin Demo", 0,             0,  0, 0,              0,              0, 0 },
  { TEXT_CreateIndirect,     "",           GUI_ID_TEXT0,  3,  3, 0,              0,              0, 0 }
};

/*********************************************************************
*
*       Static variables
*
**********************************************************************
*/
static GUIDEMO_CONFIG _GUIDemoConfig;
static WM_HWIN        _hDialogControl;
static WM_HWIN        _hDialogInfo;

static void (* _pfDrawBk)(int DrawLogo);
static int     _iDemo;
static int     _iDemoMinor;
static int     _HaltTime;
static int     _HaltTimeStart;
static int     _Halt;
static int     _Next;
static int     _Pressed;
static U8      _DrawLogo;

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/
/*********************************************************************
*
*       _ClearHalt
*
*   This function is called if the next button is pressed after
*   the demo was halted
*/
static void _ClearHalt(void) {
  _Halt          = 0;
  _HaltTime      = 0;
  _HaltTimeStart = 0;
}

/*********************************************************************
*
*       _DrawBkSimple
*/
static void _DrawBkSimple(int DrawLogo) {
  GUI_SetBkColor(BK_COLOR_1);
  GUI_Clear();
  if (DrawLogo) {
    GUI_DrawBitmap(&bmSTLogo70x35, 5, 5);
  }
}

#if GUIDEMO_USE_AUTO_BK

/*********************************************************************
*
*       _DrawBk
*/
static void _DrawBk(int DrawLogo) {
  int xSize;
  int ySize;

  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();
  GUI_DrawGradientV(0, 0, xSize, ySize, BK_COLOR_0, BK_COLOR_1);
  if (DrawLogo) {
    GUI_DrawBitmap(&bmSTLogo70x35, 5, 5);
  }
}

/*********************************************************************
*
*       _DrawBkCircle
*/
static void _DrawBkCircle(int DrawLogo) {
  static GUI_MEMDEV_Handle   hMemStretch;
  GUI_MEMDEV_Handle          hMemGradient;
  GUI_MEMDEV_Handle          hMemCircle;
  GUI_MEMDEV_Handle          hMemOld;
  int                        CircleWidth;
  int                        ySizeV;
  int                        xSize;
  int                        ySize;
  int                        i;
  U32                      * pData;

  xSize  = LCD_GetXSize();
  ySize  = LCD_GetYSize();
  ySizeV = LCD_GetVYSize();
  if (ySizeV > ySize) {
    GUI_SetBkColor(BK_COLOR_1);
    GUI_ClearRect(0, 0, xSize, ySizeV);
  }
  if (hMemStretch == 0) {
    CircleWidth  = (CIRCLE_RADIUS << 1) + 1;
    hMemCircle   = GUI_MEMDEV_CreateFixed(0, 0, CircleWidth, CircleWidth,   GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
    hMemStretch  = GUI_MEMDEV_CreateEx   (0, 0, xSize,       ySize,         GUI_MEMDEV_NOTRANS);
    hMemGradient = GUI_MEMDEV_CreateFixed(0, 0, 1,           CIRCLE_RADIUS, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
    //
    // Initialize background
    //
    hMemOld = GUI_MEMDEV_Select(hMemCircle);
    GUI_SetBkColor(BK_COLOR_1);
    GUI_Clear();
    //
    // Create Gradient
    //
    GUI_MEMDEV_Select(hMemGradient);
    GUI_DrawGradientV(0, 0, 0, CIRCLE_RADIUS, BK_COLOR_0, BK_COLOR_1);
    //
    // Get color and draw circles
    //
    pData = (U32 *)GUI_MEMDEV_GetDataPtr(hMemGradient);
    GUI_MEMDEV_Select(hMemCircle);
    for (i = 0; i < CIRCLE_RADIUS; i++, pData++) {
      GUI_SetColor(*pData);
      GUI_DrawCircle(CIRCLE_RADIUS, CIRCLE_RADIUS, i);
    }
    //
    // Stretch and display
    //
    GUI_MEMDEV_Select(hMemStretch);
    GUI_MEMDEV_DrawPerspectiveX(hMemCircle, 0, 0, ySize, ySize, xSize, 0);
    GUI_MEMDEV_Delete(hMemCircle);
    GUI_MEMDEV_Delete(hMemGradient);
    GUI_MEMDEV_Select(hMemOld);
  }
  GUI_MEMDEV_Write(hMemStretch);
  if (DrawLogo) {
    GUI_DrawBitmap(&bmSTLogo70x35, LOGO_DIST_BORDER, LOGO_DIST_BORDER);
  }
}

#endif

/*********************************************************************
*
*       _HideProgress
*/
static void _HideProgress(void) {
  PROGBAR_Handle hProg;

  hProg = WM_GetDialogItem(_hDialogControl, GUI_ID_PROGBAR0);
  WM_HideWindow(hProg);
}

/*********************************************************************
*
*       _ShowProgress
*/
static void _ShowProgress(void) {
  PROGBAR_Handle hProg;

  hProg = WM_GetDialogItem(_hDialogControl, GUI_ID_PROGBAR0);
  WM_ShowWindow(hProg);
}

/*********************************************************************
*
*       _cbBk
*/
static void _cbBk(WM_MESSAGE * pMsg) {
  WM_KEY_INFO * pInfo;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    _pfDrawBk(_DrawLogo);
    break;
  case WM_SET_FOCUS:
    pMsg->Data.v = 0;
    break;
  case WM_KEY:
    pInfo = (WM_KEY_INFO *)pMsg->Data.p;
    if (pInfo->PressedCnt) {
      switch (pInfo->Key) {
      case 'n':
        _Next = 1;
        break;
      }
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbEffect
*/
static int _cbEffect(int TimeRem, void * pVoid) {
  GUI_PID_STATE State;
  int           Pressed;

  GUI_USE_PARA(TimeRem);
  Pressed = *((int *)pVoid);
  GUI_Exec();
  GUI_PID_GetState(&State);
  if (State.Pressed) {
    *((int *)pVoid) = 1;
    return 0;
  } else {
    if ((State.Pressed == 0) && (Pressed == 1)) {
      *((int *)pVoid) = 0;
      _Next   = 1;
      return 1;
    }
    _Next = GUIDEMO_CheckCancel();
    return _Next;
  }
}

/*********************************************************************
*
*       _cbFrameWinControl
*/
static void _cbFrameWinControl(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     xSize;
  int     ySize;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_KEY:
    WM_SendMessage(WM_HBKWIN, pMsg);
    break;
  case WM_INIT_DIALOG:
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
    PROGBAR_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    PROGBAR_SetFont(hItem, &GUI_FontD6x8);
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_HALT);
    BUTTON_SetFocussable(hItem, 0);
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_NEXT);
    BUTTON_SetFocussable(hItem, 0);
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0);
    TEXT_SetText(hItem, "Intro");
    TEXT_SetFont(hItem, &GUI_Font8_ASCII);
    break;
  case WM_PAINT:
    xSize = WM_GetWindowSizeX(pMsg->hWin);
    ySize = WM_GetWindowSizeY(pMsg->hWin);
    GUI_DrawGradientV(0, 0, xSize - 1, ySize - 1, 0xFFFFFF, 0xDCCEC0);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch (NCode) {
    case WM_NOTIFICATION_CLICKED:
      switch (Id) {
      case GUI_ID_HALT:
        if (_Halt) {
          _Halt          = 0;
          _HaltTime      = GUI_GetTime() - _HaltTimeStart;
          WM_MakeModal(0);
        } else {
          _Halt          = 1;
          _HaltTimeStart = GUI_GetTime() - _HaltTime;
          WM_MakeModal(pMsg->hWin);
        }
        break;
      case GUI_ID_NEXT:
        _Next = 1;    // Will be set to 0 by GUIDEMO_GetNextState()
        _ClearHalt(); // Clear _Halt, so the next sample demonstrates immediately
        break;
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbFrameWinInfo
*/
static void _cbFrameWinInfo(WM_MESSAGE * pMsg) {
  int xSize;
  int ySize;

  switch (pMsg->MsgId) {
  case WM_KEY:
    WM_SendMessage(WM_HBKWIN, pMsg);
    break;
  case WM_CREATE:
    xSize = LCD_GetXSize();
    WM_SetWindowPos(pMsg->hWin, xSize >> 1, 0, xSize >> 1, INFO_SIZE_Y);
    break;
  case WM_PAINT:
    xSize = WM_GetWindowSizeX(pMsg->hWin);
    ySize = WM_GetWindowSizeY(pMsg->hWin);
    GUI_DrawGradientV(0, 0, xSize - 1, ySize - 1, 0xFFFFFF, 0xDCCEC0);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _FRAMEWIN_DrawSkinFlex
*/
static int _FRAMEWIN_DrawSkinFlex(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_CREATE:
    FRAMEWIN_SetTextAlign(pDrawItemInfo->hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
    FRAMEWIN_SetTextColor(pDrawItemInfo->hWin, GUI_BLACK);
    break;
  default:
    return FRAMEWIN_DrawSkinFlex(pDrawItemInfo);
  }
  return 0;
}

/*********************************************************************
*
*       _Main
*/
static void _Main(void) {
  int xSize;
  int ySize;

  WM_SelectWindow(WM_HBKWIN);
  GUI_Clear();
  #if (GUI_SUPPORT_CURSOR | GUI_SUPPORT_TOUCH)
//    GUI_CURSOR_Show();
  #endif
  //
  // Create and configure Control and Information window
  //
  xSize           = LCD_GetXSize();
  ySize           = LCD_GetYSize();
  _hDialogControl = GUI_CreateDialogBox(_aFrameWinControl, GUI_COUNTOF(_aFrameWinControl), &_cbFrameWinControl, WM_HBKWIN, xSize - CONTROL_SIZE_X, ySize - CONTROL_SIZE_Y);
  _hDialogInfo    = GUI_CreateDialogBox(_aFrameWinInfo,    GUI_COUNTOF(_aFrameWinInfo),    &_cbFrameWinInfo,    WM_HBKWIN, (xSize >> 1) - 1,       0);
  WM_HideWindow(_hDialogInfo);
  //
  // Show Intro
  //
  WM_InvalidateWindow(_hDialogControl);
  WM_DisableMemdev(WM_HBKWIN);
  GUI_Exec();
  WM_EnableMemdev(WM_HBKWIN);
  
  GUI_SetBkColor(GUI_RED);
  GUI_Clear();
  GUI_SetBkColor(GUI_GREEN);
  GUI_Clear();
  GUI_SetBkColor(GUI_BLUE);
  GUI_Clear();
  
  GUIDEMO_Intro();
  //
  // Run the demos
  //
  for (_iDemo = 0; _GUIDemoConfig.apFunc[_iDemo]; _iDemo++) {
    _ClearHalt();
    GUIDEMO_UpdateControlText();
    (*_GUIDemoConfig.apFunc[_iDemo])();
    _iDemoMinor = 0;
    _Pressed    = 0;
  }
  _iDemo = 0;
  //
  // Cleanup
  //
  WM_DeleteWindow(_hDialogControl);
  WM_DeleteWindow(_hDialogInfo);
  #if (GUI_SUPPORT_CURSOR | GUI_SUPPORT_TOUCH)
    GUI_CURSOR_Hide();
  #endif
}

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIDEMO_SetDrawLogo
*/
void GUIDEMO_SetDrawLogo(U8 OnOff) {
  _DrawLogo = OnOff ? 1 : 0;
}

/*********************************************************************
*
*       GUIDEMO_AddIntToString
*/
void GUIDEMO_AddIntToString(char * pText, unsigned int Number) {
  int TextLen;
  int LenNum;
  int i;

  TextLen = 0;
  while (*(pText + TextLen)) {
    TextLen++;
  }
  i       = 1;
  LenNum  = 1;
  while ((Number / i) >= 10) {
    i *= 10;
    LenNum++;
  }
  *(pText + TextLen + LenNum) = '\0';
  while (LenNum) {
    *(pText + TextLen + LenNum - 1) = '0' + Number % 10;
    Number /= 10;
    LenNum--;
  }
}

/*********************************************************************
*
*       GUIDEMO_AddStringToString
*/
void GUIDEMO_AddStringToString(char * pText, const char * acAdd) {
  int i;
  int j;

  i = 0;
  j = 0;
  while (*(pText + i)) {
    i++;
  }
  while (*(acAdd + j)) {
    *(pText + i) = *(acAdd + j);
    i++;
    j++;
  }
  *(pText + i) = '\0';
}

/*********************************************************************
*
*       GUIDEMO_CheckCancel
*/
int GUIDEMO_CheckCancel(void) {
  //
  // Do not return until the button is released
  //
  while (_Halt == 1) {
    GUI_Delay(10);
  }
  //
  // Check Next Button
  //
  if (_Next == 1) {
    _Next = 0;
    return 1;
  }
  return 0;
}

/*********************************************************************
*
*       GUIDEMO_ClearText
*
*/
void GUIDEMO_ClearText(char * pText) {
  *pText = 0;
}

/*********************************************************************
*
*       GUIDEMO_Delay
*
*   This function has to be called if the current step of the sample
*   is the last one and consists of a single frame
*/
void GUIDEMO_Delay(int TimeDelay) {
  PROGBAR_Handle hProg;
  int            NextState;
  U32            TimeStart;
  U32            TimeDiff;

  hProg = WM_GetDialogItem(_hDialogControl, GUI_ID_PROGBAR0);
  if (TimeDelay > SHOW_PROGBAR_AT) {
    _ShowProgress();
  }
  PROGBAR_SetValue(hProg, 0);
  PROGBAR_SetMinMax(hProg, 0, TimeDelay);
  TimeStart     = GUI_GetTime();
  do {
    
     
    TimeDiff = GUIDEMO_GetTime() - TimeStart;
    if (TimeDelay > SHOW_PROGBAR_AT) {
      PROGBAR_SetValue(hProg, TimeDiff);
    }
    GUI_Delay(5);
    NextState = GUIDEMO_CheckCancel();
  } while (TimeDiff < (U32)TimeDelay && !NextState);
  if (TimeDelay > SHOW_PROGBAR_AT) {
    _HideProgress();
  }
  GUI_Exec();
}

/*********************************************************************
*
*       GUIDEMO_DrawBk
*/
void GUIDEMO_DrawBk(int DrawLogo) {
  _pfDrawBk(DrawLogo);
}

/*********************************************************************
*
*       GUIDEMO_GetConfFlag
*/
U16 GUIDEMO_GetConfFlag(U16 Flag) {
  return (_GUIDemoConfig.Flags & Flag) ? 1 : 0;
}

/*********************************************************************
*
*       GUIDEMO_GetTime
*/
int GUIDEMO_GetTime(void) {
  return _Halt ? _HaltTimeStart : GUI_GetTime() - _HaltTime;
}

/*********************************************************************
*
*       GUIDEMO_HideControlWin
*/
void GUIDEMO_HideControlWin(void) {
  WM_HideWindow(_hDialogControl);
  WM_ValidateWindow(WM_HBKWIN);
}

/*********************************************************************
*
*       GUIDEMO_HideInfoWin
*/
void GUIDEMO_HideInfoWin(void) {
  WM_HideWindow(_hDialogInfo);
  WM_ValidateWindow(WM_HBKWIN);
}

/*********************************************************************
*
*       GUIDEMO_NotifyStartNext
*
*   Use this function if the next step of the current sample will be
*   shown immediately
*/
void GUIDEMO_NotifyStartNext(void) {
  _iDemoMinor++;
  _ClearHalt();
  GUIDEMO_UpdateControlText();
}

/*********************************************************************
*
*       GUIDEMO_ShowControlWin
*/
void GUIDEMO_ShowControlWin(void) {
  WM_ShowWindow(_hDialogControl);
  GUI_Exec();
}

/*********************************************************************
*
*       GUIDEMO_ShowInfo
*/
void GUIDEMO_ShowInfo(const char * acInfo) {
  TEXT_Handle hText;

  if (WM_IsVisible(_hDialogInfo)) {
    hText = WM_GetDialogItem(_hDialogInfo, GUI_ID_TEXT0);
    TEXT_SetText(hText, acInfo);
  }
}

/*********************************************************************
*
*       GUIDEMO_ShowInfoWin
*/
void GUIDEMO_ShowInfoWin(void) {
  WM_ShowWindow(_hDialogInfo);
}

/*********************************************************************
*
*       GUIDEMO_ShowIntro
*
*  Function description
*    Shows the GUIDEMO introduction screen which display the title of
*    the sample and a short description.
*/
void GUIDEMO_ShowIntro(const char * acTitle, const char * acDescription) {
  int FontDistY;
  int TimeWait;
  int xCenter;
  int yCenter;
  int xSize;
  int ySize;
  int i;

  xSize   = LCD_GetXSize();
  ySize   = LCD_GetYSize();
  xCenter = xSize >> 1;
  yCenter = ySize >> 1;
  GUIDEMO_HideInfoWin();
  GUIDEMO_ShowControlWin();
  GUI_Exec();
  GUIDEMO_DrawBk(1);
  GUI_SetColor(GUI_WHITE);
  //
  // Title
  //
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_SetFont(&GUI_FontRounded22);
  GUI_DispStringHCenterAt(acTitle, xCenter, 60);
  //
  // Description
  //
  GUI_SetFont(&GUI_Font16_ASCII);
  FontDistY = GUI_GetFontDistY();
  GUI_DispStringHCenterAt(acDescription, xCenter, yCenter - FontDistY + 10);
  //
  // Determine time to wait
  //
  i = 0;
  while (acDescription[i]) {
    i++;
  }
  TimeWait = i * CHAR_READING_TIME;
  GUIDEMO_Wait(TimeWait);
}

/*********************************************************************
*
*       GUIDEMO_UpdateControlText
*/
void GUIDEMO_UpdateControlText(void) {
  TEXT_Handle hText;
  char        acText[20] = { 0 };

  hText = WM_GetDialogItem(_hDialogControl, GUI_ID_TEXT0);
  GUIDEMO_AddStringToString(acText, "Demo ");
  GUIDEMO_AddIntToString   (acText, _iDemo + 1);
  GUIDEMO_AddStringToString(acText, ".");
  GUIDEMO_AddIntToString   (acText, _iDemoMinor);
  GUIDEMO_AddStringToString(acText, "/");
  GUIDEMO_AddIntToString   (acText, _GUIDemoConfig.NumDemos - 1);
  TEXT_SetText             (hText,  acText);
}

/*********************************************************************
*
*       GUIDEMO_Wait
*
*   This function has to be called if the current step is a static
*   frame and another step will follow
*/
void GUIDEMO_Wait(int TimeWait) {
  GUIDEMO_Delay(TimeWait);
  GUIDEMO_NotifyStartNext();
}

/*********************************************************************
*
*       GUIDEMO_Main
*/
void GUIDEMO_Main(void) {
  FRAMEWIN_SKINFLEX_PROPS Framewin_Props;
#if GUIDEMO_USE_AUTO_BK
  int                     NumFreeBytes;
  int                     BitsPerPixel;
#endif

  GUI_MEMDEV_SetAnimationCallback(_cbEffect, (void *)&_Pressed);
  WM_SetCallback(WM_HBKWIN, _cbBk);
  BUTTON_SetReactOnLevel();
  FRAMEWIN_GetSkinFlexProps(&Framewin_Props, FRAMEWIN_SKINFLEX_PI_ACTIVE);
  Framewin_Props.Radius = 0;
  FRAMEWIN_SetSkinFlexProps(&Framewin_Props, FRAMEWIN_SKINFLEX_PI_ACTIVE);
  FRAMEWIN_GetSkinFlexProps(&Framewin_Props, FRAMEWIN_SKINFLEX_PI_INACTIVE);
  Framewin_Props.Radius = 0;
  FRAMEWIN_SetSkinFlexProps(&Framewin_Props, FRAMEWIN_SKINFLEX_PI_INACTIVE);
  FRAMEWIN_SetDefaultSkin  (_FRAMEWIN_DrawSkinFlex);
  PROGBAR_SetDefaultSkin   (PROGBAR_SKIN_FLEX);
  BUTTON_SetDefaultSkin    (BUTTON_SKIN_FLEX);
  SCROLLBAR_SetDefaultSkin (SCROLLBAR_SKIN_FLEX);
  SLIDER_SetDefaultSkin    (SLIDER_SKIN_FLEX);
  HEADER_SetDefaultSkin    (HEADER_SKIN_FLEX);
  GUI_SetTextMode          (GUI_TM_TRANS);
  GUIDEMO_Config(&_GUIDemoConfig);
  #if GUIDEMO_USE_VNC
    if (GUIDEMO_GetConfFlag(GUIDEMO_CF_USE_VNC)) {
      _GUIDemoConfig.pGUI_VNC_X_StartServer(0, 0);
    }
  #endif
  #if GUIDEMO_USE_AUTO_BK
    //
    // Determine if HW has enough memory to draw the gradient circle as background
    //
    BitsPerPixel = LCD_GetBitsPerPixel();
    if ((BitsPerPixel >= 16) && GUIDEMO_GetConfFlag(GUIDEMO_CF_USE_AUTO_BK)) {
      NumFreeBytes = GUI_ALLOC_GetNumFreeBytes();
      if (NumFreeBytes > NUMBYTES_NEEDED) {
        _pfDrawBk = _DrawBkCircle;
      } else {
        _pfDrawBk = _DrawBk;
      }
    } else
  #endif
    {
      _pfDrawBk = _DrawBkSimple;
    }
  GUIDEMO_SetDrawLogo(1);
  while (1) {
    _Main();
  }
}

/*************************** End of file ****************************/

