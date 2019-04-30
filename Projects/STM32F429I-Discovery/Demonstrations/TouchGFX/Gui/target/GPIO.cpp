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
  


#include <touchgfx/hal/GPIO.hpp>

#include "stm32f429i_discovery.h"

using namespace touchgfx;
            
static bool gpioState[4];

void GPIO::init()
{
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);

    for (int i = 0; i < 2; i++)
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
