/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
