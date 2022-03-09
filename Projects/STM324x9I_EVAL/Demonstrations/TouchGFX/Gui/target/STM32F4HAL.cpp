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
  


#include <touchgfx/hal/OSWrappers.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/hal/GPIO.hpp>

#include "stm32f4xx.h"
#include "stm32f4xx_hal_ltdc.h"

#include <STM32F4HAL.hpp>
#include <STM32F4DMA.hpp>

extern LTDC_HandleTypeDef hltdc;

uint16_t* STM32F4HAL::getTFTFrameBuffer() const
{
    return (uint16_t*)LTDC_Layer1->CFBAR;
}

void STM32F4HAL::setTFTFrameBuffer(uint16_t* adr)
{
    LTDC_Layer1->CFBAR = (uint32_t)adr;

    /* Reload immediate */
    LTDC->SRCR = (uint32_t)LTDC_SRCR_IMR;
}

uint16_t STM32F4HAL::getTFTCurrentLine()
{
    // This function only requires an implementation if single buffering
    // on LTDC display is being used (REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL).

    // The CPSR register (bits 15:0) specify current line of TFT controller.
    uint16_t curr = (uint16_t)(LTDC->CPSR & 0xffff);
    uint16_t backPorchY = (uint16_t)(LTDC->BPCR & 0x7FF) + 1;

    // The semantics of the getTFTCurrentLine() function is to return a value
    // in the range of 0-totalheight. If we are still in back porch area, return 0.
    if (curr < backPorchY)
    {
        return 0;
    }
    else
    {
        return curr - backPorchY;
    }
}

void STM32F4HAL::configureInterrupts()
{
    // These two priorities MUST be EQUAL, and MUST be functionally lower than RTOS scheduler interrupts.
    NVIC_SetPriority(DMA2D_IRQn, 9);
    NVIC_SetPriority(LTDC_IRQn, 9);
}

static uint16_t lcd_int_active_line;
static uint16_t lcd_int_porch_line;

/* Enable LCD line interrupt, when entering video (active) area */
void STM32F4HAL::enableLCDControllerInterrupt()
{
    lcd_int_active_line = (LTDC->BPCR & 0x7FF) - 1;
    lcd_int_porch_line = (LTDC->AWCR & 0x7FF) - 1;

    HAL_LTDC_ProgramLineEvent(&hltdc, lcd_int_active_line);
    __HAL_LTDC_ENABLE_IT(&hltdc, LTDC_IT_LI|LTDC_IT_FU); /* Enable line and FIFO underrun interrupts */
}

void STM32F4HAL::disableInterrupts()
{
    NVIC_DisableIRQ(LTDC_IRQn);
    NVIC_DisableIRQ(DMA2D_IRQn);
    NVIC_DisableIRQ(LTDC_ER_IRQn);
}

void STM32F4HAL::enableInterrupts()
{
    NVIC_EnableIRQ(LTDC_IRQn);
    NVIC_EnableIRQ(DMA2D_IRQn);
    NVIC_EnableIRQ(LTDC_ER_IRQn);
}

extern "C"
void HAL_LTDC_LineEvenCallback(LTDC_HandleTypeDef *hltdc)
{
    if (LTDC->LIPCR == lcd_int_active_line)
    {
        //entering active area
        HAL_LTDC_ProgramLineEvent(hltdc, lcd_int_porch_line);
        HAL::getInstance()->vSync();
        OSWrappers::signalVSync();
        // Swap frame buffers immediately instead of waiting for the task to be scheduled in.
        // Note: task will also swap when it wakes up, but that operation is guarded and will not have
        // any effect if already swapped.
        HAL::getInstance()->swapFrameBuffers();
        GPIO::set(GPIO::VSYNC_FREQ);
    }
    else
    {
        //exiting active area
        HAL_LTDC_ProgramLineEvent(hltdc, lcd_int_active_line);
        GPIO::clear(GPIO::VSYNC_FREQ);
        HAL::getInstance()->frontPorchEntered();
    }
}
