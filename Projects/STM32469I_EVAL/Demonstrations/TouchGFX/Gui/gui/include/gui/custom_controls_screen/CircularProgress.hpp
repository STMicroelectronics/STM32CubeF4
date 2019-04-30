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
  

#ifndef CIRCULARPROGRESS_HPP
#define CIRCULARPROGRESS_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565Bitmap.hpp>
#include <BitmapDatabase.hpp>

using namespace touchgfx;

/**
 * Based on CircularProgress from github repository TOR.
 * https://github.com/draupnergraphics/touchgfx-open-repository/tree/master/widgets/CircularProgress
 */
class CircularProgress : public Container
{
public:

    CircularProgress();

    virtual ~CircularProgress();

    int getValue() { return currentPercentage; }

    void setBarAngle(int angleInDegrees);

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

protected:
    static const int END_DEGREE = 116;
    static const int START_DEGREE = -END_DEGREE;

    Image background;
    Image centerImage;

    Circle bar;
    PainterRGB565Bitmap bitmapPainter;
    PainterRGB565 colorPainter;

    TextAreaWithOneWildcard percentageText;
    Unicode::UnicodeChar percentageTextBuffer[5];

    TextArea headlineText;

    int currentPercentage;

    void updateBar(int newX, int newY);
};


#endif /* CIRCULARPROGRESS_HPP */
