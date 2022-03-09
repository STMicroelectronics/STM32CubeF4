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
  


#ifndef SWIPE_CONTAINER_HPP
#define SWIPE_CONTAINER_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <gui/common/DotIndicator.hpp>

/**
 * @class SwipeContainer
 *
 * A container that holds elements that be swiped through to the left or right.
 * The container will animate to the next element or snaps back to the shown
 * element if a too small drag is performed.
 *
 * All elements added must have the same dimensions.
 *
 * This widget is part of the TouchGFX Open Widget Repository.
 * https://github.com/draupnergraphics/touchgfx-widgets
 * Widget has been slightly modified for this demo.
 *
 * @see touchgfx::Container
 */
class SwipeContainer : public touchgfx::Container
{
public:
    SwipeContainer();
    virtual ~SwipeContainer();

    virtual void handleTickEvent();
    virtual void handleClickEvent(const touchgfx::ClickEvent& evt);
    virtual void handleDragEvent(const touchgfx::DragEvent& evt);
    virtual void handleGestureEvent(const touchgfx::GestureEvent& evt);

    /**
     * @fn virtual void SwipeContainer::add(touchgfx::Drawable& screen);
     *
     * @brief Adds a screen to the container.
     *        Note that all screens must have the same width and height.
     *
     * @param screen The screen to add.
     */
    virtual void add(touchgfx::Drawable& screen);

    /**
     * @fn virtual void SwipeContainer::setSwipeCutoff(uint16_t cutoff);
     *
     * @brief Set the swipe cutoff which indicates how far you should drag
     *        a screen before it results in a screen change
     *
     * @param cutoff The cutoff in pixels.
     */
    virtual void setSwipeCutoff(uint16_t cutoff);

    /**
     * @fn void SwipeContainer::setDotIndicatorXY(uint16_t x, uint16_t y);
     *
     * @brief Sets the x and y position of the dot indicator.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    void setDotIndicatorXY(uint16_t x, uint16_t y);

    /**
     * @fn void SwipeContainer::setDotIndicatorXYWithCenteredX(uint16_t x, uint16_t y);
     *
     * @brief Sets the x and y position of the dot indicator. The value specified as x
     *        will be the center coordinate of the dot indicators.
     *        Note that this method should first be used after all screens have been added, the
     *        setDotIndicatorBitmaps has been called and the dot indicator therefore has the correct width.
     *
     * @param x The center x coordinate.
     * @param y The y coordinate.
     */
    void setDotIndicatorXYWithCenteredX(uint16_t x, uint16_t y);

    /**
     * @fn void SwipeContainer::setDotIndicatorBitmaps(const touchgfx::Bitmap& normalDot, const touchgfx::Bitmap& highlightedDot);
     *
     * @brief Sets the bitmaps that are used by the dot indicator.
     *
     * @param normalDot      The normal dot.
     * @param highlightedDot The highlighted dot.
     */
    void setDotIndicatorBitmaps(const touchgfx::Bitmap& normalDot, const touchgfx::Bitmap& highlightedDot);

    /**
     * @fn void SwipeContainer::setEndSwipeElasticWidth(uint16_t width);
     *
     * @brief When dragging either one of the end screens a part of the
     *        background will become visible until the user stop dragging
     *        and the end screen swipes back to its position. The width of
     *        this area is set by this method.
     *
     *
     * @param width The width in pixels.
     */
    void setEndSwipeElasticWidth(uint16_t width);

    /**
     * @fn uint8_t SwipeContainer::getNumberOfScreens()
     *
     * @brief Gets number of screens.
     *
     * @return The number of screens.
     */
    uint8_t getNumberOfScreens()
    {
        return numberOfScreens;
    }

    /**
     * @fn void SwipeContainer::setSelectedScreen(uint8_t screenIndex);
     *
     * @brief Sets the selected screen.
     *
     * @param screenIndex Zero-based index of the screen. Range from 0 to numberOfScreens-1.
     */
    void setSelectedScreen(uint8_t screenIndex);

    uint8_t getSelectedScreen()
    {
        return currentScreen;
    }


    /**
     * @fn void CollapsibleMenu::setStateChangedCallback(touchgfx::GenericCallback<const CollapsibleMenu&, const bool>& callback)
     *
     * @brief Associates an action to be performed when a transition to a different screen initiates.
     *
     * @param callback The callback to be executed.
     *
     * @see GenericCallback
     */
    void setMenuChangeInitiatedCallback(touchgfx::GenericCallback<const SwipeContainer&>& callback)
    {
        menuChangeInitiatedAction = &callback;
    }

    /**
     * @fn void SwipeContainer::setMenuChangeEndedCallback(touchgfx::GenericCallback<const SwipeContainer&>& callback)
     *
     * @brief Associates an action to be performed when a transition to a different screen ends.
     *
     * @param callback The callback to be executed.
     *
     * @see GenericCallback
     */
    void setMenuChangeEndedCallback(touchgfx::GenericCallback<const SwipeContainer&>& callback)
    {
        menuChangeEndedAction = &callback;
    }

private:

    enum States
    {
        ANIMATE_SWIPE_CANCELLED_LEFT,
        ANIMATE_SWIPE_CANCELLED_RIGHT,
        ANIMATE_LEFT,
        ANIMATE_RIGHT,
        NO_ANIMATION
    } currentState;

    uint8_t numberOfScreens;

    uint8_t animationCounter;

    uint16_t swipeCutoff;

    int16_t dragX;
    int16_t animateDistance;
    int16_t startX;
    uint8_t currentScreen;

    // Background that becomes visible when trying to swipe
    // beyond the number of screens.
    uint16_t endElasticWidth;

    DotIndicator dotIndicator;

    touchgfx::ListLayout screens;

    GenericCallback<const SwipeContainer&>* menuChangeInitiatedAction;
    GenericCallback<const SwipeContainer&>* menuChangeEndedAction;

    void adjustScreens();

    void animateSwipeCancelledLeft();
    void animateSwipeCancelledRight();
    void animateLeft();
    void animateRight();

};


#endif
