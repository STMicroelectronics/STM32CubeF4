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
  

#include <gui/game2048_screen/Game2048View.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>

Game2048View::Game2048View() :
    DemoView(),
    moveCompletedCallback(this, &Game2048View::moveCompletedHandler),
    gameWonCallback(this, &Game2048View::gameWonHandler),
    gameLostCallback(this, &Game2048View::gameLostHandler),
    buttonPressedCallback(this, &Game2048View::buttonPressedHandler)
{
    background.setBitmap(Bitmap(BITMAP_GAME_2048_BACKGROUND_ID));
    background.setXY(0, 0);
    add(background);

    gotoMenuButton.setBitmaps(Bitmap(BITMAP_GAME_2048_MENU_BUTTON_ID), Bitmap(BITMAP_GAME_2048_MENU_BUTTON_PRESSED_ID));
    gotoMenuButton.setXY(642, 424);
    add(gotoMenuButton);

    gameBoard.setXY(20, 0);
    gameBoard.setMoveCompletedCallback(moveCompletedCallback);
    gameBoard.setGameWonCallback(gameWonCallback);
    gameBoard.setGameLostCallback(gameLostCallback);
    add(gameBoard);

    currentScore = 0;

    scoreTxt.setWildcard(scoreTxtbuf);
    scoreTxt.setTypedText(TypedText(T_2048_SCORE));
    scoreTxt.setPosition(688-200, 58 - scoreTxt.getTypedText().getFont()->getFontHeight()/2, 200, 50); //134, 25);
    scoreTxt.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    updateScoreText();
    add(scoreTxt);

    deltaScoreTxt.setWildcard(deltaScoreTxtbuf);
    deltaScoreTxt.setTypedText(TypedText(T_2048_SCORE_DELTA));
    deltaScoreTxt.setWidth(scoreTxt.getWidth());
    deltaScoreTxt.setHeight(scoreTxt.getHeight());
    deltaScoreTxt.setColor(Color::getColorFrom24BitRGB(0x28, 0x28, 0x28));
    deltaScoreTxt.setAlpha(0);
    add(deltaScoreTxt);

    newGameButton.setBitmaps(Bitmap(BITMAP_NEW_GAME_BUTTON_ID), Bitmap(BITMAP_NEW_GAME_BUTTON_PRESSED_ID));
    newGameButton.setXY(528, 106);
    newGameButton.setAction(buttonPressedCallback);
    add(newGameButton);

    autoPlayButton.setBitmaps(Bitmap(BITMAP_AUTO_PLAY_BUTTON_ID), Bitmap(BITMAP_AUTO_PLAY_BUTTON_PRESSED_ID));
    autoPlayButton.setXY(newGameButton.getX(), 183);
    autoPlayButton.setAction(buttonPressedCallback);
    add(autoPlayButton);

    howToPlayButton.setBitmaps(Bitmap(BITMAP_HOW_TO_PLAY_BUTTON_ID), Bitmap(BITMAP_HOW_TO_PLAY_BUTTON_PRESSED_ID));
    howToPlayButton.setXY(newGameButton.getX(), 260);
    howToPlayButton.setAction(buttonPressedCallback);
    add(howToPlayButton);

    mcuLoadTxt.setXY(600, 336);
    mcuLoadValueTxt.setPosition(mcuLoadTxt.getRect().right()-4, mcuLoadTxt.getY(), 50, 30);
    add(mcuLoadTxt);
    add(mcuLoadValueTxt);

    endGameStatus.setBitmap(Bitmap(BITMAP_GAME_2048_YOU_WIN_ID));
    endGameStatus.setXY(gameBoard.getX()+(gameBoard.getWidth() - endGameStatus.getWidth())/2, gameBoard.getY()+(gameBoard.getHeight() - endGameStatus.getHeight())/2);
    endGameStatus.setVisible(false);
    add(endGameStatus);

    overlay.setPosition(0, 0, getActiveDisplayWidth(), getActiveDisplayHeight());
    overlay.setColor(Color::getColorFrom24BitRGB(0x0, 0x0, 0x0));
    overlay.setAlpha(160);
    overlay.setTouchable(true);
    overlay.setVisible(false);
    add(overlay);

    popUp.setBitmap(Bitmap(BITMAP_HOW_TO_PLAY_BACKGROUND_ID));
    popUp.setVisible(false);
    add(popUp);

    howToPlayInstruction[0].setBitmap(Bitmap(BITMAP_HOW_TO_PLAY_00_ID));    
    howToPlayInstruction[1].setBitmap(Bitmap(BITMAP_HOW_TO_PLAY_01_ID));
    howToPlayInstruction[2].setBitmap(Bitmap(BITMAP_HOW_TO_PLAY_02_ID));

    for (int i = 0; i < NUMBER_OF_INSTRUCTIONS; i++)
    {
        // Enlarge the instruction swipe area by placing it in the howToPlayInstructionExtraSpaceContainer
        howToPlayInstruction[i].setXY(43, 10);
        howToPlayInstructionExtraSpaceContainer[i].add(howToPlayInstruction[i]);
        howToPlayInstructionExtraSpaceContainer[i].setPosition(0, 0, 43 + howToPlayInstruction[i].getWidth() + 44, howToPlayInstruction[i].getHeight()+20);
        howToPlayInstructionContainer.add(howToPlayInstructionExtraSpaceContainer[i]);
    }
    howToPlayInstructionContainer.setXY((getActiveDisplayWidth() - howToPlayInstructionContainer.getWidth())/2, (getActiveDisplayHeight() - howToPlayInstructionContainer.getHeight())/2);
    howToPlayInstructionContainer.setDotIndicatorBitmaps(Bitmap(BITMAP_GAME_2048_DOT_OFF_ID), Bitmap(BITMAP_GAME_2048_DOT_ON_ID));
    howToPlayInstructionContainer.setDotIndicatorXYWithCenteredX(howToPlayInstructionContainer.getWidth()/2, howToPlayInstructionContainer.getHeight()-7);
    howToPlayInstructionContainer.setSwipeCutoff(80);
    howToPlayInstructionContainer.setEndSwipeElasticWidth(30);
    howToPlayInstructionContainer.setVisible(false);
    add(howToPlayInstructionContainer);

    popUp.setXY((getActiveDisplayWidth() - popUp.getWidth())/2, (getActiveDisplayHeight() - popUp.getHeight())/2);

    closeHowToPlayButton.setBitmaps(Bitmap(BITMAP_HOW_TO_PLAY_CLOSE_BUTTON_ID), Bitmap(BITMAP_HOW_TO_PLAY_CLOSE_BUTTON_ID));
    closeHowToPlayButton.setXY(popUp.getAbsoluteRect().right() - closeHowToPlayButton.getWidth() - 5, popUp.getY() + 5);
    closeHowToPlayButton.setAction(buttonPressedCallback);
    closeHowToPlayButton.setVisible(false);
    add(closeHowToPlayButton);
}

Game2048View::~Game2048View()
{
}

void Game2048View::setupScreen()
{

}

void Game2048View::tearDownScreen()
{
}

void Game2048View::moveCompletedHandler(const GameBoard& gb)
{
    int scoreDifference = gb.getScore() - currentScore;
    currentScore = gb.getScore();
    updateScoreText();
    showDeltaScoreText(scoreDifference);
}


void Game2048View::gameWonHandler(const GameBoard& gb)
{
    endGameStatus.setBitmap(Bitmap(BITMAP_GAME_2048_YOU_WIN_ID));
    endGameStatus.setVisible(true);
    endGameStatus.invalidate();

    gameBoard.setTouchable(false);
    autoPlayButton.setTouchable(false);
    howToPlayButton.setTouchable(false);
}

void Game2048View::gameLostHandler(const GameBoard& gb)
{
    endGameStatus.setBitmap(Bitmap(BITMAP_GAME_2048_GAME_OVER_ID));
    endGameStatus.setVisible(true);
    endGameStatus.invalidate();

    gameBoard.setTouchable(false);
    autoPlayButton.setTouchable(false);
    howToPlayButton.setTouchable(false);
}

void Game2048View::updateScoreText()
{
    Unicode::snprintf(scoreTxtbuf, 10, "%d", currentScore);
    scoreTxt.invalidate();
}

void Game2048View::showDeltaScoreText(int deltaScore)
{
    if (deltaScore == 0)
    {
        return;
    }
    Unicode::snprintf(deltaScoreTxtbuf, 10, "%d", deltaScore);

    deltaScoreTxt.setAlpha(255);
    deltaScoreTxt.setXY(scoreTxt.getX() - 8, scoreTxt.getY() - 4);

    int duration = 26;
    deltaScoreTxt.startMoveAnimation(deltaScoreTxt.getX(), deltaScoreTxt.getY() - 24, duration);
    deltaScoreTxt.startFadeAnimation(0, duration, EasingEquations::cubicEaseIn);
    deltaScoreTxt.invalidate();
}

void Game2048View::buttonPressedHandler(const AbstractButton& button)
{
    if (!gameBoard.isTouchable())
    {
        // Game has either been lost or won
        if (&button == &newGameButton)
        {
            endGameStatus.setVisible(false);
            endGameStatus.invalidate();
            gameBoard.setTouchable(true);
            autoPlayButton.setTouchable(true);
            howToPlayButton.setTouchable(true);
            startNewGame();
        }
        else
        {
            return;
        }
    }

    if (&button == &newGameButton)
    {
        startNewGame();
    }
    else if (&button == &autoPlayButton)
    {
        gameBoard.setAutoPlay(autoPlayButton.getState());
    }
    else if (&button == &howToPlayButton)
    {
        howToPlayInstructionContainer.setSelectedScreen(0);
        showHowToPlayPopUp(true);
    }
    else if (&button == &closeHowToPlayButton)
    {
        showHowToPlayPopUp(false);
    }
}

void Game2048View::showHowToPlayPopUp(bool visible)
{
    overlay.setVisible(visible);
    popUp.setVisible(visible);
    howToPlayInstructionContainer.setVisible(visible);
    closeHowToPlayButton.setVisible(visible);
    overlay.invalidate();
}

void Game2048View::startNewGame()
{
    gameBoard.initiateNewGame();
    currentScore = 0;
    updateScoreText();
}

void Game2048View::screenSaverMinorTick()
{
    if (popUp.isVisible())
    {
        showHowToPlayPopUp(false);
    }

    if (endGameStatus.isVisible())
    {
        buttonPressedHandler(newGameButton);
    }

    if (!gameBoard.getAutoPlay())
    {
        autoPlayButton.forceState(true);
        autoPlayButton.invalidate();
        gameBoard.setAutoPlay(true);
    }
}
