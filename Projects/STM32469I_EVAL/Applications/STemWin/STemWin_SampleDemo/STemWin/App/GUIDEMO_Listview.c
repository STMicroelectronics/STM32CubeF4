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
File        : GUIDEMO_Listview.c
Purpose     : Listview demo
----------------------------------------------------------------------
*/

/**
  ******************************************************************************
  * @file    GUIDEMO_Listview.c
  * @author  MCD Application Team
  * @brief   Listview demo
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "GUIDEMO.h"

#if (SHOW_GUIDEMO_LISTVIEW && GUI_WINSUPPORT)

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LISTVIEW_SIZE_X 210
#define MIN_SPACE       5

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
enum {
  JOB_ADDROWS = 1,
  JOB_SETSORT,
  JOB_REVERSE,
  JOB_INCSEL,
  JOB_DECSEL,
  JOB_COLOR
};

/*********************************************************************
*
*       COL_PROP
*/
typedef struct {
  char * pText;
  int    Width;
  int    Align;
  int (* fpCompare)(const void * p0, const void * p1);
} COL_PROP;

/*********************************************************************
*
*       ACTION_ITEM
*/
typedef struct {
  int          Delay;
  int          Job;
  const char * pDescription;
} ACTION_ITEM;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _pNames
*/
static const char * _pNames[] = {
  "Adina", "Adis", "Aimee", "Alea", "Alessio", "Aletta", "Alexia", "Alissa", "Amelia", 
  "Anakin", "Ania", "Ankea", "Annalies", "Annina", "Annrike", "Arkadius", "Arvin", 
  "Aurelia", "Ava", "Beeke", "Benita", "Bennit", "Bent", "Berenike", "Berit", "Birk", 
  "Bosse", "Carsta", "Catalina", "Celia", "Charis", "Charlin", "Charlotta", "Cordelia", 
  "Corey", "Cornelis", "Corvin", "Cosima", "Crisula", "Darian", "Darina", "Deik", "Derek", 
  "Destina", "Devin", "Diane", "Dina", "Dion", "Dorette", "Dorle", "Duncan", "Eddy", 
  "Einar", "Elea", "Elin", "Elita", "Elona", "Elouise", "Emilian", "Emina", "Emmeline", 
  "Enna", "Estelle", "Etta", "Fabiola", "Fabrice", "Falko", "Felice", "Feline", "Femke", 
  "Fiete", "Filippa", "Fine", "Finia", "Finley", "Fleur", "Florentine" "Florenz", "Florin", 
  "Floyd", "Fokko", "Fritzi", "Gero", "Gerret", "Gerwin", "Gideon", "Gillian", "Glen", 
  "Gunilla", "Hanka", "Heinke", "Helmar", "Henner", "Hennes", "Henrieke", "Immo", 
  "India", "Isalie", "Iven", "Jade", "Janice", "Janika", "Janka", "Janneke", "Jannika", 
  "Jano", "Jarla", "Jarne", "Jarno", "Jaromir", "Jaron", "Jelana", "Jesper", "Joleen", "Jolie", 
  "Jonte", "Joost", "Jördis", "Joris", "Josie", "Josina", "Jula", "Julina", "Jytte", "Kalle", 
  "Kennet", "Keno", "Kiana", "Kieran", "Laila", "Laureen", "Laurens", "Laurent", 
  "Lauritz", "Leana", "Leandra", "Leandro", "Len", "Lenore", "Leona", "Leonore", 
  "Levke", "Lewin", "Lientje", "Lilith", "Line", "Linna", "Linnea", "Lisanne", "Livia", 
  "Lorena", "Lorna", "Lovis", "Luana", "Ludger", "Madita", "Mads", "Maiga", "Mailin", 
  "Malina", "Manja", "Marieke", "Marielle", "Mariette", "Mariola", "Marla", "Marlena", 
  "Marlin", "Marlo", "Marthe", "Martje", "Matz", "Maureen", "Mayra", "Medina", 
  "Melda", "Melek", "Melike", "Melinda", "Melis", "Melisa", "Merit", "Mieke", "Mila", 
  "Mina", "Mio", "Mirella", "Momke", "Momme", "Morris", "Nantke", "Nelson", "Nika", 
  "Norwin", "Oke", "Onno", "Otis", "Ove", "Peer", "Pelle", "Philine", "Philomena", 
  "Quentin", "Quint", "Rahel", "Raik", "Riana", "Ricardo", "Rika", "Rixa", "Rolex", 
  "Ruwen", "Ryan", "Salma", "Sandrine", "Severin", "Sidonie", "Silas", "Silja", "Simeon", 
  "Sinja", "Smilla", "Stig", "Stine", "Talisa", "Tamina", "Tammo", "Tara", "Thyra", 
  "Tiana", "Tjalf", "Tjark", "Tjelle", "Tjorben", "Tomma", "Tore", "Vanity", "Violet", 
  "Violetta", "Wilko", "Wulf", "Yasmina", "Ylvie", "Younes", "Yves", "Yvette",
};

/*********************************************************************
*
*       _aColProps
*/
static const COL_PROP _aColProps[] = {
  { "Name",    70, GUI_TA_LEFT,    LISTVIEW_CompareText},
  { "Code",    50, GUI_TA_HCENTER, LISTVIEW_CompareText},
  { "Balance", 60, GUI_TA_RIGHT,   LISTVIEW_CompareDec }
};

/*********************************************************************
*
*       _aAction
*/
static const ACTION_ITEM _aAction[] = {
  {   0, JOB_ADDROWS,    "Adding some rows" },
  { 900, 0,              NULL },
  { 500, 0,              "Enable sorting" },
  { 500, 0,              NULL },
  { 600, JOB_SETSORT,    NULL  },
  { 600, JOB_SETSORT,    NULL  },
  { 600, JOB_SETSORT,    NULL  },
  { 500, 0,              "Using reverse/normal\nsorting order" },
  { 500, 0,              NULL },
  { 600, JOB_REVERSE,    NULL  },
  { 600, JOB_REVERSE,    NULL  },
  { 600, JOB_REVERSE,    NULL  },
  { 500, 0,              "Changing selection" },
  { 500, 0,              NULL },
  {  50, JOB_INCSEL,     NULL  },
  {  50, JOB_INCSEL,     NULL  },
  {  50, JOB_INCSEL,     NULL  },
  {  50, JOB_INCSEL,     NULL  },
  {  50, JOB_INCSEL,     NULL  },
  {  50, JOB_INCSEL,     NULL  },
  {  50, JOB_INCSEL,     NULL  },
  {  50, JOB_INCSEL,     NULL  },
  {  50, JOB_INCSEL,     NULL  },
  {  50, JOB_INCSEL,     NULL  },
  {  50, JOB_DECSEL,     NULL  },
  {  50, JOB_DECSEL,     NULL  },
  {  50, JOB_DECSEL,     NULL  },
  {  50, JOB_DECSEL,     NULL  },
  {  50, JOB_DECSEL,     NULL  },
  { 500, 0,              "Setting some individual colors" },
  { 500, 0,              NULL },
  {  50, JOB_COLOR,      NULL  },
  {1800, 0,              NULL },
};

/*********************************************************************
*
*       _aColor
*/
static const GUI_COLOR _aColor[] = {
  GUI_BLUE        , GUI_GREEN       , GUI_RED         , GUI_CYAN        , 
  GUI_MAGENTA     , GUI_YELLOW      , GUI_LIGHTBLUE   , GUI_LIGHTGREEN  , 
  GUI_LIGHTRED    , GUI_LIGHTCYAN   , GUI_LIGHTMAGENTA, GUI_LIGHTYELLOW , 
  GUI_DARKBLUE    , GUI_DARKGREEN   , GUI_DARKRED     , GUI_DARKCYAN    , 
  GUI_DARKMAGENTA , GUI_DARKYELLOW  , GUI_WHITE       , GUI_LIGHTGRAY   , 
  GUI_GRAY        , GUI_DARKGRAY    , GUI_BLACK       , GUI_BROWN       , 
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _AddRow
*
* Purpose:
*   Inserts or adds a new row of data to the given LISTVIEW
*
* Parameter:
*   hItem  - Handle of LISTVIEW widget
*   Insert - 1 for inserting a row,0 for adding a row
*/
static int _AddRow(WM_HWIN hItem, int Insert) {
  int    i, r;
  char * pText[3];
  char   acText[3][20] = {0};

  //
  // Create name field
  //
  i = rand() % GUI_COUNTOF(_pNames);
  strcpy(acText[0], _pNames[i]);
  //
  // Create code field
  //
  for (i = 0; i < 5; i++) {
    acText[1][i] = rand() % 26 + 'A';
  }
  //
  // Create balance field
  //
	i = (rand() % 10000) - 5000;
	sprintf(acText[2], "%i", i);
  //
  // Fill pointer array
  //
  for (i = 0; i < 3; i++) {
    pText[i] = acText[i];
  }
  //
  // Add/Insert row
  //
  if (Insert) {
    r = LISTVIEW_InsertRow(hItem, 0, (const GUI_ConstString *)pText);
  } else {
    r = LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pText);
  }
  return r;
}

/*********************************************************************
*
*       _InitListview
*
* Purpose:
*   Adds some data to the listview widget
*/
static void _InitListview(WM_HWIN hList) {
  int i;

  HEADER_SetDragLimit(LISTVIEW_GetHeader(hList), 1);
  LISTVIEW_SetAutoScrollV(hList, 1);
  LISTVIEW_SetGridVis(hList, 1);
  LISTVIEW_SetRBorder(hList, 5);
  LISTVIEW_SetLBorder(hList, 5);
  for (i = 0; i < 3; i++) {
    LISTVIEW_AddColumn(hList, _aColProps[i].Width, _aColProps[i].pText, _aColProps[i].Align);
    LISTVIEW_SetCompareFunc(hList, i, _aColProps[i].fpCompare);
  }
  LISTVIEW_EnableSort(hList);
}

/*********************************************************************
*
*       _Action
*
* Purpose:
*   Does some action with the widget
*/
static void _Action(WM_HWIN hList) {
  unsigned ColoredCol   = 0;
  unsigned ColoredRow   = 0;
  unsigned i, NumRows;
  int      ColorIndex   = 0;
  int      Reverse      = 0;
  int      SortedColumn = -1;
  int      Index;

  Index = 0;
  do {
    if (_aAction[Index].pDescription) {
      GUIDEMO_ShowInfo(_aAction[Index].pDescription);
    }
    switch (_aAction[Index].Job) {
    case JOB_COLOR:
      NumRows = LISTVIEW_GetNumRows(hList);
      for (i = 0; i < NumRows; i++) {
        LISTVIEW_SetItemBkColor(hList, ColoredCol, ColoredRow, LISTVIEW_CI_UNSEL, _aColor[ColorIndex]);
        if (++ColorIndex == GUI_COUNTOF(_aColor)) {
          ColorIndex = 0;
        }
        if (++ColoredCol == LISTVIEW_GetNumColumns(hList)) {
          ColoredCol = 0;
        }
        if (++ColoredRow == NumRows) {
          ColoredRow = 0;
        }
      }
      break;
    case JOB_INCSEL:
      LISTVIEW_IncSel(hList);
      break;
    case JOB_DECSEL:
      LISTVIEW_DecSel(hList);
      break;
    case JOB_REVERSE:
      Reverse ^= 1;
      LISTVIEW_SetSort(hList, SortedColumn, Reverse);
      break;
    case JOB_ADDROWS:
      for (i = 0; i < 40; i++) {
        _AddRow(hList, 0);
      }
      break;
    case JOB_SETSORT:
      if (SortedColumn == -1) {
        SortedColumn = 0;
      } else {
        SortedColumn++;
        if (SortedColumn == (int)LISTVIEW_GetNumColumns(hList)) {
          SortedColumn = 0;
        }
      }
      LISTVIEW_SetSort(hList, SortedColumn, Reverse);
      break;
    }
    GUI_Delay(_aAction[Index].Delay);
  } while ((++Index < GUI_COUNTOF(_aAction)) && (GUIDEMO_CheckCancel() == 0));
}

/*********************************************************************
*
*       _DemoListview
*/
static void _DemoListview(void) {
  WM_HWIN hList;
  int     xSize, ySize, xSpace, ySpace;

  xSize  = LCD_GetXSize();
  ySize  = LCD_GetYSize();
  if ((CONTROL_SIZE_X + MIN_SPACE) < ((xSize - LISTVIEW_SIZE_X) >> 1)) {
    xSpace = (xSize - LISTVIEW_SIZE_X) >> 1;
  } else {
    xSpace = (xSize - LISTVIEW_SIZE_X - CONTROL_SIZE_X) >> 1;
  }
  ySpace = (ySize - INFO_SIZE_Y) >> 3;
  hList  = LISTVIEW_CreateEx(xSpace, INFO_SIZE_Y + ySpace, LISTVIEW_SIZE_X, ySize - INFO_SIZE_Y - (ySpace << 1), WM_HBKWIN, WM_CF_SHOW, 0, GUI_ID_LISTVIEW0);
  WM_SetFocus(hList);
  _InitListview(hList);
  _Action(hList);
  WM_DeleteWindow(hList);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIDEMO_Listview
*/
void GUIDEMO_Listview(void) {
  GUIDEMO_ShowIntro("Listview demo",
                    "Shows some features of\n"
                    "the LISTVEW widget");
  GUIDEMO_DrawBk(1);
  GUIDEMO_ShowInfoWin();
  _DemoListview();
}

#else

void GUIDEMO_Listview(void) {}

#endif

/*************************** End of file ****************************/
