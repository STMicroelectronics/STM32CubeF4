/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.18 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software  has been  licensed to  STMicroelectronics  International
N.V. whose  registered office  is situated at Plan-les-Ouates, Geneva,
39 Chemin du Champ des Filles,  Switzerland solely for the purposes of
creating libraries for  STMicroelectronics  ARM Cortex™-M-based 32-bit
microcontroller    products,    sublicensed    and    distributed   by
STMicroelectronics  under  the  terms  and  conditions of the End User
License Agreement supplied with  the software. The use of the software
on a stand-alone basis  or for any purpose other  than to operate with
the specified  microcontroller is prohibited and subject to a separate
license agreement.

Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : TOUCH_Calibrate.c
Purpose     : Demonstrates how a touch screen can be calibrated at run time
---------------------------END-OF-HEADER------------------------------
*/

#include "calibration.h"
#include "rtc.h"
#include "GUI.h"


typedef union
{
  struct
  {
    uint32_t     A1 : 15; 
    uint32_t     B1 : 16;
    uint32_t     Reserved : 1; 
  }b;
  uint32_t d32;
}CALIBRATION_Data1Typedef; 

typedef union
{
  struct
  {
    uint32_t      A2 : 15;
    uint32_t      B2 : 16;
    uint32_t      IsCalibrated : 1;
  }b;
  uint32_t d32;
  
}CALIBRATION_Data2Typedef; 

uint32_t CALIBRATION_Done = 0;
/********************************************************************
*
*       Static data
*
*********************************************************************
*/

static const char * _acPos[] = {
  "(upper left position)",
  "(lower right position)"
};

int16_t  A1, A2, B1, B2;
  CALIBRATION_Data1Typedef data1;
  CALIBRATION_Data2Typedef data2;
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _WaitForPressedState
*
* Purpose:
*   Waits until the touch is in the given pressed state for at least 250 ms
*/
static void _WaitForPressedState(int Pressed) {
  GUI_PID_STATE State;
  do {
    GUI_TOUCH_GetState(&State);
    GUI_Delay(10);
    if (State.Pressed == Pressed) {
      int TimeStart = GUI_GetTime();
      do {
        GUI_TOUCH_GetState(&State);
        GUI_Delay(10);
        if (State.Pressed != Pressed) {
          break;
        } else if ((GUI_GetTime() - 150) > TimeStart) {
          return;
        }
      } while (1);
    }
  } while (1);
}

/*********************************************************************
*
*       _DispStringCentered
*
* Purpose:
*   Shows the given text horizontally and vertically centered
*/
static void _DispStringCentered(const char * pString) {
  GUI_RECT Rect;
  Rect.x0 = Rect.y0 = 0;
  Rect.x1 = LCD_GetXSize() - 1;
  Rect.y1 = LCD_GetYSize() - 1;
  GUI_DispStringInRect(pString, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
}

/*********************************************************************
*
*       _GetPhysValues
*
* Purpose:
*   Asks the user to press the touch screen at the given position
*   and returns the physical A/D values
*/
static void _GetPhysValues(int LogX, int LogY, int * pPhysX, int * pPhysY, const char * pString) {
  char acText[] = "Press here";
  GUI_RECT Rect;
  GUI_PID_STATE State;
  int FontSizeY, Align;
  FontSizeY = GUI_GetFontSizeY();
  GUI_Clear();
  GUI_SetColor(GUI_BLACK);
  _DispStringCentered("Runtime calibration,\n"
                      "please touch the screen\n"
                      "at the center of the ring."); /* Ask user to press the touch */
  /* Calculate the rectangle for the string */
  Rect.y0 = LogY - FontSizeY;
  Rect.y1 = LogY + FontSizeY;
  if (LogX < LCD_GetXSize() / 2) {
    Rect.x0 = LogX + 15;
    Rect.x1 = LCD_GetXSize();
    Align = GUI_TA_LEFT;
  } else {
    Rect.x0 = 0;
    Rect.x1 = LogX - 15;
    Align = GUI_TA_RIGHT;
  }
  /* Show the text nearby the ring */
  GUI_DispStringInRect(acText, &Rect, Align | GUI_TA_TOP);
  GUI_DispStringInRect(pString, &Rect, Align | GUI_TA_BOTTOM);
  /* Draw the ring */
  GUI_FillCircle(LogX, LogY, 5);
  GUI_SetColor(GUI_WHITE);
  GUI_FillCircle(LogX, LogY, 2);
  GUI_SetColor(GUI_BLACK);
  /* Wait until touch is pressed */
  _WaitForPressedState(1);

   GUI_TOUCH_GetState(&State);
  *pPhysX = State.x;
  *pPhysY = State.y; 
  /* Wait until touch is released */
  _WaitForPressedState(0);
}

/********************************************************************
*
*       _Explain
*
* Purpose:
*   Shows a text to give a short explanation of the sample program
*/
static void _Explain(void) {
  _DispStringCentered("At first time, you need to\n"
                      "calibrate the Touch screen\n"
                      "Please press the touch\n"
                      "screen to continue...");
  GUI_DispStringHCenterAt("Touch screen Calibration", LCD_GetXSize() / 2, 5);
  _WaitForPressedState(1);
  _WaitForPressedState(0);
}
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void CALIBRATION_Check(void)
{

  int aPhysX[2], aPhysY[2], aLogX[2], aLogY[2], i;

  
  data1.d32 = BACKUP_RestoreParameter(RTC_BKP_DR0);
  data2.d32 = BACKUP_RestoreParameter(RTC_BKP_DR1);
  
  A2 = data2.b.A2;
  B2 = data2.b.B2;    
  A1 = data1.b.A1;
  B1 = data1.b.B1;
  
  if(data2.b.IsCalibrated == 0)
  {
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font13B_ASCII);
    _Explain();
    /* Set the logical values */
    aLogX[0] = 15;
    aLogY[0] = 15;
    aLogX[1] = LCD_GetXSize() - 15;
    aLogY[1] = LCD_GetYSize() - 15;
    /* Get the physical values of the AD converter for 2 positions */
    for (i = 0; i < 2; i++) {
      _GetPhysValues(aLogX[i], aLogY[i], &aPhysX[i], &aPhysY[i], _acPos[i]);
    }
    /* Use the physical values to calibrate the touch screen */
    
    A1 = (1000 * ( aLogX[1] - aLogX[0]))/ ( aPhysX[1] - aPhysX[0]); 
    B1 = (1000 * aLogX[0]) - A1 * aPhysX[0]; 
    
    A2 = (1000 * ( aLogY[1] - aLogY[0]))/ ( aPhysY[1] - aPhysY[0]); 
    B2 = (1000 * aLogY[0]) - A2 * aPhysY[0]; 
    
    data1.b.A1 = A1;
    data1.b.B1 = B1;
    BACKUP_SaveParameter(RTC_BKP_DR0, data1.d32);
    
    data2.b.A2 = A2;
    data2.b.B2 = B2;
    data2.b.IsCalibrated = 1;
    BACKUP_SaveParameter(RTC_BKP_DR1, data2.d32);  
    
    /* Display the result */
    GUI_CURSOR_Show();
    GUI_Clear();
    _DispStringCentered("Touch screen has been\n"
                        "calibrated. Please use\n"
                          "the cursor to test\n"
                            "the calibration...");
    

  }
  
  CALIBRATION_Done = 1;
  
  GUI_Delay(1000);
}

uint8_t CALIBRATION_IsDone(void)
{
  return CALIBRATION_Done;
}

uint16_t CALIBRATION_GetX(uint16_t x)
{
  return (((A1 * x) + B1)/1000);
}

uint16_t CALIBRATION_GetY(uint16_t y)
{
  return (((A2 * y) + B2)/1000);
}
/*************************** End of file ****************************/

