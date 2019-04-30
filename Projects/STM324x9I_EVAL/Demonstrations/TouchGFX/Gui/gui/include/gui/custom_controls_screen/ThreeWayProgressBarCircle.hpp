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
  


#ifndef THREE_WAY_PROGRESS_BAR_CIRCLE_HPP
#define THREE_WAY_PROGRESS_BAR_CIRCLE_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

using namespace touchgfx;

class ThreeWayProgressBarCircle : public Container
{
public:
    ThreeWayProgressBarCircle();
    virtual ~ThreeWayProgressBarCircle();

    void setIcon(BitmapId newIconBmp, BitmapId newIconActiveBmp);
    void setActive(bool active);

    void setPercentage(int percentage);

private:
    BitmapId iconBmp;
    BitmapId iconActiveBmp;

    Image background;
    Image icon;
    TextAreaWithOneWildcard text;
    Unicode::UnicodeChar textBuffer[5];
};

#endif /* THREE_WAY_PROGRESS_BAR_CIRCLE_HPP */
