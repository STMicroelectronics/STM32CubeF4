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
  


#include <gui/custom_controls_screen/VerticalSlideMenu.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/EasingEquations.hpp>

VerticalSlideMenu::VerticalSlideMenu() :
    size(0),
    animationDuration(10),
    animationEndedCallback(this, &VerticalSlideMenu::animationEndedHandler)
{
}

void VerticalSlideMenu::setup(int numberOfElements, int selectedElementIndex, Bitmap smallBmp, Bitmap largeBmp)
{
    assert(selectedElementIndex < numberOfElements);
    assert(numberOfElements <= MAX_SIZE);

    size = numberOfElements;
    imagesYPositionDelta = 16;

    smallImageWidth = smallBmp.getWidth();
    smallImageHeight = smallBmp.getHeight();
    largeImageWidth = largeBmp.getWidth();
    largeImageHeight = largeBmp.getHeight();
    largeImageX = (getWidth() - largeImageWidth) / 2;
    largeImageY = (getHeight() - largeImageHeight) / 2;
    smallImageX = (getWidth() - smallImageWidth) / 2;
    fixedCenterCorrection = (largeImageHeight - smallImageHeight) / 2;

    currentSelected = selectedElementIndex;

    images[currentSelected].setWidth(largeImageWidth);
    images[currentSelected].setHeight(largeImageHeight);
    images[currentSelected].setXY(largeImageX, largeImageY);
    images[currentSelected].setAnimationEndedCallback(animationEndedCallback);
    add(images[currentSelected]);

    for (int i = currentSelected - 1; i >= 0; i--)
    {
        images[i].setWidth(smallImageWidth);
        images[i].setHeight(smallImageHeight);

        images[i].setXY(smallImageX, images[i + 1].getY() - imagesYPositionDelta - images[i].getHeight());

        images[i].setAnimationEndedCallback(animationEndedCallback);
        add(images[i]);
    }

    for (int i = currentSelected + 1; i < size; i++)
    {
        images[i].setWidth(smallImageWidth);
        images[i].setHeight(smallImageHeight);

        images[i].setXY(smallImageX, images[i - 1].getY() + images[i - 1].getHeight() + imagesYPositionDelta);

        images[i].setAnimationEndedCallback(animationEndedCallback);
        add(images[i]);
    }
}

VerticalSlideMenu::~VerticalSlideMenu()
{
}

void VerticalSlideMenu::setBitmapsForElement(int elementIndex, BitmapId smallBmp, BitmapId largeBmp)
{
    // Make sure that the dimensions of the bitmap stays the same
    int oldWidth = images[elementIndex].getWidth();
    int oldHeight = images[elementIndex].getHeight();

    images[elementIndex].setBitmaps(Bitmap(smallBmp), Bitmap(largeBmp));

    images[elementIndex].setWidth(oldWidth);
    images[elementIndex].setHeight(oldHeight);
}

void VerticalSlideMenu::animateUp()
{
    if (animating() || currentSelected == size - 1)
    {
        return;
    }

    for (int i = 0; i < size; i++)
    {
        // If image is not visible and will not be after animation just move it
        if (images[i].getY() + images[i].getHeight() < 0)
        {
            images[i].moveTo(images[i].getX(), images[i].getY() - images[i].getHeight() - imagesYPositionDelta);
        }
        // If image is not visible and will not be after animation just move it
        else if (images[i].getY() - images[i].getHeight() - imagesYPositionDelta > getHeight())
        {
            images[i].moveTo(images[i].getX(), images[i].getY() - images[i].getHeight() - imagesYPositionDelta);
        }
        else if (i == currentSelected + 1)
        {
            images[i].startZoomAndMoveAnimation(images[i].getX(), images[i].getY() - largeImageHeight - imagesYPositionDelta + fixedCenterCorrection, largeImageWidth, largeImageHeight, animationDuration,
                                                ZoomAnimationImage::FIXED_CENTER, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone);
        }
        else if (i == currentSelected)
        {
            images[i].startZoomAndMoveAnimation(images[i].getX(), images[i].getY() - smallImageHeight - imagesYPositionDelta - fixedCenterCorrection, smallImageWidth, smallImageHeight, animationDuration,
                                                ZoomAnimationImage::FIXED_CENTER, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone);
        }
        else
        {
            images[i].startZoomAndMoveAnimation(images[i].getX(), images[i].getY() - smallImageHeight - imagesYPositionDelta, smallImageWidth, smallImageHeight, animationDuration,
                                                ZoomAnimationImage::FIXED_CENTER, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone);
        }
    }
    currentSelected++;
}

void VerticalSlideMenu::animateDown()
{
    if (animating() || currentSelected == 0)
    {
        return;
    }

    for (int i = 0; i < size; i++)
    {
        // If image is not visible and will not be after animation just move it
        if (images[i].getY() > getHeight())
        {
            images[i].moveTo(images[i].getX(), images[i].getY() + images[i].getHeight() + imagesYPositionDelta);
        }
        // If image is not visible and will not be after animation just move it
        else if (images[i].getY() + 2 * images[i].getHeight() + imagesYPositionDelta < 0)
        {
            images[i].moveTo(images[i].getX(), images[i].getY() + images[i].getHeight() + imagesYPositionDelta);
        }
        else if (i == currentSelected - 1)
        {
            images[i].startZoomAndMoveAnimation(images[i].getX(), images[i].getY() + largeImageHeight + imagesYPositionDelta - fixedCenterCorrection, Bitmap(BITMAP_CONTROL_MENU_ICON_DATEPICKER_LARGE_ID).getWidth(), Bitmap(BITMAP_CONTROL_MENU_ICON_DATEPICKER_LARGE_ID).getHeight(), animationDuration,
                                                ZoomAnimationImage::FIXED_CENTER, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone);
        }
        else if (i == currentSelected)
        {
            images[i].startZoomAndMoveAnimation(images[i].getX(), images[i].getY() + smallImageHeight + imagesYPositionDelta  + fixedCenterCorrection, Bitmap(BITMAP_CONTROL_MENU_ICON_DATEPICKER_SMALL_ID).getWidth(), Bitmap(BITMAP_CONTROL_MENU_ICON_DATEPICKER_SMALL_ID).getHeight(), animationDuration,
                                                ZoomAnimationImage::FIXED_CENTER, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone);
        }
        else
        {
            images[i].startZoomAndMoveAnimation(images[i].getX(), images[i].getY() + smallImageHeight + imagesYPositionDelta, images[i].getWidth(), images[i].getHeight(), animationDuration,
                                                ZoomAnimationImage::FIXED_CENTER, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone);
        }
    }
    currentSelected--;
}

void VerticalSlideMenu::animationEndedHandler(const ZoomAnimationImage& image)
{
    if (elementSelectedAction && elementSelectedAction->isValid())
    {
        elementSelectedAction->execute(*this);
    }

}

bool VerticalSlideMenu::animating()
{
    for (int i = 0; i < size; i++)
    {
        if (images[i].isRunning())
        {
            return true;
        }
    }
    return false;
}
