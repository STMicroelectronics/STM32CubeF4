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
  

#ifndef MAIN_MENU_ANIMATING_BUTTONS_VIEW_HPP
#define MAIN_MENU_ANIMATING_BUTTONS_VIEW_HPP

#include <gui/main_menu_screen/MainMenuView.hpp>
#include <gui/main_menu_screen/MainMenuPresenter.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <gui/common/ZoomAnimationBox.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

class MainMenuAnimatingButtonsView : public MainMenuView
{
public:
    MainMenuAnimatingButtonsView();
    virtual ~MainMenuAnimatingButtonsView();

    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void screenSaverMinorTick();
private:
    enum AnimationState
    {
        ANIMATE_BOX_IN,
        ANIMATE_BOX_OUT,
        ANIMATE_BUTTONS_FROM_BOX,
        ANIMATE_BUTTONS_INTO_BOX,
        ANIMATE_ICONS_AND_TEXT_IN,
        NO_ANIMATION
    };

    AnimationState currentAnimationState;

    static const int NUMBER_OF_BUTTONS = NUMBER_OF_DEMO_SCREENS;

    static const int BUTTONS_WIDTH = 360;
    static const int BUTTONS_HEIGHT = 50;
    static const int BUTTONS_SPACE = 14;

    static const int BUTTONS_Y_OFFSET = 30;

    Image background;
    ZoomAnimationBox startUpAnimationBox;

    MoveAnimator<Container> buttonContainers[NUMBER_OF_BUTTONS];
    Button buttons[NUMBER_OF_BUTTONS];
    FadeAnimator<Image> icons[NUMBER_OF_BUTTONS];
    FadeAnimator<MoveAnimator<TextArea> > texts[NUMBER_OF_BUTTONS];

    Callback<MainMenuAnimatingButtonsView, const ZoomAnimationBox&> startUpAnimationBoxDone;
    void StartupAnimationBoxDoneHandler(const ZoomAnimationBox& box);

    Callback<MainMenuAnimatingButtonsView, const MoveAnimator<Container>&> buttonAnimationDone;
    void buttonAnimationDoneHandler(const MoveAnimator<Container>& box);

    Callback<MainMenuAnimatingButtonsView, const AbstractButton&> buttonClicked;
    void buttonClickedHandler(const AbstractButton& button);

    Callback<MainMenuAnimatingButtonsView, const MoveAnimator<TextArea>&> textAnimationDone;
    void textAnimationDoneHandler(const MoveAnimator<TextArea>& t);

    virtual void toggleMenu();
};

#endif // MAIN_MENU_ANIMATING_BUTTONS_VIEW_HPP
