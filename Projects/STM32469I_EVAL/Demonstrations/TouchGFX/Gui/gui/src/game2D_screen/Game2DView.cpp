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
  

#include <gui/game2D_screen/Game2DView.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <gui/common/Utils.hpp>

#if defined(ST) && !defined(SIMULATOR)
extern volatile bool disableChromArt;
#endif

Game2DView::Game2DView() :
    DemoView(),
    parallaxBackground(640, 290),
    chromArtOff(false),
    velocity(-11),
    collectibleItemCatched(0),
    collectibleItemMissed(0),
    playerDeadCounter(0),
    onButtonPressed(this, &Game2DView::buttonPressedhandler)
{
    playerCharacter.setBitmaps(BITMAP_SPRITE_00_ID, BITMAP_SPRITE_09_ID);
    playerCharacter.setUpdateTicksInterval(3);
}

Game2DView::~Game2DView()
{
}

void Game2DView::setupScreen()
{
    background.setPosition(0, 0, getActiveDisplayWidth(), getActiveDisplayHeight());
    background.setBitmap(Bitmap(BITMAP_GAME_2D_FRAME_BACKGROUND_ID));
    add(background);

    flyButton.setBitmaps(Bitmap(BITMAP_GAME2D_FLY_BUTTON_ID), Bitmap(BITMAP_GAME2D_FLY_BUTTON_PRESSED_ID));
    flyButton.setXY(80, 409);
    flyButton.setAction(onButtonPressed);
    add(flyButton);

#if defined(ST)
    updateChromArtButtonBitmaps();
    chromArtOnOffButton.setXY(160, 409);
    chromArtOnOffButton.setAction(onButtonPressed);
    add(chromArtOnOffButton);

    chromArtHeader.setTypedText(TypedText(T_GAME_2D_CHROM_ART_HEADER));
    chromArtHeader.setXY(chromArtOnOffButton.getRect().right() + 6, chromArtOnOffButton.getY() + 10);
    chromArtHeader.setColor(Color::getColorFrom24BitRGB(0x7B, 0xAF, 0xB6));
    add(chromArtHeader);

    chromArtDescription.setTypedText(TypedText(T_GAME_2D_CHROM_ART_DESCRIPTION));
    chromArtDescription.setXY(chromArtHeader.getX(), chromArtHeader.getY() + 24);
    chromArtDescription.setColor(Color::getColorFrom24BitRGB(0x7B, 0xAF, 0xB6));
    add(chromArtDescription);
    
#endif

    gotoMenuButton.setBitmaps(Bitmap(BITMAP_GAME2D_MENU_BUTTON_ID), Bitmap(BITMAP_GAME2D_MENU_BUTTON_PRESSED_ID));
    gotoMenuButton.setXY(20, 20);
    add(gotoMenuButton);

    mcuLoadTxt.setXY(676, 13);
    mcuLoadTxt.setColor(Color::getColorFrom24BitRGB(0x18, 0x28, 0x28));
    mcuLoadValueTxt.setTypedText(TypedText(T_DEMO_VIEW_MCU_LOAD_VALUE_CENTER));
    mcuLoadValueTxt.setPosition(mcuLoadTxt.getX(), mcuLoadTxt.getY()+25, mcuLoadTxt.getWidth(), 30);
    mcuLoadValueTxt.setColor(mcuLoadTxt.getColor());

    add(mcuLoadTxt);
    add(mcuLoadValueTxt);

    bullet.setBitmaps(BITMAP_SPHERE1_ID, BITMAP_SPHERE1_ID);
    bullet.stopAnimation();
    parallaxBackground.addCollectibleItem(bullet);

    playerCharacterStartX = parallaxBackground.getX() + 36;
    playerCharacterStartY = parallaxBackground.getRect().bottom() - playerCharacter.getHeight() - 14;
    playerCharacterMaxY = parallaxBackground.getY() + 30;

    parallaxBackground.setXY(0, 0);
    parallaxBackground.addPlayerCharacter(playerCharacter);

    for (int i = 0; i < NUMBER_OF_COLLECTIBLE_ITEMS; i++)
    {
        collectibleItem[i].setX(parallaxBackground.getRect().right());
        collectibleItem[i].stopAnimation();
        parallaxBackground.addCollectibleItem(collectibleItem[i]);
    }

    touchArea.setPosition(0,0,parallaxBackground.getWidth(),parallaxBackground.getHeight());

    gameArea.add(parallaxBackground);
    gameArea.add(touchArea);

    gameArea.setPosition(80, 115, parallaxBackground.getWidth(), parallaxBackground.getHeight());

    add(gameArea);
    scoreText.setTypedText(TypedText(T_GAME_2D_SCORE_TEXT));
    scoreText.setPosition(640, flyButton.getY()+10, 80, 64);
    scoreText.setColor(Color::getColorFrom24BitRGB(0x7B, 0xAF, 0xB6));
    add(scoreText);

    scoreValue.setWildcard(scoreValueBuf);
    scoreValue.setTypedText(TypedText(T_GAME_2D_SCORE_VALUE));
    scoreValue.setPosition(640, 440, 80, 64);
    scoreValue.setColor(Color::getColorFrom24BitRGB(248,160,0));

    add(scoreValue);
    updateScoreText();

    parallaxBackground.startAnimation();

#if defined(ST) && !defined(SIMULATOR)
    disableChromArt = chromArtOff;
#endif

    restartGame();
}


void Game2DView::tearDownScreen()
{
#if defined(ST) && !defined(SIMULATOR)
    disableChromArt = false;
#endif
}

void Game2DView::buttonPressedhandler(const AbstractButton& button)
{
    if (&button == &chromArtOnOffButton)
    {
        chromArtOff = !chromArtOff;
#if defined(ST) && !defined(SIMULATOR)
        disableChromArt = chromArtOff;
#endif
        updateChromArtButtonBitmaps();
    }
}

void Game2DView::handleTickEvent()
{
    // Stop collectible items if they have passed the left border
    for (int i = 0; i < NUMBER_OF_COLLECTIBLE_ITEMS; i++)
    {
        if (collectibleItem[i].getRect().right() < 0)
        {
            collectibleItem[i].setXY(parallaxBackground.getRect().right(), parallaxBackground.getY() + 50 + Utils::randomNumberBetween(0, 180));
            collectibleItem[i].stopAnimation();

            if (playerDeadCounter == 0)
            {
                collectibleItemMissed++;
                updateScoreText();
            }
        }
    }
    if (bullet.getRect().right() < 0)
    {
        bullet.setXY(parallaxBackground.getRect().right(), parallaxBackground.getY() + 50 + Utils::randomNumberBetween(0, 180));
        bullet.stopAnimation();
    }

    // Start new collectible items
    if (Utils::randomNumberBetween(0, 100) < 8)
    {
        for (int i = 0; i < NUMBER_OF_COLLECTIBLE_ITEMS; i++)
        {
            if (!collectibleItem[i].isRunning())
            {
                collectibleItem[i].setXY(parallaxBackground.getRect().right(), parallaxBackground.getY() + 50 + Utils::randomNumberBetween(0, 180));
                collectibleItem[i].startAnimation(Utils::randomNumberBetween(4, 14));
                break;
            }
        }
    }

    // Dead time period timeout
    if (playerDeadCounter > 0)
    {
        playerDeadCounter++;

        if (playerDeadCounter > 100)
        {
            restartGame();
        }
        return;
    }

    // Start new bullet
    if (Utils::randomNumberBetween(0, 100) < 3)
    {
        if (!bullet.isRunning())
        {
            bullet.setXY(parallaxBackground.getRect().right(), parallaxBackground.getY() + 50 + Utils::randomNumberBetween(0, 180));
            bullet.startAnimation(Utils::randomNumberBetween(14, 20));
        }
    }

    // Collect collectible items
    Rect collisionArea(playerCharacter.getRect().x, playerCharacter.getRect().y + 10, playerCharacter.getRect().width - 20, playerCharacter.getRect().height - 10);
    for (int i = 0; i < NUMBER_OF_COLLECTIBLE_ITEMS; i++)
    {
        if (collectibleItem[i].getRect().intersect(collisionArea))
        {
            collectibleItem[i].setX(parallaxBackground.getRect().right());
            collectibleItem[i].stopAnimation();
            collectibleItemCatched++;
            updateScoreText();
        }
    }


    
    if (bullet.getRect().intersect(collisionArea))
    {
        // Collision with bullet
        playerCharacter.stopAnimation();
        playerCharacter.startMoveAnimation(playerCharacter.getX() - 160, playerCharacter.getY() + 100, 40, EasingEquations::quadEaseOut, EasingEquations::quadEaseIn);
        playerDeadCounter = 1;
        bullet.setX(-bullet.getWidth());
        bullet.invalidate();
    }
    else
    {
        // The PlayerCharacter can only be controlled when it has arrived at
        // its start position
        if (playerCharacter.getX() == playerCharacterStartX)
        {
            // Control player
            if (flyButton.getPressedState() || touchArea.getPressedState())
            {
                velocity += 3;
                velocity = (velocity > 7) ? 7 : velocity;
            }
            else if (velocity > -10)
            {
                velocity -= 2;
            }

            if (velocity != 0)
            {
                int newY = playerCharacter.getY() - velocity;

                if (newY < playerCharacterMaxY)
                {
                    newY = playerCharacterMaxY;
                }
                else if (newY > playerCharacterStartY)
                {
                    newY = playerCharacterStartY;
                }

                playerCharacter.moveTo(playerCharacter.getX(), newY);
            }
        }
    }

}

void Game2DView::updateScoreText()
{
    Unicode::snprintf(scoreValueBuf, 16, "%i\0", (collectibleItemCatched * 2) - collectibleItemMissed);
    scoreValue.invalidate();
}

void Game2DView::updateChromArtButtonBitmaps()
{
    if (chromArtOff)
    {
        chromArtOnOffButton.setBitmaps(Bitmap(BITMAP_GAME2D_ST_BUTTON_OFF_ID), Bitmap(BITMAP_GAME2D_ST_BUTTON_OFF_PRESSED_ID));
    } 
    else
    {
        chromArtOnOffButton.setBitmaps(Bitmap(BITMAP_GAME2D_ST_BUTTON_ON_ID), Bitmap(BITMAP_GAME2D_ST_BUTTON_PRESSED_ID));
    }
    chromArtOnOffButton.invalidate();
}

void Game2DView::restartGame()
{
    playerDeadCounter = 0;
    collectibleItemCatched = 0;
    collectibleItemMissed = 0;

    playerCharacter.setXY(-playerCharacter.getWidth(), playerCharacterStartY);
    playerCharacter.startMoveAnimation(playerCharacterStartX, playerCharacterStartY, 32, EasingEquations::cubicEaseOut);
    playerCharacter.startAnimation(false, true, true);
    playerCharacter.invalidate();
    updateScoreText();
}
