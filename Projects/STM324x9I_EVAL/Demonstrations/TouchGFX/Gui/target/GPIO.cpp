/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
  *
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  


#include <touchgfx/hal/GPIO.hpp>

#include "stm324x9i_eval.h"
using namespace touchgfx;

static bool gpioState[4];

void GPIO::init()
{
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);
    
    for (int i = 0; i < 4; i++)
        clear((GPIO_ID)i);
}

void GPIO::set(GPIO_ID id)
{
    gpioState[id] = 1;
    if (id == VSYNC_FREQ)
    {
        BSP_LED_Off(LED3);
    }
    else if (id == RENDER_TIME)
    {
        BSP_LED_Off(LED4);
    }
}

void GPIO::clear(GPIO_ID id)
{
    gpioState[id] = 0;
    if (id == VSYNC_FREQ)
    {
        BSP_LED_On(LED3);
    }
    else if (id == RENDER_TIME)
    {
        BSP_LED_On(LED4);
    }
}

void GPIO::toggle(GPIO_ID id)
{
    if (get(id))
    {
        clear(id);
    }
    else
    {
        set(id);
    }
}

bool GPIO::get(GPIO_ID id)
{
    return gpioState[id];
}
