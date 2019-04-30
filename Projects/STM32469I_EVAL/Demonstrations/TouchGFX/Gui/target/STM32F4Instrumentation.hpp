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
  


#ifndef STM32F4INSTRUMENTATION_HPP
#define STM32F4INSTRUMENTATION_HPP

#include <platform/core/MCUInstrumentation.hpp>
#include <stdint.h>

namespace touchgfx
{
class STM32F4Instrumentation: public MCUInstrumentation
{
public:
    virtual void init();
    virtual unsigned int getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency);
    virtual unsigned int getCPUCycles();
    virtual void setMCUActive(bool active);
private:
    uint32_t m_sysclkRatio;
};

} // namespace touchgfx

#endif // STM32F4INSTRUMENTATION_HPP
