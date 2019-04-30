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
  


volatile bool overrideBlitcaps = false;

#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>

using namespace touchgfx; 

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/**
 * Define the FreeRTOS task priorities and stack sizes
 */
#define configGUI_TASK_PRIORITY                 ( tskIDLE_PRIORITY + 3 )

#define configGUI_TASK_STK_SIZE                 ( 1500 )

#define CANVAS_BUFFER_SIZE (8 * 1024)

static void GUITask(void* params)
{
  touchgfx::HAL::getInstance()->taskEntry();
}

static uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];

int main (void)
{
  hw_init();
  touchgfx_init();

  CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);

  xTaskCreate( GUITask, "GUITask",
               configGUI_TASK_STK_SIZE,
               NULL,
               configGUI_TASK_PRIORITY,
               NULL);

  vTaskStartScheduler();

  for(;;);

}

