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
  


#include <gui/main_menu_screen/MainMenuView.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>

MainMenuView::MainMenuView() :
    buttonPressedCallback(this, &MainMenuView::buttonPressedHandler),
    menuStateChangeInitiatedCallback(this, &MainMenuView::menuStateChangeInitiatedHandler),
    menuStateChangeEndedCallback(this, &MainMenuView::menuStateChangeEndedHandler)
{
}

MainMenuView::~MainMenuView()
{
}

void MainMenuView::setupScreen()
{
    backgroundLeft.setBitmap(Bitmap(BITMAP_MENU_STAGE_STRETCH_LEFT_SIDE_ID));
    backgroundLeft.setXY(0, 0);
    add(backgroundLeft);

    backgroundRight.setBitmap(Bitmap(BITMAP_MENU_STAGE_STRETCH_RIGHT_SIDE_ID));
    backgroundRight.setXY(HAL::DISPLAY_WIDTH - backgroundRight.getWidth(), 0);
    add(backgroundRight);

    menuContainer.setXY(0, 0);
    menuContainer.setSwipeCutoff(80);
    menuContainer.setEndSwipeElasticWidth(50);
    menuContainer.setDotIndicatorBitmaps(Bitmap(BITMAP_SCREEN_SWIPE_DOTS_INACTIVE_ID), Bitmap(BITMAP_SCREEN_SWIPE_DOTS_ACTIVE_ID));
    menuContainer.setDotIndicatorXYWithCenteredX(HAL::DISPLAY_WIDTH / 2, HAL::DISPLAY_HEIGHT - 16);
    menuContainer.setMenuChangeInitiatedCallback(menuStateChangeInitiatedCallback);
    menuContainer.setMenuChangeEndedCallback(menuStateChangeEndedCallback);
    menuContainer.setTouchable(false);

    demoScreens[0].setBitmap(Bitmap(BITMAP_MENU_DEMO_SCREEN_02_ID));
    demoScreens[1].setBitmap(Bitmap(BITMAP_MENU_DEMO_SCREEN_03_ID));
    demoScreens[2].setBitmap(Bitmap(BITMAP_MENU_DEMO_SCREEN_04_ID));
    demoScreens[3].setBitmap(Bitmap(BITMAP_MENU_DEMO_SCREEN_05_ID));
    demoScreensButton[0].setBitmaps(Bitmap(BITMAP_DEMO_BUTTON_02_03_ID), Bitmap(BITMAP_DEMO_BUTTON_02_03_PRESSED_ID));
    demoScreensButton[1].setBitmaps(Bitmap(BITMAP_DEMO_BUTTON_02_03_ID), Bitmap(BITMAP_DEMO_BUTTON_02_03_PRESSED_ID));
    demoScreensButton[2].setBitmaps(Bitmap(BITMAP_DEMO_BUTTON_04_ID), Bitmap(BITMAP_DEMO_BUTTON_04_PRESSED_ID));
    demoScreensButton[3].setBitmaps(Bitmap(BITMAP_DEMO_BUTTON_05_06_ID), Bitmap(BITMAP_DEMO_BUTTON_05_06_PRESSED_ID));


    for (int i = 0; i < NUMBER_OF_DEMO_SCREENS; i++)
    {
        demoScreensButton[i].setXY(318, 137);
        demoScreensButton[i].setTouchable(false);
        demoScreensButton[i].setAction(buttonPressedCallback);
        demoScreensContainer[i].setPosition(0, 0, demoScreens[i].getWidth(), demoScreens[i].getHeight());
        demoScreensContainer[i].add(demoScreens[i]);
        demoScreensContainer[i].add(demoScreensButton[i]);
        menuContainer.add(demoScreensContainer[i]);
    }

    add(menuContainer);

    mcuLoadTxt.setXY(370, 30 - mcuLoadTxt.getTypedText().getFont()->getFontHeight());
    mcuLoadValueTxt.setPosition(mcuLoadTxt.getRect().right(), mcuLoadTxt.getY(), 34, 14);
    add(mcuLoadTxt);
    add(mcuLoadValueTxt);

    demoScreenIndex[0] = MainMenuPresenter::CUSTOM_CONTROLS;
    demoScreenIndex[1] = MainMenuPresenter::EASING_EQUATION;
    demoScreenIndex[2] = MainMenuPresenter::PRODUCT_PRESENTER;
    demoScreenIndex[3] = MainMenuPresenter::GRAPH_SCREEN;
}

void MainMenuView::afterTransition()
{
    menuContainer.setTouchable(true);
    for (int i = 0; i < NUMBER_OF_DEMO_SCREENS; i++)
    {
        demoScreensButton[i].setTouchable(true);
    }
}

void MainMenuView::tearDownScreen()
{

}

void MainMenuView::buttonPressedHandler(const AbstractButton& button)
{
    gotoSelectedDemoScreen(menuContainer.getSelectedScreen());
}

void MainMenuView::gotoSelectedDemoScreen(uint8_t demoIndex)
{
    presenter->demoSelected(demoScreenIndex[demoIndex], demoIndex);
}

void MainMenuView::setSelectedMenuIndex(uint8_t demoIndex)
{
    menuContainer.setSelectedScreen(demoIndex);
}

void MainMenuView::menuStateChangeInitiatedHandler(const SwipeContainer& sc)
{
}

void MainMenuView::menuStateChangeEndedHandler(const SwipeContainer& sc)
{
}

