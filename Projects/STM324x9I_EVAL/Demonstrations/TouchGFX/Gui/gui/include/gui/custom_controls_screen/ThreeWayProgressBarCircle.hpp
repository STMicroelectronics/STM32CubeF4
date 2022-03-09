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
