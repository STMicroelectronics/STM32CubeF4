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
  


#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"

#include <touchgfx/hal/HAL.hpp>
#include <STM32F4Instrumentation.hpp>

namespace touchgfx
{
static TIM_HandleTypeDef tim;

void STM32F4Instrumentation::init()
{
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock, uwAPB1Prescaler = 0U;
    uint32_t pFLatency;

    __TIM2_CLK_ENABLE();
    tim.Instance = TIM2;
    tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    tim.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim.Init.Period = 0xFFFFFFFF;
    tim.Init.Prescaler = 0;
    tim.Init.RepetitionCounter = 1;

    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* TIM2 is on APB1 bus */
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;

    if (uwAPB1Prescaler == RCC_HCLK_DIV1)
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    else
        uwTimclock = 2 * HAL_RCC_GetPCLK1Freq();

    m_sysclkRatio = HAL_RCC_GetHCLKFreq() / uwTimclock;

    HAL_TIM_Base_Init(&tim);
    HAL_TIM_Base_Start(&tim);
}

//Board specific clockfrequency
unsigned int STM32F4Instrumentation::getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency)
{
    return ((now - start) + (clockfrequency / 2)) / clockfrequency;
}

unsigned int STM32F4Instrumentation::getCPUCycles()
{
    return __HAL_TIM_GET_COUNTER(&tim) * m_sysclkRatio;
}

void STM32F4Instrumentation::setMCUActive(bool active)
{
    if (active) //idle task sched out
    {
        uint32_t current_cc = cc_in;
        cc_consumed += getCPUCycles() - current_cc;
    }
    else //idle task sched in
    {
        cc_in = getCPUCycles();
    }
}

}
