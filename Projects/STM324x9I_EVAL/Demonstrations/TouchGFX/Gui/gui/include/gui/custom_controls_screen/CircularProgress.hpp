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
#if !defined(USE_BPP) || USE_BPP==16
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565Bitmap.hpp>
#elif USE_BPP==24
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888Bitmap.hpp>
#elif USE_BPP==4
#include <touchgfx/widgets/canvas/PainterGRAY4.hpp>
#include <touchgfx/widgets/canvas/PainterGRAY4Bitmap.hpp>
#elif USE_BPP==2
#include <touchgfx/widgets/canvas/PainterGRAY2.hpp>
#include <touchgfx/widgets/canvas/PainterGRAY2Bitmap.hpp>
#else
#error Unknown USE_BPP
#endif
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

    int getValue()
    {
        return currentPercentage;
    }

    void setBarAngle(int angleInDegrees);

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

protected:
    static const int END_DEGREE = 116;
    static const int START_DEGREE = -END_DEGREE;

    Image background;
    Image centerImage;

    Circle bar;
#if !defined(USE_BPP) || USE_BPP==16
    PainterRGB565Bitmap bitmapPainter;
    PainterRGB565 colorPainter;
#elif USE_BPP==24
    PainterRGB888Bitmap bitmapPainter;
    PainterRGB888 colorPainter;
#elif USE_BPP==4
    PainterGRAY4Bitmap bitmapPainter;
    PainterGRAY4 colorPainter;
#elif USE_BPP==2
    PainterGRAY2Bitmap bitmapPainter;
    PainterGRAY2 colorPainter;
#else
#error Unknown USE_BPP
#endif

    TextAreaWithOneWildcard percentageText;
    Unicode::UnicodeChar percentageTextBuffer[5];

    TextArea headlineText;

    int currentPercentage;

    void updateBar(int newX, int newY);
};


#endif /* CIRCULARPROGRESS_HPP */
