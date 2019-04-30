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
  


#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <touchgfx/hal/HAL.hpp>

#ifdef SIMULATOR
#include <ctime>
#ifndef _MSC_VER
#include <sys/time.h>
#endif /* _MSC_VER*/
#else

static volatile long lastUs;
extern uint32_t SystemCoreClock;
static int freqMHz;
#endif /* SIMULATOR */

Model::Model() :
    modelListener(0),
    mcuLoadActive(true),
    selectedMenuIndex(0)
{
#ifndef SIMULATOR
    lastUs = touchgfx::HAL::getInstance()->getCPUCycles();
    freqMHz = SystemCoreClock / 1000000;
#endif
}

#ifndef SIMULATOR
//extern volatile uint32_t mcu_load_pct;
static uint8_t mcuLoadLast = 0;
#endif // SIMULATOR

void Model::tick()
{
    Time previousTime = currentTime;

#ifdef SIMULATOR
#ifdef _MSC_VER
    time_t rawtime;
    struct tm timenow;
    time(&rawtime);
    localtime_s(&timenow, &rawtime);

    currentTime.hours =   timenow.tm_hour;
    currentTime.minutes = timenow.tm_min;
    currentTime.seconds = timenow.tm_sec;
    currentTime.milliseconds = 0;

#else
    timeval timenow;
    gettimeofday(&timenow, NULL);

    currentTime.hours = (timenow.tv_sec / 60 / 60) % 24;
    currentTime.minutes = (timenow.tv_sec / 60) % 60;
    currentTime.seconds = timenow.tv_sec % 60;
    currentTime.milliseconds = timenow.tv_usec / 1000;
#endif  /*_MSC_VER*/
#else
    static int milliseconds = 123456;
    uint8_t mcuLoadPct = touchgfx::HAL::getInstance()->getMCULoadPct();
    if (mcuLoadLast != /*mcu_load_pct*/ mcuLoadPct)
    {
        mcuLoadLast = mcuLoadPct;
        modelListener->mcuLoadUpdated(mcuLoadLast);
    }

    //long now = cpu_cycles();
    long now = touchgfx::HAL::getInstance()->getCPUCycles();
    milliseconds += (now - lastUs + freqMHz / 2) / freqMHz / 1000;
    lastUs = now;
    currentTime.hours = (milliseconds / 1000 / 60 / 60) % 24;
    currentTime.minutes = (milliseconds / 1000 / 60) % 60;
    currentTime.seconds = (milliseconds / 1000) % 60;
    currentTime.milliseconds = milliseconds % 1000;

#endif /* SIMULATOR */

    if (currentTime.seconds != previousTime.seconds)
    {
        if (modelListener)
        {
            modelListener->timeUpdated(currentTime);
        }
    }
}

void Model::screenSaverMinorTick()
{
    modelListener->screenSaverMinorTick();
}

void Model::screenSaverMajorTick()
{
    modelListener->screenSaverMajorTick();
}

