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
  


#include <new>
#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <touchgfx/transitions/SlideTransition.hpp>
#include <gui/menu_screen/MenuView.hpp>
#include <gui/menu_screen/MenuPresenter.hpp>
#include <gui/menu_screen/MenuView.hpp>
#include <gui/menu_screen/MenuPresenter.hpp>
#include <gui/home_automation_screen/HomeAutomationView.hpp>
#include <gui/home_automation_screen/HomeAutomationPresenter.hpp>
#include <gui/animated_graphics_screen/AnimatedGraphicsView.hpp>
#include <gui/animated_graphics_screen/AnimatedGraphicsPresenter.hpp>
#include <gui/live_data_display_screen/LiveDataDisplayView.hpp>
#include <gui/live_data_display_screen/LiveDataDisplayPresenter.hpp>
#include <gui/chrom_art_screen/ChromArtView.hpp>
#include <gui/chrom_art_screen/ChromArtPresenter.hpp>
#include <gui/common/FrontendHeap.hpp>


FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : MVPApplication(),
      transitionCallback(),
      frontendHeap(heap),
      model(m)
{
}

void FrontendApplication::handleTickEvent()
{
    model.tick();
    MVPApplication::handleTickEvent();
}

void FrontendApplication::gotoMenuScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoMenuScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoMenuScreenImpl()
{
    makeTransition< MenuView, MenuPresenter, SlideTransition<WEST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoMenuScreenNoAnimation()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoMenuScreenNoAnimationImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoMenuScreenNoAnimationImpl()
{
    makeTransition< MenuView, MenuPresenter, NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoHomeAutomationScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoHomeAutomationScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoHomeAutomationScreenImpl()
{
    makeTransition< HomeAutomationView, HomeAutomationPresenter, SlideTransition<NORTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoAnimatedGraphicsScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoAnimatedGraphicsScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoAnimatedGraphicsScreenImpl()
{
    makeTransition< AnimatedGraphicsView, AnimatedGraphicsPresenter, SlideTransition<WEST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoLiveDataDisplayScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoLiveDataDisplayScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoLiveDataDisplayScreenImpl()
{
    makeTransition< LiveDataDisplayView, LiveDataDisplayPresenter, SlideTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoChromArtScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoChromArtScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoChromArtScreenImpl()
{
    makeTransition< ChromArtView, ChromArtPresenter, SlideTransition<EAST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
