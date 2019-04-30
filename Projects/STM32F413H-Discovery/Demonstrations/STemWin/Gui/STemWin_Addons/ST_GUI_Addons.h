/**
  ******************************************************************************
  * @file    ST_Addons.h
  * @author  MCD Application Team
  * @brief   ST Graphical addons : Extension for STemWin
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ST_GUI_ADDONS_H
#define __ST_GUI_ADDONS_H

#ifdef __cplusplus
 extern "C" {
#endif
 
/* Includes ------------------------------------------------------------------*/   
#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"
#include "CHOOSEFILE.h"
   
#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

#ifndef GUI_FLASH
  #define GUI_FLASH
#endif    

/* Exported types ------------------------------------------------------------*/
typedef WM_HMEM ST_ANIMATED_ICONVIEW_Handle;

/* Exported variables ------------------------------------------------------------*/
/*** ST Fonts ****/
extern GUI_FLASH const GUI_FONT GUI_FontAvantGarde16;
extern GUI_FLASH const GUI_FONT GUI_FontAvantGarde16B;
extern GUI_FLASH const GUI_FONT GUI_FontAvantGarde20;
extern GUI_FLASH const GUI_FONT GUI_FontAvantGarde20B;
extern GUI_FLASH const GUI_FONT GUI_FontAvantGarde24;
extern GUI_FLASH const GUI_FONT GUI_FontAvantGarde24B;
extern GUI_FLASH const GUI_FONT GUI_FontAvantGarde32;
extern GUI_FLASH const GUI_FONT GUI_FontAvantGarde32B;

extern GUI_FLASH const GUI_FONT GUI_FontLubalGraph16;
extern GUI_FLASH const GUI_FONT GUI_FontLubalGraph16B;
extern GUI_FLASH const GUI_FONT GUI_FontLubalGraph20;
extern GUI_FLASH const GUI_FONT GUI_FontLubalGraph20B;
extern GUI_FLASH const GUI_FONT GUI_FontLubalGraph24;
extern GUI_FLASH const GUI_FONT GUI_FontLubalGraph24B;
extern GUI_FLASH const GUI_FONT GUI_FontLubalGraph32;
extern GUI_FLASH const GUI_FONT GUI_FontLubalGraph32B;

/* Exported constants --------------------------------------------------------*/
#define GUI_STCOLOR_LIGHTBLUE   0x00DCA939
#define GUI_STCOLOR_DARKBLUE    0x00522000

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** ST Skins ***/ 
void SLIDER_SetSTSkin(WM_HMEM hObj);
void SCROLLBAR_SetSTSkin(WM_HMEM hObj);
void HEADER_SetSTSkin(WM_HMEM hObj);
void SLIDER_SetDefaultSTSkin(void);
void SCROLLBAR_SetDefaultSTSkin(void);
void HEADER_SetDefaultSTSkin(void);

/** ST Widgets ***/ 
ST_ANIMATED_ICONVIEW_Handle ST_AnimatedIconView_CreateEx      (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int xSizeItems, int ySizeItems, int delay, int FrameNbr);
ST_ANIMATED_ICONVIEW_Handle ST_AnimatedIconView_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int xSizeItems, int ySizeItems, int NumExtraBytes, int delay, int FrameNbr);
ST_ANIMATED_ICONVIEW_Handle ST_AnimatedIconView_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb, int delay, int FrameNbr);
void ST_AnimatedIconView_DeleteItem              (ST_ANIMATED_ICONVIEW_Handle hObj, unsigned Index);
U32  ST_AnimatedIconView_GetItemUserData         (ST_ANIMATED_ICONVIEW_Handle hObj, int Index);
int  ST_AnimatedIconView_GetNumItems             (ST_ANIMATED_ICONVIEW_Handle hObj);
int  ST_AnimatedIconView_GetItemText             (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, char * pBuffer, int MaxSize);
int  ST_AnimatedIconView_GetSel                  (ST_ANIMATED_ICONVIEW_Handle hObj);
int  ST_AnimatedIconView_GetUserData             (ST_ANIMATED_ICONVIEW_Handle hObj, void * pDest, int NumBytes);
void ST_AnimatedIconView_SetBkColor              (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, GUI_COLOR Color);
void ST_AnimatedIconView_SetFont                 (ST_ANIMATED_ICONVIEW_Handle hObj, const GUI_FONT * pFont);
void ST_AnimatedIconView_SetDualFont             (ST_ANIMATED_ICONVIEW_Handle hObj, const GUI_FONT * pFont1, const GUI_FONT * pFont2);
void ST_AnimatedIconView_SetFrame                (ST_ANIMATED_ICONVIEW_Handle hObj, int Coord, int Value);
void ST_AnimatedIconView_SetItemText             (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, const char * pText);
void ST_AnimatedIconView_SetDualItemText         (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, const char * pText, char * pText1);
void ST_AnimatedIconView_SetItemUserData         (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, U32 UserData);
void ST_AnimatedIconView_SetSel                  (ST_ANIMATED_ICONVIEW_Handle hObj, int Sel);
void ST_AnimatedIconView_SetSpace                (ST_ANIMATED_ICONVIEW_Handle hObj, int Coord, int Value);
void ST_AnimatedIconView_SetIconAlign            (ST_ANIMATED_ICONVIEW_Handle hObj, int IconAlign);
void ST_AnimatedIconView_SetTextAlign            (ST_ANIMATED_ICONVIEW_Handle hObj, int TextAlign);
void ST_AnimatedIconView_SetTextColor            (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, GUI_COLOR Color);
void ST_AnimatedIconView_SetDualTextColor        (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, GUI_COLOR Color, GUI_COLOR Color1);
int  ST_AnimatedIconView_SetUserData             (ST_ANIMATED_ICONVIEW_Handle hObj, const void * pSrc, int NumBytes);
void ICONVIEW_SetWrapMode_ST             (ST_ANIMATED_ICONVIEW_Handle hObj, GUI_WRAPMODE WrapMode);

int  ST_AnimatedIconView_SetIcon(ST_ANIMATED_ICONVIEW_Handle hObj, int Index, const GUI_BITMAP ** pOpenBitmap, const GUI_BITMAP ** pCloseBitmap, int size, int index);
int  ST_AnimatedIconView_AddIcon(ST_ANIMATED_ICONVIEW_Handle hObj, const GUI_BITMAP ** pOpenBitmap, const GUI_BITMAP ** pCloseBitmap, const char * pText);
void ST_AnimatedIconView_SetNextButton(ST_ANIMATED_ICONVIEW_Handle hObj);


WM_HWIN ST_CHOOSEFILE_Create(WM_HWIN        hParent,  
                          int               xPos,     
                          int               yPos,     
                          int               xSize,    
                          int               ySize,    
                          const char      * apRoot[], 
                          int               NumRoot,  
                          int               SelRoot,  
                          const char      * sCaption, 
                          int               Flags,    
                          CHOOSEFILE_INFO * pInfo  
                          );

void    ST_CHOOSEFILE_Callback            (WM_MESSAGE * pMsg);
void    ST_CHOOSEFILE_EnableToolTips      (void);
void    ST_CHOOSEFILE_SetButtonText       (WM_HWIN hWin, unsigned ButtonIndex, const char * pText);
void    ST_CHOOSEFILE_SetDefaultButtonText(unsigned ButtonIndex, const char * pText);
void    ST_CHOOSEFILE_SetDelim            (char Delim);
void    ST_CHOOSEFILE_SetToolTips         (const TOOLTIP_INFO * pInfo, int NumItems);
void    ST_CHOOSEFILE_SetTopMode          (unsigned OnOff);
void    ST_CHOOSEFILE_SetButtonSize       (int XSize, int YSize) ;
void    ST_CHOOSEFILE_SetButtonBitmaps    (int index, 
                                           const GUI_BITMAP * EnabledBMP, 
                                           const GUI_BITMAP * DisabledBMP) ;
/** ST 2D Drawing methods ***/ 
void DrawRect3D(int x0, int y0, int width, int height);

#ifdef __cplusplus
}
#endif

#endif /*__ST_GUI_ADDONS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
