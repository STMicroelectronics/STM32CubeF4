  /**
  ******************************************************************************
  * @file    USB_Host/HID_Standalone/Src/ts_calibration.c
  * @author  MCD Application Team
  * @brief   This example code shows how to calibrate the touchscreen.
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
#include "ts_calibration.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t ts_calibration_done = 0;
static int16_t aPhysX[2], aPhysY[2], aLogX[2], aLogY[2];
/* Imported variables */
extern TS_StateTypeDef TS_State;
#define CALIB_RES(val,pte1,ptm1,pte2,ptm2) ((((val-ptm2)*(pte1-pte2))/(ptm1-ptm2))+pte2)
/* Private function prototypes -----------------------------------------------*/
static void TouchscreenCalibration_SetHint(void);
static void TouchScreen_Calibration_GetPhysValues(int16_t LogX, int16_t LogY, int16_t * pPhysX, int16_t * pPhysY) ;
static void TouchScreen_Calibration_WaitForPressedState(uint8_t Pressed) ;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Performs the TS calibration
  * @param  None
  * @retval Status (TS_OK = 0/ TS_ERROR = 1 / TS_TIMEOUT = 1 / TS_DEVICE_NOT_FOUND = 3)
  */
uint8_t Touchscreen_Calibration(void)
{
  uint8_t ts_status = TS_OK;
  uint8_t i;
    
  TouchscreenCalibration_SetHint();
  
  /* Start touchscreen internal calibration and configuration + start */
  ts_status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
  if (ts_status != TS_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)"Touchscreen cannot be calibrated", CENTER_MODE);
    if(ts_status == TS_ERROR)
    {
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"Touchscreen undefined error", CENTER_MODE);
    }
    else if(ts_status == TS_TIMEOUT)
    {
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"Touchscreen Timeout", CENTER_MODE);
    }
    else
    {
      /* TS_DEVICE_NOT_FOUND */
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"Touchscreen Not Found", CENTER_MODE);
    }
  }
  else
  {
    /* status == TS_OK */
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"FT6x06 internal calibration passed", CENTER_MODE);
    
    /* Get touch points for SW calibration processing */
    aLogX[0] = 20;
    aLogY[0] = 20;
    aLogX[1] = BSP_LCD_GetXSize() - 20;
    aLogY[1] = BSP_LCD_GetYSize() - 20;
    
    for (i = 0; i < 2; i++)
    {
      TouchScreen_Calibration_GetPhysValues(aLogX[i], aLogY[i], &aPhysX[i], &aPhysY[i]);
    }
   
    ts_calibration_done = 1;
  }
  
  return (ts_status);
}

/**
  * @brief  Display calibration hint
  * @param  None
  * @retval None
  */
static void TouchscreenCalibration_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set Touchscreen Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

  BSP_LCD_SetFont(&Font20);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 - 35, (uint8_t *)"Before using the Touchscreen", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 - 15, (uint8_t *)"you need to calibrate it.", CENTER_MODE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 7, (uint8_t *)"WAIT until the black circle appears", CENTER_MODE);
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 30, (uint8_t *)"THEN Press precisely on the black circles", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
}


/**
  * @brief  Get Physical position
  * @param  LogX : logical X position
  * @param  LogY : logical Y position
  * @param  pPhysX : Physical X position
  * @param  pPhysY : Physical Y position
  * @retval None
  */
static void TouchScreen_Calibration_GetPhysValues(int16_t LogX, int16_t LogY, int16_t * pPhysX, int16_t * pPhysY)
{
  /* Draw the ring */

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillCircle(LogX, LogY, 20);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillCircle(LogX, LogY, 10);

  /* Wait until pressed state on the touch panel */
  TouchScreen_Calibration_WaitForPressedState(1);

  /* Return as physical touch values the positions of first touch, even if double touched occurred */
  *pPhysX = TS_State.touchX[0];
  *pPhysY = TS_State.touchY[0];

  /* Wait until touch is released on touch panel */
  TouchScreen_Calibration_WaitForPressedState(0);

  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillCircle(LogX, LogY, 20);
}

/**
  * @brief  TouchScreen_Calibration_WaitForPressedState : wait until a particular press/depress action
  *         The function is managing anti-rebound : that is the awaited state when detected
  *         needs to be stable for a sufficient time (timeout time), otherwise a new sense to search
  *         for awaited state is performed. When awaited state is found and state is stable for timeout
  *         duration, the function is exited.
  * @param  uint8_t Pressed :  Awaited pressed state
  *         - Await touch (single/multiple) detection if Pressed == 1
  *         - Await no touch detection if Pressed == 0
  * @retval None
  */
static void TouchScreen_Calibration_WaitForPressedState(uint8_t Pressed)
{
  uint16_t TimeStart = 0;
  uint8_t  status = TS_OK;
  uint32_t exitFirstLevelWhileLoopReq = 0;  /* By default no exit request from first level while loop  */
  uint32_t exitSecondLevelWhileLoopReq = 0; /* By default no exit request from second level while loop */

  /* First level while loop entry */
  do
  {
    /* reset exit second level while loop in case it was set */
    exitSecondLevelWhileLoopReq = 0;

    /* Sense of touch state from touch IC until get the awaited state in parameter 'Pressed' */
    status = BSP_TS_GetState(&TS_State);
    if(status == TS_OK)
    {
      if (((Pressed == 0) && (TS_State.touchDetected == 0)) ||
          ((Pressed == 1) && ((TS_State.touchDetected == 1) || (TS_State.touchDetected == 2))))
      {
        /* Got awaited press state */
        /* Record in 'TimeStart' the time of awaited touch event for anti-rebound calculation */
        /* The state should persist for a minimum sufficient time */
        TimeStart = HAL_GetTick();

        /* Is state of the touch changing ? */
        /* Second level while loop entry */
        do
        {
          /* New sense of touch state from touch IC : to evaluate if state was stable */
          status = BSP_TS_GetState(&TS_State);
          if(status == TS_OK)
          {
            /* Is there a state change compared since having found the awaited state ? */
            if (((Pressed == 0) && ((TS_State.touchDetected == 1) || (TS_State.touchDetected == 2))) ||
                ((Pressed == 1) && ((TS_State.touchDetected == 0))))
            {
              /* Too rapid state change => anti-rebound management : restart first touch search */
              exitSecondLevelWhileLoopReq = 1; /* exit request from second level while loop */
            }
            else if ((HAL_GetTick() - 100) > TimeStart)
            {
              /* State have not changed for the timeout duration (stable touch for 100 ms) */
              /* This means the touch state is stable : can exit function */

              /* found valid touch, exit both while levels */
              exitSecondLevelWhileLoopReq = 1;
              exitFirstLevelWhileLoopReq  = 1;
            }

            /* Wait 10 ms before next sense of touch at next loop iteration */
            HAL_Delay(10);

          } /* of if(status == TS_OK) */
        }
        while (!exitSecondLevelWhileLoopReq);

      } /* of if (((Pressed == 0) && .... */

    } /* of if(status == TS_OK) */

    if(!exitFirstLevelWhileLoopReq)
    {
      /* Wait some time before next sense of touch at next loop iteration */
      HAL_Delay(10);
    }
  }
  while (!exitSecondLevelWhileLoopReq);
}

/**
  * @brief  Calibrate x position (to obtain X = calibrated(x))
  * @param  x : X position
  * @retval calibrated x
  */
uint16_t TouchScreen_Get_Calibrated_X(uint16_t x)
{
   int32_t ret=CALIB_RES(x,aLogX[0],aPhysX[0],aLogX[1],aPhysX[1]);
   if (ret<0) ret=0;
   else if  (ret>240) ret=239;
   return ret;
}

/**
  * @brief  Calibrate Y position
  * @param  y : Y position
  * @retval calibrated y
  */
uint16_t TouchScreen_Get_Calibrated_Y(uint16_t y)
{
   int32_t ret=CALIB_RES(y,aLogY[0],aPhysY[0],aLogY[1],aPhysY[1]);
   if (ret<0) ret=0;
   else if  (ret>240) ret=239;
   return ret;
}

/** @Brief  Check if the TS is calibrated
  * @param  None
  * @retval Calibration state (1 : calibrated / 0: no)
  */
uint8_t TouchScreen_IsCalibrationDone(void)
{
  return (ts_calibration_done);
}
