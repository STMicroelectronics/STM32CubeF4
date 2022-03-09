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
  

#ifndef CAROUSEL_HPP
#define CAROUSEL_HPP

#include <gui/common/ExtendedZoomAnimationImage.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>
#include <touchgfx/EasingEquations.hpp>

using namespace touchgfx;

/**
 * @class Carousel
 *
 * @brief This widget is part of the TouchGFX Open Repository.
 *        https://github.com/draupnergraphics/touchgfx-open-repository
 *        
 *        Carousel for visually navigating through a set of graphical elements. The current
 *        implementation displays five elements at a time. The carousel has one invisible
 *        element before and one after the visible elements. These are used when fading out the
 *        visible end elements. The visible elements are there for indexed from 1-5 and the
 *        invisible 0 and 6. The implementation can easily be extended to hold more visible
 *        elements. To set up the carousel call setPositionOfCarouselElement for all the
 *        elements including the end invisible elements. With this method you specify the
 *        position and sizes of the images in the carousel. The carousel will animate the
 *        elements between these position and sizes. You can add more elements than there are
 *        visible carousel elements. See MAX_NUMBER_OF_ELEMENTS. For performance reasons the
 *        elements uses an ExtendedZoomAnimationImage that holds three sizes of the images of
 *        the elements. So you have to supply each element in the carousel with three bitmaps
 *        of the element. One for each size of the visible elements. Use addElement(...) for
 *        this.
 *
 * @sa Container
 */
class Carousel : public Container
{
public:
    struct Bitmaps 
    {
        Bitmap smallBmp;
        Bitmap mediumBmp;
        Bitmap largeBmp;
    };

    /**
     * @fn Carousel::Carousel();
     *
     * @brief Default constructor.
     */
    Carousel();

    /**
     * @fn Carousel::~Carousel();
     *
     * @brief Destructor.
     */
    virtual ~Carousel();

    /**
     * @fn virtual void Carousel::setPositionOfCarouselElement(uint8_t index, int16_t x, int16_t y, uint16_t width, uint16_t height);
     *
     * @brief Setup a carousel graphical element. This is what defines the position and sizes of all
     *        the graphical elements in the carousel, so must be called for all visible elements
     *        and the two invisible end elements in the carousel.
     *
     * @param index  the index of the visible/invisible element to specify position and size for.
     *               Index 0 is the invisible element to the left. 1-5 is the visible elements from
     *               left to right. 6 is the index for the invisible element to the right.
     * @param x      the x coordinate for the visible/invisible element.
     * @param y      the y coordinate for the visible/invisible element.
     * @param width  the width for the visible/invisible element.
     * @param height the height for the visible/invisible element.
     */
    virtual void setPositionOfCarouselElement(uint8_t index, int16_t x, int16_t y, uint16_t width, uint16_t height);

    /**
     * @fn virtual void Carousel::addElement(const Bitmap& smallBitmap, const Bitmap& mediumBitmap, const Bitmap& largeBitmap);
     *
     * @brief Add an element to the carousel.
     *
     * @param smallBitmap  the small bitmap to associate with the next element.
     * @param mediumBitmap the medium bitmap to associate with the next element.
     * @param largeBitmap  the large bitmap to associate with the next element.
     */
    virtual void addElement(const Bitmap& smallBitmap, const Bitmap& mediumBitmap, const Bitmap& largeBitmap);

    /**
     * @fn virtual void Carousel::setDuration(uint16_t duration);
     *
     * @brief Set the durations of the carousel animations.
     *
     * @param duration the duration in ticks of the animations.
     */
    virtual void setDuration(uint16_t duration);

    /**
     * @fn virtual ExtendedZoomAnimationImage* Carousel::getSelectedElement() const
     *
     * @brief Gets selected element.
     *
     * @return the ExtendedZoomAnimationImage of selected element.
     */
    virtual ExtendedZoomAnimationImage* getSelectedElement() { return static_cast<ExtendedZoomAnimationImage*> (&(visibleElements[selectedElement])); }

    /**
     * @fn virtual uint8_t Carousel::getSelectedElementIndex() const
     *
     * @brief Gets selected element index.
     *
     * @return the index of the selected element.
     */
    virtual uint8_t getSelectedElementIndex() const { return selectedElement; }

    /**
     * @fn virtual Bitmaps Carousel::getSelectedElementBitmaps() const
     *
     * @brief Gets selected element bitmaps.
     *
     * @return the bitmap of the selected element.
     */
    virtual Bitmaps getSelectedElementBitmaps() const { return elements[selectedElement]; }

    /**
     * @fn virtual int16_t Carousel::getXPositionOfVisibleElement(uint8_t index);
     *
     * @brief Get x coordinate position of visible element.
     *
     * @param index index of the visible element in question.
     *
     * @return the x coordinate of the visible element with the specified index.
     */
    virtual int16_t getXPositionOfVisibleElement(uint8_t index);

    /**
     * @fn virtual int16_t Carousel::getYPositionOfVisibleElement(uint8_t index);
     *
     * @brief Get y coordinate position of visible element.
     *
     * @param index index of the visible element in question.
     *
     * @return the y coordinate of the visible element with the specified index.
     */
    virtual int16_t getYPositionOfVisibleElement(uint8_t index);

    /**
     * @fn virtual void Carousel::setSelectedElement(uint8_t index);
     *
     * @brief Set the selected element.
     *
     * @param index index of the element to be selected.
     */
    virtual void setSelectedElement(uint8_t index);

    /**
     * @fn virtual void Carousel::selectedNext();
     *
     * @brief Animates the carousel to the left. Changing the selected element accordingly.
     */
    virtual void selectedNext();

    /**
     * @fn virtual void Carousel::selectedPrevious();
     *
     * @brief Animates the carousel to the right. Changing the selected element accordingly.
     */
    virtual void selectedPrevious();

    /**
     * @fn virtual void Carousel::highlightSelectedElement();
     *
     * @brief Fade out all elements except the selected one.
     */
    virtual void highlightSelectedElement();

    /**
     * @fn virtual void Carousel::cancelHighlightOfSelectedElement();
     *
     * @brief Fade in all hidden elements.
     */
    virtual void cancelHighlightOfSelectedElement();

    /**
     * @fn void Carousel::setCarouselAnimationEndedCallback(touchgfx::GenericCallback<const Carousel&>& callback)
     *
     * @brief Associates an action to be performed when an animation ends.
     *
     * @param callback The callback to be executed. The callback will be given a reference to the
     *                 Carousel.
     *
     * @see GenericCallback
     */
    void setCarouselAnimationEndedCallback(touchgfx::GenericCallback<const Carousel&>& callback) { carouselAnimationEndedAction = &callback; }

    /**
     * @fn void Carousel::setHighlightedNoAnimation();
     *
     * @brief Sets highlighted with no animation or callback. 
     *        Method not part of the carousel from the TouchGFX Open Widget Repository. 
     */
    void setHighlightedNoAnimation();

protected:
    // Five visible elements and two elements ready to be faded in (one from the left and one from the right)
    static const uint8_t NUMBER_OF_VISIBLE_ELEMENTS = 5 + 2;
    static const uint8_t MAX_NUMBER_OF_ELEMENTS = 20;  

    enum Animation
    {
        ANIMATE_LEFT,
        ANIMATE_RIGHT,
        ANIMATE_ELEMENT_SELECTED,
        NO_ANIMATION
    };

    Animation currentAnimation;
    uint16_t carouselAnimationDuration;

    FadeAnimator<ExtendedZoomAnimationImage> visibleElements[NUMBER_OF_VISIBLE_ELEMENTS];
    Rect visibleElementPositions[NUMBER_OF_VISIBLE_ELEMENTS];
    Bitmaps elements[MAX_NUMBER_OF_ELEMENTS];


    uint8_t currentNumberOfElements;
    uint8_t selectedElement;

     /**
      * @fn virtual uint8_t Carousel::prevElement(uint8_t index);
      *
      * @brief Gets the previous element index.
      *
      * @param index index of the element that you want the previous element for.
      *
      * @return index of the previous element.
      */
     virtual uint8_t prevElement(uint8_t index);

    /**
     * @fn virtual uint8_t Carousel::nextElement(uint8_t index);
     *
     * @brief Gets the next element index.
     *
     * @param index index of the element that you want the next element for.
     *
     * @return index of the next element
     */
    virtual uint8_t nextElement(uint8_t index);

    touchgfx::GenericCallback<const Carousel&>* carouselAnimationEndedAction;   ///< The carousel animation ended callback

    Callback<Carousel, const ZoomAnimationImage&> zoomAnimationCallback;    ///< The zoom animation callback

    /**
     * @fn virtual void Carousel::handleZoomAnimationEvent(const ZoomAnimationImage& element);
     *
     * @brief Handles the zoom animation event described by element.
     *
     * @param element The element.
     */
    virtual void handleZoomAnimationEvent(const ZoomAnimationImage& element);

    /**
     * @fn virtual void Carousel::updateVisibleElements();
     *
     * @brief Reset the elements after an animation has ended. Set the elements back to their original
     *        position with the new image for that position.
     */
    virtual void updateVisibleElements();
};

#endif /* CAROUSEL_HPP */
