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
  


#ifndef DOTINDICATOR_HPP_
#define DOTINDICATOR_HPP_

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/ListLayout.hpp>

using namespace touchgfx;

class DotIndicator : public Container
{
public:
    DotIndicator();
    virtual ~DotIndicator();

    void setNumberOfDots(uint8_t size);
    void setBitmaps(const Bitmap& unselected, const Bitmap& selected);

    void goRight();
    void goLeft();
    void setHighlightPosition(uint8_t index);
private:
    static const uint8_t MAX_SIZE = 10;

    ListLayout unselectedDots;
    Image dotHighlighted;
    Image dotNormal[MAX_SIZE];

    uint8_t numberOfDots;
    uint8_t currentDot;
};

#endif /* DOTINDICATOR_HPP_ */
