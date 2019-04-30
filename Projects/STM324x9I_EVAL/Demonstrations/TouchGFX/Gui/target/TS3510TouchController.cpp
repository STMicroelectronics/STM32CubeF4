/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
  


#include <TS3510TouchController.hpp>

extern "C"
{
#include "stm324x9i_eval_ts.h"

uint32_t LCD_GetXSize();
uint32_t LCD_GetYSize();
}

using namespace touchgfx;

void TS3510TouchController::init()
{
    BSP_TS_Init(LCD_GetXSize(), LCD_GetYSize());
}

bool TS3510TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    TS_StateTypeDef state;
    BSP_TS_GetState(&state);
    if (state.TouchDetected)
    {
        x = state.x;
        y = state.y;
        return true;
    }
    return false;
}
