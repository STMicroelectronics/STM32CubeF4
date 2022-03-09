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
  


#ifndef CUSTOM_CONTROLS_VIEW_HPP
#define CUSTOM_CONTROLS_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/custom_controls_screen/CustomControlsPresenter.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>
#include <gui/custom_controls_screen/ThreeWayProgressBar.hpp>
#include <gui/custom_controls_screen/CircularProgress.hpp>
#include <gui/custom_controls_screen/DatePicker.hpp>
#include <gui/custom_controls_screen/Gauge.hpp>
#include <gui/custom_controls_screen/VerticalSlideMenu.hpp>

class CustomControlsView : public DemoView<CustomControlsPresenter>
{
public:
    CustomControlsView();
    virtual ~CustomControlsView();

    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    virtual void screenSaverMinorTick();

private:
    Image menuBackground;
    Image background;
    Button menuUp;
    Button menuDown;
    Callback<CustomControlsView, const AbstractButton&> onButtonPressed;
    void buttonPressedHandler(const AbstractButton& button);


    VerticalSlideMenu slideMenu;
    Callback<CustomControlsView, const VerticalSlideMenu& > slideMenuElementSelectedCallback;
    void slideMenuElementSelectedHandler(const VerticalSlideMenu& menu);

    FadeAnimator<TextArea> statusMessage;

    ThreeWayProgressBar threeWayProgressBar;
    Callback<CustomControlsView, const ThreeWayProgressBar& > threeWayButtonPressedCallback;
    void threeWayButtonPressedHandler(const ThreeWayProgressBar& button);

    CircularProgress percentageBar;
    DatePicker datePicker;
    Gauge gauge;

    // Animation members
    enum AnimationState
    {
        ANIMATION_THREE_WAY_RUNNING,
        ANIMATION_GAUGE,
        NO_ANIMATION
    };

    AnimationState currentAnimationState;

    int animationCounter;
    int tickCounter;

    // ThreeWayProgressBar animation
    static const int NUMBER_OF_BARS = 3;
    AnimationState barAnimationState[NUMBER_OF_BARS];
    int animationDuration[NUMBER_OF_BARS];
    EasingEquation animationEquation[NUMBER_OF_BARS];

    AnimationState getAnimationState();
    EasingEquation getRandomEasingEquation();
    void startThreeWayProgressBarAnimation();
    void stopThreeWayProgressBarAnimation();
};

#endif // CUSTOM_CONTROLS_VIEW_HPP
