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
  

#ifndef GAME_2048_VIEW_HPP
#define GAME_2048_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/game2048_screen/Game2048Presenter.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>
#include <gui/game2048_screen/GameBoard.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <gui/common/SwipeContainer.hpp>
#include <touchgfx/containers/ListLayout.hpp>

class Game2048View : public DemoView<Game2048Presenter>
{
public:
    Game2048View();
    virtual ~Game2048View();

    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void screenSaverMinorTick();

private:
    static const int NUMBER_OF_INSTRUCTIONS = 3;

    Image background;

    GameBoard gameBoard;
    int currentScore;

    TextAreaWithOneWildcard scoreTxt;
    Unicode::UnicodeChar scoreTxtbuf[10];

    FadeAnimator<MoveAnimator<TextAreaWithOneWildcard> > deltaScoreTxt;
    Unicode::UnicodeChar deltaScoreTxtbuf[10];

    Button newGameButton;
    ToggleButton autoPlayButton;
    Button howToPlayButton;

    Box overlay;
    Image popUp;

    SwipeContainer howToPlayInstructionContainer;
    Container howToPlayInstructionExtraSpaceContainer[NUMBER_OF_INSTRUCTIONS];
    Image howToPlayInstruction[NUMBER_OF_INSTRUCTIONS];
    
    Button closeHowToPlayButton;

    Image endGameStatus;

    Callback<Game2048View, const GameBoard& > moveCompletedCallback;
    Callback<Game2048View, const GameBoard& > gameWonCallback;
    Callback<Game2048View, const GameBoard& > gameLostCallback;
    Callback<Game2048View, const AbstractButton& > buttonPressedCallback;

    void moveCompletedHandler(const GameBoard& gb);
    void gameWonHandler(const GameBoard& gb);    
    void gameLostHandler(const GameBoard& gb);

    void buttonPressedHandler(const AbstractButton& button);
 
    void updateScoreText();
    void showDeltaScoreText(int deltaScore);
    void showHowToPlayPopUp(bool visible);

    void startNewGame();
};

#endif // GAME_2048_VIEW_HPP
