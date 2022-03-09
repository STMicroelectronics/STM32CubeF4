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
  


#include <new>
#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <touchgfx/transitions/SlideTransition.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <gui/main_menu_screen/MainMenuView.hpp>
#include <gui/main_menu_screen/MainMenuPresenter.hpp>
#include <gui/custom_controls_screen/CustomControlsView.hpp>
#include <gui/custom_controls_screen/CustomControlsPresenter.hpp>
#include <gui/graph_screen/GraphView.hpp>
#include <gui/graph_screen/GraphPresenter.hpp>
#include <gui/easing_equation_screen/EasingEquationView.hpp>
#include <gui/easing_equation_screen/EasingEquationPresenter.hpp>
#include <gui/product_presenter_screen/ProductPresenterView.hpp>
#include <gui/product_presenter_screen/ProductPresenterPresenter.hpp>
#include <stdlib.h>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : MVPApplication(),
      transitionCallback(),
      frontendHeap(heap),
      model(m),
      tickCounter(0),
      screenSaverTick(0),
      lastClickTime(0)
{
}

void FrontendApplication::handleTickEvent()
{
    model.tick();
    MVPApplication::handleTickEvent();
    tickCounter++;

    // Screen saver functionality:
    if (lastClickTime.hasValue() && (model.getCurrentTime() - lastClickTime) >= SCREEN_SAVER_TIMEOUT)
    {
        screenSaverTick++;
        if (screenSaverTick % MINOR_TICK_FREQ == 0)
        {
            model.screenSaverMinorTick();
        }
        else if (screenSaverTick % MAJOR_TICK_FREQ == 0)
        {
            model.screenSaverMajorTick();
        }
    }

}

void FrontendApplication::handleClickEvent(const ClickEvent& evt)
{
    MVPApplication::handleClickEvent(evt);

    // A click event has been registered so reset last click time
    resetScreenSaver();
}

void FrontendApplication::handleDragEvent(const DragEvent& evt)
{
    MVPApplication::handleDragEvent(evt);

    // A drag event has been registered so reset last click time
    resetScreenSaver();
}

void FrontendApplication::resetScreenSaver()
{
    lastClickTime = model.getCurrentTime();
    screenSaverTick = 0;
}

void FrontendApplication::gotoMainMenuScreenNoTransition()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoMainMenuScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoMainMenuScreenNoTransitionImpl()
{
    makeTransition< MainMenuView, MainMenuPresenter, NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoMainMenuScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoMainMenuScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoMainMenuScreenImpl()
{
    makeTransition< MainMenuView, MainMenuPresenter, SlideTransition<NORTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoCustomControlsScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoCustomControlsScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoCustomControlsScreenImpl()
{
    makeTransition< CustomControlsView, CustomControlsPresenter, SlideTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoGraphScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoGraphScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoGraphScreenImpl()
{
    makeTransition< GraphView, GraphPresenter, SlideTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoEasingEquationScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoEasingEquationScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoEasingEquationScreenImpl()
{
    makeTransition< EasingEquationView, EasingEquationPresenter, SlideTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoProductPresenterScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoProductPresenterScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoProductPresenterScreenImpl()
{
    makeTransition< ProductPresenterView, ProductPresenterPresenter, SlideTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
