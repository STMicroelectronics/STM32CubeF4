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
  


#ifndef DEMO_VIEW_INTERFACE_HPP
#define DEMO_VIEW_INTERFACE_HPP

#include <touchgfx/hal/Types.hpp>

using namespace touchgfx;

class DemoViewInterface
{
public:
    virtual ~DemoViewInterface() {}
    virtual void updateProcessorLoad(uint8_t mcuLoadInProcent) {}
};

#endif
