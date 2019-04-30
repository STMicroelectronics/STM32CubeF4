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
  


#include <touchgfx/hal/HAL.hpp>

#include <touchgfx/hal/BoardConfiguration.hpp>
#include <common/TouchGFXInit.hpp>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

using namespace touchgfx;

static void GUITask(void* params)
{
    touchgfx::HAL::getInstance()->taskEntry();
}

int main(void)
{
    hw_init();
    touchgfx_init();

    xTaskCreate(GUITask,
                "GUITask",
                1500,
                NULL,
                tskIDLE_PRIORITY + 1,
                NULL);

    vTaskStartScheduler();

    for (;;);

}
