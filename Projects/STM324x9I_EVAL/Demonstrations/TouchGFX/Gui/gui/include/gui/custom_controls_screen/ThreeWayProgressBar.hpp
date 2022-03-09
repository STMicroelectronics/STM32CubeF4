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
  


#ifndef THREE_WAY_PROGRESS_BAR_HPP
#define THREE_WAY_PROGRESS_BAR_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <gui/custom_controls_screen/ThreeWayProgressBarCircle.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
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
#include <touchgfx/EasingEquations.hpp>

using namespace touchgfx;

class ThreeWayProgressBar : public Container
{
public:
    ThreeWayProgressBar();
    virtual ~ThreeWayProgressBar();

    void reset();
    void stopAnimation();

    void setBarPercentage(int barIndex, int percentage);

    void setActive(bool active);

    void setButtonClickedCallback(touchgfx::GenericCallback<const ThreeWayProgressBar&>& callback)
    {
        buttonClickedAction = &callback;
    }

private:
    static const int NUMBER_OF_BARS = 3;

    GenericCallback<const ThreeWayProgressBar&>* buttonClickedAction;

    int radius;

    Image background;
    ButtonWithLabel centerButton;

    MoveAnimator<ThreeWayProgressBarCircle> circles[NUMBER_OF_BARS];

    Circle bars[NUMBER_OF_BARS];
#if !defined(USE_BPP) || USE_BPP==16
    PainterRGB565Bitmap bitmapPainter;
#elif USE_BPP==24
    PainterRGB888Bitmap bitmapPainter;
#elif USE_BPP==4
    PainterGRAY4Bitmap bitmapPainter;
#elif USE_BPP==2
    PainterGRAY2Bitmap bitmapPainter;
#else
#error Unknown USE_BPP
#endif

    Callback<ThreeWayProgressBar, const AbstractButton& > buttonPressedCallback;
    void buttonPressedHandler(const AbstractButton& button);


};

#endif /* THREE_WAY_PROGRESS_BAR_HPP */
