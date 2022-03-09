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
  


#ifndef COLLAPSIBLE_MENU_HPP
#define COLLAPSIBLE_MENU_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

using namespace touchgfx;

/**
 * @class CollapsibleMenu
 *
 * This widget is part of the TouchGFX Open Widget Repository.
 * https://github.com/draupnergraphics/touchgfx-widgets
 *
 * A menu that holds a set of elements that is placed on top
 * of each other and is faded out. You can activate the menu
 * and fade in the selected element and then expand the menu
 * so that all elements becomes visible.
 *
 * @see CollapsibleMenu
 */
class CollapsibleMenu : public Container
{
public:
    enum ExpandDirection
    {
        LEFT,
        RIGHT
    };

    CollapsibleMenu();
    virtual ~CollapsibleMenu();

    void addMenuElement(const Bitmap& elementBitmap, const Bitmap& elementPressedBitmap);

    void finilizeInitialization();

    /**
     * @fn void CollapsibleMenu::setElementSpace(uint16_t space);
     *
     * @brief Sets the size of the space between the menu elements.
     *
     * @param space The size of the space between the menu elements (in pixels).
     */
    void setElementSpace(uint16_t space);

    /**
     * @fn void CollapsibleMenu::setExpandDirection(ExpandDirection direction);
     *
     * @brief Sets expand direction.
     *
     * @param direction The direction.
     */
    void setExpandDirection(ExpandDirection direction);

    /**
     * @fn void CollapsibleMenu::collapseMenu();
     *
     * @brief Collapses the menu.
     */
    void collapseMenu();

    /**
     * @fn void CollapsibleMenu::expandMenu();
     *
     * @brief Expands the menu.
     */
    void expandMenu();

    /**
     * @fn void CollapsibleMenu::setTimeout(int newTimeout);
     *
     * @brief Sets a timeout for automatic collapse.
     *
     * @param newTimeout The new timeout.
     *
     * @see collapseMenu
     */
    void setTimeout(int newTimeout);

    /**
     * @fn void CollapsibleMenu::fadeIn(int duration = 14);
     *
     * @brief Fade in the selected item.
     *
     * @param duration (Optional) the duration.
     */
    void fadeIn(int duration = 14);

    /**
     * @fn void CollapsibleMenu::fadeInAndExpand(int duration = 14);
     *
     * @brief Fade in the selected item and expand.
     *
     * @param duration (Optional) the duration.
     */
    void fadeInAndExpand(int duration = 14);

    /**
     * @fn void CollapsibleMenu::fadeOut(int duration = 14);
     *
     * @brief Fade out the selected item.
     *
     * @param duration (Optional) the duration.
     */
    void fadeOut(int duration = 14);

    /**
     * @fn bool CollapsibleMenu::isFadedOut();
     *
     * @brief Query if the selected item is faded out.
     *
     * @return true if faded out, false if not.
     */
    bool isFadedOut();

    /**
     * @fn uint8_t CollapsibleMenu::getSelectedElementIndex();
     *
     * @brief Gets selected element index.
     *
     * @return The selected element index.
     */
    uint8_t getSelectedElementIndex();

    /**
     * @fn void CollapsibleMenu::setStateChangedCallback(touchgfx::GenericCallback<const CollapsibleMenu&, const bool>& callback)
     *
     * @brief Associates an action to be performed when the menu changes expanded/collapsed state.
     *
     * @param callback The callback to be executed. The callback will be given a reference to the
     *                 CollapsibleMenu and a bool that state if the menu is expanded.
     *
     * @see GenericCallback
     */
    void setStateChangedCallback(touchgfx::GenericCallback<const CollapsibleMenu&, const bool>& callback)
    {
        stateChangedAction = &callback;
    }


private:
    static const uint8_t MAX_NUMBER_OF_ELEMENTS = 10;

    enum AnimationState
    {
        ANIMATE_TO_EXPANDED,
        ANIMATE_TO_COLLAPSED,
        FADE_OUT,
        FADE_IN,
        FADE_IN_AND_EXPAND,
        NO_ANIMATION
    };

    AnimationState currentAnimationState;

    uint8_t size;
    bool isExpanded;
    uint16_t elementSpace;
    ExpandDirection expandDirection;
    bool fadedOut;
    int timeout;
    int timeoutCounter;

    FadeAnimator<MoveAnimator<Button> > menuElements[MAX_NUMBER_OF_ELEMENTS];

    uint8_t selectedElementIndex;

    Callback<CollapsibleMenu, const AbstractButton&> onButtonPressed;
    Callback<CollapsibleMenu, const MoveAnimator<Button>& > menuElementMoveAnimationEndedCallback;
    Callback<CollapsibleMenu, const FadeAnimator<MoveAnimator<Button> >& > menuElementFadeAnimationEndedCallback;

    GenericCallback<const CollapsibleMenu&, const bool>* stateChangedAction;

    void buttonPressedHandler(const AbstractButton& button);
    void menuElementMoveAnimationEndedHandler(const MoveAnimator<Button>& element);
    void menuElementFadeAnimationEndedHandler(const FadeAnimator<MoveAnimator<Button> >& element);
    bool anyFadeAnimationRunning();
    virtual void handleTickEvent();
};

#endif /* COLLAPSIBLE_MENU_HPP */
