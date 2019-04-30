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
