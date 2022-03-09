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
  

#include <gui/main_menu_screen/MainMenuAnimatingButtonsView.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>

MainMenuAnimatingButtonsView::MainMenuAnimatingButtonsView() :
    MainMenuView(),
    currentAnimationState(NO_ANIMATION),
    startUpAnimationBoxDone(this, &MainMenuAnimatingButtonsView::StartupAnimationBoxDoneHandler),
    buttonAnimationDone(this, &MainMenuAnimatingButtonsView::buttonAnimationDoneHandler),
    buttonClicked(this, &MainMenuAnimatingButtonsView::buttonClickedHandler),
    textAnimationDone(this, &MainMenuAnimatingButtonsView::textAnimationDoneHandler)
{
}

MainMenuAnimatingButtonsView::~MainMenuAnimatingButtonsView()
{
}

void MainMenuAnimatingButtonsView::setupScreen()
{
    background.setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_BACKGROUND_ID));
    background.setXY(0, 0);
    add(background);

    add(mcuLoadTxt);
    add(mcuLoadValueTxt);

    add(toggleMenuButton);
    add(stMenuButton);

    startUpAnimationBox.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    startUpAnimationBox.setPosition(getActiveDisplayWidth()/2, getActiveDisplayHeight()/2 + BUTTONS_Y_OFFSET, 0, 0);
    startUpAnimationBox.setAnimationEndedCallback(startUpAnimationBoxDone);
    add(startUpAnimationBox);

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        buttonContainers[i].setPosition((getActiveDisplayWidth() - BUTTONS_WIDTH) / 2, BUTTONS_Y_OFFSET + ((getActiveDisplayHeight() - (BUTTONS_HEIGHT * NUMBER_OF_BUTTONS)) / 2) + i * BUTTONS_HEIGHT, BUTTONS_WIDTH, BUTTONS_HEIGHT );
        buttons[i].setBitmaps(Bitmap(BITMAP_ANIMATING_BUTTONS_TABLE_ELEMENT_SHADOW_ID), Bitmap(BITMAP_ANIMATING_BUTTONS_TABLE_ELEMENT_SHADOW_ID));
        buttons[i].setXY(0, 0);
        buttons[i].setVisible(false);
        buttons[i].setAction(buttonClicked);
        buttonContainers[i].add(buttons[i]);
        add(buttonContainers[i]);
    }
    buttonContainers[0].setMoveAnimationEndedAction(buttonAnimationDone);


    demoIDs[0] = CUSTOM_CONTROLS;
    icons[0].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_GAUGE_TINY_ID));
    texts[0].setTypedText(TypedText(T_ANIMATING_BUTTON_CUSTOM_CONTROLS_HEADLINE));

    demoIDs[1] = PRODUCT_PRESENTER;
    icons[1].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_POSTER_TINY_ID));
    texts[1].setTypedText(TypedText(T_ANIMATING_BUTTON_PRODUCT_PRESENTER_HEADLINE));

    demoIDs[2] = GAME2048;
    icons[2].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_2048_TINY_ID));
    texts[2].setTypedText(TypedText(T_ANIMATING_BUTTON_GAME_2048_HEADLINE));

    demoIDs[3] = GAME2D;
    icons[3].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_GAME_2D_TINY_ID));
    texts[3].setTypedText(TypedText(T_ANIMATING_BUTTON_GAME_2D_HEADLINE));

    demoIDs[4] = GRAPH;
    icons[4].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_GRAPH_TINY_ID));
    texts[4].setTypedText(TypedText(T_ANIMATING_BUTTON_GRAPH_HEADLINE));
    texts[4].setMoveAnimationEndedAction(textAnimationDone);

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        icons[i].setVisible(false);
        texts[i].setVisible(false);
        buttonContainers[i].add(icons[i]);
        buttonContainers[i].add(texts[i]);
    }

    startUpAnimationBox.startZoomAnimation(BUTTONS_WIDTH, BUTTONS_HEIGHT * NUMBER_OF_BUTTONS, 20, EasingEquations::backEaseOut, EasingEquations::backEaseOut);
    currentAnimationState = ANIMATE_BOX_IN;

    presenter->setPreviousSelectedMenuType(MainMenuPresenter::ANIMATING_BUTTONS_MENU);
}

void MainMenuAnimatingButtonsView::tearDownScreen()
{
}

void MainMenuAnimatingButtonsView::StartupAnimationBoxDoneHandler(const ZoomAnimationBox& box)
{
    if (currentAnimationState == ANIMATE_BOX_IN)
    {
        startUpAnimationBox.setVisible(false);
        startUpAnimationBox.invalidate();

        for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
        {
            buttons[i].setVisible(true);
            buttons[i].invalidate();

            buttonContainers[i].startMoveAnimation(buttonContainers[i].getX(), buttonContainers[i].getY() + (i - (NUMBER_OF_BUTTONS/2))*BUTTONS_SPACE, 16, EasingEquations::linearEaseNone, EasingEquations::backEaseOut);
            buttonContainers[i].setMoveAnimationDelay(i + 3);
        }
        currentAnimationState = ANIMATE_BUTTONS_FROM_BOX;
    }
    else if (currentAnimationState == ANIMATE_BOX_OUT)
    {
        gotoSelectedDemoScreen();
    }
}

void MainMenuAnimatingButtonsView::buttonClickedHandler(const AbstractButton& button)
{
    if (currentAnimationState != NO_ANIMATION)
    {
        return;
    }

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        if (&button == &buttons[i])
        {
            setSelectedDemoScreen(i);
        }

        int duration = 12;
        int delayMax = NUMBER_OF_BUTTONS - 1;
        icons[i].setFadeAnimationDelay(delayMax - i );
        icons[i].startFadeAnimation(0, duration);
        texts[i].setFadeAnimationDelay(delayMax - i );
        texts[i].startFadeAnimation(0, duration);

        buttonContainers[i].setMoveAnimationDelay(duration + delayMax + i);
        buttonContainers[i].startMoveAnimation((getActiveDisplayWidth() - BUTTONS_WIDTH) / 2, BUTTONS_Y_OFFSET + ((getActiveDisplayHeight() - (BUTTONS_HEIGHT * NUMBER_OF_BUTTONS)) / 2) + i * BUTTONS_HEIGHT, 16, EasingEquations::linearEaseNone, EasingEquations::backEaseOut);
    }
    currentAnimationState = ANIMATE_BUTTONS_INTO_BOX;
}

void MainMenuAnimatingButtonsView::buttonAnimationDoneHandler(const MoveAnimator<Container>& box)
{
    if (currentAnimationState == ANIMATE_BUTTONS_FROM_BOX)
    {
        for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
        {
            texts[i].setVisible(true);
            texts[i].setXY(buttons[i].getRect().right(), buttons[i].getY() + 14);
            texts[i].setMoveAnimationDelay(i*2);
            texts[i].startMoveAnimation(buttons[i].getX() + 60, texts[i].getY(), 20, EasingEquations::cubicEaseOut);

            icons[i].setVisible(true);
            icons[i].setXY(buttons[i].getX() + 10, buttons[i].getY() + 5);
            icons[i].setAlpha(0);
            icons[i].startFadeAnimation(255, 16);
        }
        currentAnimationState = ANIMATE_ICONS_AND_TEXT_IN;
    }
    else if (currentAnimationState == ANIMATE_BUTTONS_INTO_BOX)
    {
        for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
        {
            buttons[i].setVisible(false);
            buttons[i].invalidate();
        }

        startUpAnimationBox.setVisible(true);
        startUpAnimationBox.invalidate();

        startUpAnimationBox.startZoomAnimation(0, 0, 14, EasingEquations::backEaseOut, EasingEquations::backEaseOut);
        currentAnimationState = ANIMATE_BOX_OUT;
    }
}

void MainMenuAnimatingButtonsView::toggleMenu()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
}

void MainMenuAnimatingButtonsView::textAnimationDoneHandler(const MoveAnimator<TextArea>& t)
{
    if (currentAnimationState == ANIMATE_ICONS_AND_TEXT_IN && &t == &texts[4])
    {
        currentAnimationState = NO_ANIMATION;
    }
}

void MainMenuAnimatingButtonsView::screenSaverMinorTick()
{
    toggleMenu();
}
