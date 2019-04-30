/**
  ******************************************************************************
  * @file    k_calibration.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel calibration functions   
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
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

