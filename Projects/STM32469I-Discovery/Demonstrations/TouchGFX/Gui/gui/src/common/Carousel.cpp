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
  

#include <gui/common/Carousel.hpp>
#include <BitmapDatabase.hpp>

Carousel::Carousel() :
    currentAnimation(NO_ANIMATION),
    carouselAnimationDuration(10),
    currentNumberOfElements(0),
    carouselAnimationEndedAction(0),
    zoomAnimationCallback(this, &Carousel::handleZoomAnimationEvent)
{
    add(visibleElements[0]);
    add(visibleElements[6]);

    add(visibleElements[1]);
    add(visibleElements[5]);
    add(visibleElements[2]);
    add(visibleElements[4]);
    add(visibleElements[3]);

    visibleElements[0].setAnimationEndedCallback(zoomAnimationCallback);
    visibleElements[2].setAnimationEndedCallback(zoomAnimationCallback);
    visibleElements[6].setAnimationEndedCallback(zoomAnimationCallback);
}

Carousel::~Carousel()
{

}

void Carousel::setHighlightedNoAnimation()
{
    for (int i = 1; i < NUMBER_OF_VISIBLE_ELEMENTS-1; i++)
    {
        if (i != 3)
        {
            visibleElements[i].setPosition(visibleElements[i].getX() + visibleElements[i].getWidth()/2, visibleElements[i].getY() + visibleElements[i].getHeight()/2, 0, 0);
        }
    }
}

void Carousel::setPositionOfCarouselElement(uint8_t index, int16_t x, int16_t y, uint16_t width, uint16_t height)
{
    assert(index < NUMBER_OF_VISIBLE_ELEMENTS && "Illegal index of visible element");

    visibleElementPositions[index].x = x;
    visibleElementPositions[index].y = y;
    visibleElementPositions[index].width = width;
    visibleElementPositions[index].height = height;

    visibleElements[index].setPosition(x, y, width, height);
}

int16_t Carousel::getXPositionOfVisibleElement(uint8_t index)
{
    assert(index < NUMBER_OF_VISIBLE_ELEMENTS && "Illegal index of visible element");

    return visibleElementPositions[index].x;
}

int16_t Carousel::getYPositionOfVisibleElement(uint8_t index)
{
    assert(index < NUMBER_OF_VISIBLE_ELEMENTS && "Illegal index of visible element");

    return visibleElementPositions[index].y;
}

void Carousel::addElement(const Bitmap& smallBitmap, const Bitmap& mediumBitmap, const Bitmap& largeBitmap)
{
    elements[currentNumberOfElements].smallBmp = smallBitmap;
    elements[currentNumberOfElements].mediumBmp = mediumBitmap;
    elements[currentNumberOfElements].largeBmp = largeBitmap;
    currentNumberOfElements++;
}

void Carousel::setDuration(uint16_t duration)
{
    carouselAnimationDuration = duration;
}

void Carousel::setSelectedElement(uint8_t index)
{
    selectedElement = index;
    updateVisibleElements();
}

void Carousel::selectedNext()
{
    if (currentAnimation != NO_ANIMATION)
    {
        return;
    }
    currentAnimation = ANIMATE_LEFT;

    // Fade out/in elements
    visibleElements[1].startFadeAnimation(0, carouselAnimationDuration);
    visibleElements[6].startFadeAnimation(255, carouselAnimationDuration);

    for (int i = 1; i < NUMBER_OF_VISIBLE_ELEMENTS; i++)
    {
        // Initiate zoom and move of the elements
        visibleElements[i].startZoomAndMoveAnimation(visibleElementPositions[i-1].x, visibleElementPositions[i-1].y, visibleElementPositions[i-1].width, visibleElementPositions[i-1].height, carouselAnimationDuration, ZoomAnimationImage::FIXED_LEFT_AND_TOP);
    }

    // Make sure that the new middle element is in front of the other elements 
    remove(visibleElements[4]);
    insert(&visibleElements[3], visibleElements[4]);
}

void Carousel::selectedPrevious()
{
    if (currentAnimation != NO_ANIMATION)
    {
        return;
    }
    currentAnimation = ANIMATE_RIGHT;

    // Fade out/in elements
    visibleElements[0].startFadeAnimation(255, carouselAnimationDuration);
    visibleElements[5].startFadeAnimation(0, carouselAnimationDuration);

    for (int i = 0; i < NUMBER_OF_VISIBLE_ELEMENTS-1; i++)
    {
        // Initiate zoom and move of the elements
        visibleElements[i].startZoomAndMoveAnimation(visibleElementPositions[i+1].x, visibleElementPositions[i+1].y, visibleElementPositions[i+1].width, visibleElementPositions[i+1].height, carouselAnimationDuration, ZoomAnimationImage::FIXED_LEFT_AND_TOP, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone, EasingEquations::quadEaseIn);
    }

    // Make sure that the new middle element is in front of the other elements 
    remove(visibleElements[2]);
    insert(&visibleElements[3], visibleElements[2]);
}


void Carousel::highlightSelectedElement()
{
    if (currentAnimation != NO_ANIMATION)
    {
        return;
    }
    currentAnimation = ANIMATE_ELEMENT_SELECTED;

    for (int i = 1; i < NUMBER_OF_VISIBLE_ELEMENTS-1; i++)
    {
        if (i != 3)
        {
            visibleElements[i].startZoomAnimation(0, 0, carouselAnimationDuration, ZoomAnimationImage::FIXED_CENTER);
        }
    }
}

void Carousel::cancelHighlightOfSelectedElement()
{
    if (currentAnimation != NO_ANIMATION)
    {
        return;
    }
    currentAnimation = ANIMATE_ELEMENT_SELECTED;

    for (int i = 1; i < NUMBER_OF_VISIBLE_ELEMENTS-1; i++)
    {
        if (i != 3)
        {
            visibleElements[i].startZoomAnimation(visibleElementPositions[i].width, visibleElementPositions[i].height, carouselAnimationDuration, ZoomAnimationImage::FIXED_CENTER);
        }
    }
}

uint8_t Carousel::prevElement(uint8_t index)
{
    return (index == 0) ? currentNumberOfElements - 1 : index - 1;
}

uint8_t Carousel::nextElement(uint8_t index)
{
    return (index == currentNumberOfElements-1) ? 0 : index + 1;
}

void Carousel::handleZoomAnimationEvent(const ZoomAnimationImage& element)
{
    if (currentAnimation == ANIMATE_LEFT)
    {
        // Reset the order of the visible elements
        remove(visibleElements[4]);
        insert(&visibleElements[2], visibleElements[4]);

        setSelectedElement(nextElement(selectedElement));
    }
    else if (currentAnimation == ANIMATE_RIGHT)
    {
        // Reset the order of the visible elements
        remove(visibleElements[2]);
        insert(&visibleElements[5], visibleElements[2]);

        setSelectedElement(prevElement(selectedElement));
    }
    else if (currentAnimation == ANIMATE_ELEMENT_SELECTED)
    {
    }
    currentAnimation = NO_ANIMATION;

    if (carouselAnimationEndedAction && carouselAnimationEndedAction->isValid())
    {
        carouselAnimationEndedAction->execute(*this);
    }
}


void Carousel::updateVisibleElements()
{
    // Update the visibleElements to match the new selected element
    uint8_t currentElement = prevElement(prevElement(prevElement(selectedElement)));

    for (uint8_t i = 0; i < NUMBER_OF_VISIBLE_ELEMENTS; i++)
    {
        visibleElements[i].setBitmaps(elements[currentElement].smallBmp, elements[currentElement].mediumBmp, elements[currentElement].largeBmp);
        visibleElements[i].setPosition(visibleElementPositions[i].x, visibleElementPositions[i].y, visibleElementPositions[i].width, visibleElementPositions[i].height);
        visibleElements[i].setAlpha(255);
        visibleElements[i].invalidate();
        currentElement = nextElement(currentElement);
    }

    // The two elements in the end should be ready to be faded in when 
    // animation starts
    visibleElements[0].setAlpha(0);
    visibleElements[NUMBER_OF_VISIBLE_ELEMENTS-1].setAlpha(0);
}


