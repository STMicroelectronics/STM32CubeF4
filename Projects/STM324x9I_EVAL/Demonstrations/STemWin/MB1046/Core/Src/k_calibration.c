/**
  ******************************************************************************
  * @file    k_calibration.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel calibration functions   
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
#include "k_calibration.h"
#include "k_rtc.h"
#include "GUI.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_CALIBRATION
  * @brief Kernel calibration routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
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

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint32_t CALIBRATION_Done = 0;

static const char * _acPos[] = {
  "(upper left position)",
  "(lower right position)"
};

int16_t  A1, A2, B1, B2;
CALIBRATION_Data1Typedef data1;
CALIBRATION_Data2Typedef data2;

/**
  * @brief  wait for process state
  * @param  Pressed: touch status
  * @retval None
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

/**
  * @brief  Shows the given text horizontally and vertically centered
  * @param  pString: pointer to the text
  * @retval None
  */
static void _DispStringCentered(const char * pString) {
  GUI_RECT Rect;
  Rect.x0 = Rect.y0 = 0;
  Rect.x1 = LCD_GetXSize() - 1;
  Rect.y1 = LCD_GetYSize() - 1;
  GUI_DispStringInRect(pString, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
}

/**
  * @brief  Asks the user to press the touch screen at the given position
  *         and returns the physical A/D values
  * @param  LogX:    logical X position
  * @param  LogY:    logical Y position
  * @param  pPhysX:  Physical X position
  * @param  pPhysY:  Physical Y position
  * @param  pString: pointer to string
  * @retval None
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

/**
  * @brief  Shows text to give short explanation of the sample program
  * @param  None.
  * @retval None
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

/**
  * @brief  Initialization of calibration
  * @param  None.
  * @retval None
  */
void k_CalibrationInit(void)
{
  int aPhysX[2], aPhysY[2], aLogX[2], aLogY[2], i;

  data1.d32 = k_BkupRestoreParameter(RTC_BKP_DR0);
  data2.d32 = k_BkupRestoreParameter(RTC_BKP_DR1);
  
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
    k_BkupSaveParameter(RTC_BKP_DR0, data1.d32);
    
    data2.b.A2 = A2;
    data2.b.B2 = B2;
    data2.b.IsCalibrated = 1;
    k_BkupSaveParameter(RTC_BKP_DR1, data2.d32);  
    
    /* Display the result */
    GUI_Clear();
    _DispStringCentered("Touch screen has been\n"
                        "calibrated. Please use\n"
                          "the cursor to test\n"
                            "the calibration...");
    

  }
  
  CALIBRATION_Done = 1;
  
  GUI_Delay(1000);
}

uint8_t k_CalibrationIsDone(void)
{
  return CALIBRATION_Done;
}

uint16_t k_CalibrationGetX(uint16_t x)
{
  return (((A1 * x) + B1)/1000);
}

uint16_t k_CalibrationGetY(uint16_t y)
{
  return (((A2 * y) + B2)/1000);
}

/**
  * @}
  */

/**
  * @}
  */
