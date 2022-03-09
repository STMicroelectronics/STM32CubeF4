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
  


#ifndef OTM8009TOUCHCONTROLLER_HPP
#define OTM8009TOUCHCONTROLLER_HPP

#include <platform/driver/touch/TouchController.hpp>

class OTM8009TouchController : public touchgfx::TouchController
{
public:
    OTM8009TouchController() {}
    virtual ~OTM8009TouchController() {}
    virtual void init();
    virtual bool sampleTouch(int32_t& x, int32_t& y);
};

#endif // OTM8009TOUCHCONTROLLER_HPP
