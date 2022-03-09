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
