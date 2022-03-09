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
