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
  


#ifndef JOG_WHEEL_HPP_
#define JOG_WHEEL_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <BitmapDatabase.hpp>

using namespace touchgfx;

class JogWheel : public Container
{
public:
    JogWheel(int16_t startValue_, int16_t deltaValueForOneRound_);
    virtual ~JogWheel();

    void setBitmap(Bitmap newBackgroundImage);
    void setBitmaps(Bitmap newBackgroundImage, Bitmap newBackgroundImageWhenDragged);

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

    void setValueUpdatedCallback(GenericCallback< int16_t >& callback)
    {
        valueChangedCallback = &callback;
    }
    void setEndDragEventCallback(GenericCallback< int16_t >& callback)
    {
        endDragEventCallback = &callback;
    }

    void setValue(int16_t value);
    int16_t getValue()
    {
        return currentValue;
    }

private:
    Image background;

    Bitmap backgroundImage;
    Bitmap backgroundImageWhenDragged;

    int32_t degreesRotated;
    int16_t startValue;
    int16_t deltaValueForOneRound;
    int16_t currentValue;

    bool firstDragEvent;
    int16_t centerX;
    int16_t centerY;
    int16_t radius;
    int16_t oldX;
    int16_t oldY;
    double oldLength;
    const uint16_t ANGLE_MULTIPLIER;

    GenericCallback< int16_t >* valueChangedCallback;
    GenericCallback< int16_t >* endDragEventCallback;
};


#endif /* JOG_WHEEL_HPP_ */
