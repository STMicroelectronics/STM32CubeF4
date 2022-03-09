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
#include <gui/common/FrontendHeap.hpp>
#include <gui/main_menu_screen/MainMenuAnimatingButtonsView.hpp>
#include <gui/main_menu_screen/MainMenuPresenter.hpp>
#include <gui/product_presenter_screen/ProductPresenterView.hpp>
#include <gui/product_presenter_screen/ProductPresenterPresenter.hpp>
#include <gui/game2D_screen/Game2DView.hpp>
#include <gui/game2D_screen/Game2DPresenter.hpp>
#include <gui/custom_controls_screen/CustomControlsView.hpp>
#include <gui/custom_controls_screen/CustomControlsPresenter.hpp>
#include <gui/graph_screen/GraphView.hpp>
#include <gui/graph_screen/GraphPresenter.hpp>
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

void FrontendApplication::gotoMainMenuAnimatingButtonsScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoMainMenuAnimatingButtonsScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoMainMenuAnimatingButtonsScreenImpl()
{
    makeTransition< MainMenuAnimatingButtonsView, MainMenuPresenter, NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoProductPresenterScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoProductPresenterScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoProductPresenterScreenImpl()
{
    makeTransition< ProductPresenterView, ProductPresenterPresenter, NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoGame2DScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoGame2DScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoGame2DScreenImpl()
{
    makeTransition< Game2DView, Game2DPresenter, NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoCustomControlsScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoCustomControlsScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoCustomControlsScreenImpl()
{
    makeTransition< CustomControlsView, CustomControlsPresenter, NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoGraphScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoGraphScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoGraphScreenImpl()
{
    makeTransition< GraphView, GraphPresenter, NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
