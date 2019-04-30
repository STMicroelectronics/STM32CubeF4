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
  

#ifndef MAIN_MENU_CAROUSEL_VIEW_HPP
#define MAIN_MENU_CAROUSEL_VIEW_HPP

#include <gui/main_menu_screen/MainMenuView.hpp>
#include <gui/main_menu_screen/MainMenuPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TouchArea.hpp>
#include <gui/common/Carousel.hpp>

class MainMenuCarouselView : public MainMenuView
{
public:
    MainMenuCarouselView();
    virtual ~MainMenuCarouselView() { }

    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleDragEvent(const DragEvent& evt);
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void screenSaverMinorTick();
private:

    enum State
    {
        ELEMENT_SELECTED,
        NO_STATE
    };

    State currentState;

    Image background;

    Carousel carousel;


    TEXTS headlines[NUMBER_OF_DEMO_SCREENS];
    TEXTS texts[NUMBER_OF_DEMO_SCREENS];

    TextArea demoHeadline;
    TextArea demoText;

    int pauses;

    void setupCarousel();

    Callback<MainMenuCarouselView, const Carousel&> carouselAnimationEnded;
    void carouselAnimationEndedhandler(const Carousel& c);

    virtual void toggleMenu();
    void updateDemoText();

    int16_t dragDistance;
    bool dragInsideCarousel;
    bool dragInsideCenterButton;
    bool hasAnimatedDrag;
};

#endif // MAIN_MENU_CAROUSEL_VIEW_HPP
