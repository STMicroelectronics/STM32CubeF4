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
  

#include <gui/game2048_screen/GameBoard.hpp>
#include <BitmapDatabase.hpp>
#include <gui/common/Utils.hpp>

GameBoard::GameBoard() :
    currentAnimationState(NO_ANIMATION),
    score(0),
    moveCompletedAction(0),
    gameWonAction(0),
    gameLostAction(0),
    dragX(0),
    dragY(0),
    autoPlay(false),
    tileMoveEndedCallback(this, &GameBoard::tileMoveEndedHandler)
{
    setTouchable(true);

    tileOffset = Bitmap(BITMAP_TILE_0002_ID).getWidth() + TILE_SPACING;

    for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
    {
        for (int j = 0; j < NUMBER_OF_HORIZONTAL_TILES; j++)
        {
            tiles[i][j] = &tilesAlloc[i][j];
            tiles[i][j]->image.setAnimationEndedCallback(tileMoveEndedCallback);
            add(tiles[i][j]->image);
        }
    }

    initiateNewGame();
    updateBoard();

    setWidth(470);
    setHeight(480);
}

GameBoard::~GameBoard()
{
    touchgfx::Application::getInstance()->unregisterTimerWidget(this);
}

void GameBoard::handleClickEvent(const ClickEvent& evt)
{
    if (currentAnimationState != NO_ANIMATION)
    {
        return;
    }

    if (evt.getType() == ClickEvent::RELEASED)
    {
        // Analyze the dragged distance to determine if a board move should be initiated
        int xDragDistance = (dragX < 0) ? -dragX : dragX;
        int yDragDistance = (dragY < 0) ? -dragY : dragY;
        int maxDragDistance = (yDragDistance < xDragDistance) ? xDragDistance : yDragDistance;

        if (maxDragDistance > DRAG_THRESHOLD)
        {
            if (xDragDistance > yDragDistance)
            {
                (dragX > 0) ? moveTilesRight(true) : moveTilesLeft(true); 
            }
            else
            {
                (dragY > 0) ? moveTilesDown(true) : moveTilesUp(true);
            }
        }

        dragX=0;
        dragY=0;
    }
}

void GameBoard::handleDragEvent(const DragEvent& evt)
{
    dragX += evt.getDeltaX();
    dragY += evt.getDeltaY();
}

/*
void GameBoard::handleGestureEvent(const GestureEvent& evt)
{
    if (currentAnimationState != NO_ANIMATION)
    {
        return;
    }
    
    dragX=0;
    dragY=0;

    if (evt.getType() == evt.SWIPE_HORIZONTAL)
    {
       moveTilesHorizontal(evt.getVelocity());
    }
    else if (evt.getType() == evt.SWIPE_VERTICAL)
    {
       moveTilesVertical(evt.getVelocity());
    }
}
*/

void GameBoard::updateBoard()
{
    for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
    {
        for (int j = 0; j < NUMBER_OF_HORIZONTAL_TILES; j++)
        {
            // Reposition tile and update image
            tiles[i][j]->image.moveTo(20 + j*tileOffset, 25 + i*tileOffset);
            updateTile(tiles[i][j]);
        }
    }
}

void GameBoard::updateTile(Tile* tile)
{
    tile->image.setVisible(tile->state != TILE_EMPTY);
    tile->image.setBitmaps(getBitmapForTileState(tile->state), getBitmapForTileState(tile->state));
    tile->image.invalidate();
}

void GameBoard::initiateNewGame()
{
    gameStatus = ACTIVE;
    score = 0;

    for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
    {
        for (int j = 0; j < NUMBER_OF_HORIZONTAL_TILES; j++)
        {
            tiles[i][j]->state = TILE_EMPTY;
            tiles[i][j]->isPromoted = false;
        }
    }

    if (hasEmptyTile())
    {
        getRandomEmptyTile()->state = getStartTileState();
    }
    if (hasEmptyTile())
    {
        getRandomEmptyTile()->state = getStartTileState();
    }
    updateBoard();
}

bool GameBoard::hasEmptyTile()
{
    for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
    {
        for (int j = 0; j < NUMBER_OF_HORIZONTAL_TILES; j++)
        {
            if (tiles[i][j]->state == TILE_EMPTY)
            {
                return true;
            }
        }
    }
    return false;
}

GameBoard::Tile* GameBoard::getRandomEmptyTile()
{
    int numberOfEmptyTiles = 0;

    for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
    {
        for (int j = 0; j < NUMBER_OF_HORIZONTAL_TILES; j++)
        {
            if (tiles[i][j]->state == TILE_EMPTY)
            {
                numberOfEmptyTiles++;
            }
        }
    }

    if (numberOfEmptyTiles == 0)
    {
        return 0;
    }
    else
    {
        int randomEmptyTile = Utils::randomNumberBetween(0, numberOfEmptyTiles);

        for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
        {
            for (int j = 0; j < NUMBER_OF_HORIZONTAL_TILES; j++)
            {
                if (tiles[i][j]->state == TILE_EMPTY)
                {
                    if (randomEmptyTile-- == 0)
                    {
                        return tiles[i][j];
                    }
                }
            }
        }
    }

    return 0;
}


bool GameBoard::moveTileTo(int fromX, int fromY, int toX, int toY, bool performMove)
{
    if (fromX != toX || fromY != toY)
    {
        tiles[toY][toX]->state = tiles[fromY][fromX]->state;
        tiles[fromY][fromX]->state = TILE_EMPTY;

        if (performMove)
        {
            // Move image to the front
            remove(tiles[fromY][fromX]->image);
            add(tiles[fromY][fromX]->image);

            tiles[fromY][fromX]->image.startZoomAndMoveAnimation(tiles[toY][toX]->image.getX(), tiles[toY][toX]->image.getY(), tiles[toY][toX]->image.getWidth(), tiles[toY][toX]->image.getHeight(), TILE_MOVE_DURATION, ZoomAnimationImage::FIXED_LEFT_AND_TOP, EasingEquations::cubicEaseIn, EasingEquations::cubicEaseIn);
            currentAnimationState = ANIMATE_TILE_MOVEMENT;
        }
        return true;
    }
    return false;
}

void GameBoard::tileMoveEndedHandler(const ZoomAnimationImage& image)
{
    if (currentAnimationState == ANIMATE_TILE_MOVEMENT && getNumberOfAnimatingTiles() == 0)
    {
        updateBoard();

        currentAnimationState = NO_ANIMATION;
        
        // Initiate promotions
        for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
        {
            for (int j = 0; j < NUMBER_OF_HORIZONTAL_TILES; j++)
            {
                if (tiles[i][j]->isPromoted)
                {
                    int imageWidth = Bitmap(BITMAP_TILE_0002_ID).getWidth();
                    int imageHeight = Bitmap(BITMAP_TILE_0002_ID).getHeight();
                    tiles[i][j]->image.setPosition(tiles[i][j]->image.getX() + (imageWidth-50)/2, tiles[i][j]->image.getY() + (imageHeight-50)/2, 50, 50);
                    tiles[i][j]->image.startZoomAnimation(imageWidth, imageHeight, 6, ZoomAnimationImage::FIXED_CENTER, EasingEquations::backEaseInOut, EasingEquations::backEaseInOut);
                    tiles[i][j]->isPromoted = false;

                    currentAnimationState = ANIMATE_PROMOTION;
                }
            }
        }
        
        if (currentAnimationState == NO_ANIMATION)
        {
            // If no promotions - insert random tile
            insertRandomTile();
        }
    }
    else if (currentAnimationState == ANIMATE_PROMOTION && getNumberOfAnimatingTiles() == 0)
    {
        // Promotion animation ended - so insert random tile
        insertRandomTile();
    }
    else if (currentAnimationState == ANIMATE_INSERT_RANDOM_TILE && getNumberOfAnimatingTiles() == 0)
    {
        // Move animation has ended
        currentAnimationState = NO_ANIMATION;

        if (moveCompletedAction && moveCompletedAction->isValid())
        {
            moveCompletedAction->execute(*this);
        }

        if (hasGameBeenWon())
        {
            gameStatus = WON;
            if (gameWonAction && gameWonAction->isValid())
            {
                gameWonAction->execute(*this);
            }
        }
        else if (hasGameBeenLost())
        {
            gameStatus = LOST;
            if (gameLostAction && gameLostAction->isValid())
            {
                gameLostAction->execute(*this);
            }
        }

    }
}

void GameBoard::insertRandomTile()
{
    Tile* newTile = getRandomEmptyTile();
    if (newTile)
    {
        newTile->state = getStartTileState();
        updateTile(newTile);

        int imageWidth = Bitmap(BITMAP_TILE_0002_ID).getWidth();
        int imageHeight = Bitmap(BITMAP_TILE_0002_ID).getHeight();

        newTile->image.setPosition(newTile->image.getX() + imageWidth/2, newTile->image.getY() + imageHeight/2, 0, 0);
        newTile->image.startZoomAnimation(imageWidth, imageHeight, 8, ZoomAnimationImage::FIXED_CENTER, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);

        currentAnimationState = ANIMATE_INSERT_RANDOM_TILE;
    }
}


int GameBoard::getNumberOfAnimatingTiles()
{
    int result = 0;

    for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
    {
        for (int j = 0; j < NUMBER_OF_HORIZONTAL_TILES; j++)
        {
            if (tiles[i][j]->image.isRunning())
            {
                result++;
            }
        }
    }

    return result;
}

Bitmap GameBoard::getBitmapForTileState(TileState tileState)
{
    switch (tileState)
    {
    case GameBoard::TILE_EMPTY:
        return Bitmap(BITMAP_TILE_0002_ID);
    case GameBoard::TILE_0002:
        return Bitmap(BITMAP_TILE_0002_ID);
    case GameBoard::TILE_0004:
        return Bitmap(BITMAP_TILE_0004_ID);
    case GameBoard::TILE_0008:
        return Bitmap(BITMAP_TILE_0008_ID);
    case GameBoard::TILE_0016:
        return Bitmap(BITMAP_TILE_0016_ID);
    case GameBoard::TILE_0032:
        return Bitmap(BITMAP_TILE_0032_ID);
    case GameBoard::TILE_0064:
        return Bitmap(BITMAP_TILE_0064_ID);
    case GameBoard::TILE_0128:
        return Bitmap(BITMAP_TILE_0128_ID);
    case GameBoard::TILE_0256:
        return Bitmap(BITMAP_TILE_0256_ID);
    case GameBoard::TILE_0512:
        return Bitmap(BITMAP_TILE_0512_ID);
    case GameBoard::TILE_1024:
        return Bitmap(BITMAP_TILE_1024_ID);
    case GameBoard::TILE_2048:
        return Bitmap(BITMAP_TILE_2048_ID);
    case GameBoard::NUMBER_OF_TILE_STATES:
    default:
        return Bitmap(BITMAP_TILE_0002_ID);
    }
}


void GameBoard::promoteTile(int x, int y)
{
    TileState promotedState;

    switch (tiles[y][x]->state)
    {
    case GameBoard::TILE_EMPTY:
        promotedState = TILE_0002;
        break;
    case GameBoard::TILE_0002:
        promotedState = TILE_0004;
        score += 4;
        break;
    case GameBoard::TILE_0004:
        promotedState = TILE_0008;
        score += 8;
        break;
    case GameBoard::TILE_0008:
        promotedState = TILE_0016;
        score += 16;
        break;
    case GameBoard::TILE_0016:
        promotedState = TILE_0032;
        score += 32;
        break;
    case GameBoard::TILE_0032:
        promotedState = TILE_0064;
        score += 64;
        break;
    case GameBoard::TILE_0064:
        promotedState = TILE_0128;
        score += 128;
        break;
    case GameBoard::TILE_0128:
        promotedState = TILE_0256;
        score += 256;
        break;
    case GameBoard::TILE_0256:
        promotedState = TILE_0512;
        score += 512;
        break;
    case GameBoard::TILE_0512:
        promotedState = TILE_1024;
        score += 1024;
        break;
    case GameBoard::TILE_1024:
        promotedState = TILE_2048;
        score += 2048;
        break;
    case GameBoard::TILE_2048:
        promotedState = TILE_2048;
        break;
    case GameBoard::NUMBER_OF_TILE_STATES:
    default:
        promotedState = TILE_0002;
        break;
    }

    tiles[y][x]->state = promotedState;
    tiles[y][x]->isPromoted = true;
}

GameBoard::TileState GameBoard::getStartTileState()
{
    if (Utils::randomNumberBetween(0, 100) < 75)
    {
        return GameBoard::TILE_0002;
    }
    else
    {
        return GameBoard::TILE_0004;
    }
}

void GameBoard::rotateBoard()
{
    // transpose
    for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
    {
        for (int j = i; j < NUMBER_OF_HORIZONTAL_TILES; j++)
        {
            Tile* temp = tiles[i][j];
            tiles[i][j] = tiles[j][i];
            tiles[j][i] = temp;
        }
    }

    // reverse element order
    for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
    {
        for (int j = 0; j < NUMBER_OF_HORIZONTAL_TILES/2; j++)
        {
            Tile* temp = tiles[i][j];
            tiles[i][j] = tiles[i][NUMBER_OF_HORIZONTAL_TILES - j - 1];
            tiles[i][NUMBER_OF_HORIZONTAL_TILES - j - 1]= temp;     
        }
    }
}

int GameBoard::moveTilesRight(bool performMove)
{
    int numberOfTilesMoved = 0;

    for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
    {
        // The last tile in the row that can be promoted
        int lastActiveTile = NUMBER_OF_HORIZONTAL_TILES - 1;
        for (int j = NUMBER_OF_HORIZONTAL_TILES - 2; j >= 0; j--)
        {
            if (tiles[i][j]->state != TILE_EMPTY)
            {
                for (int j2 = j+1; j2 <= lastActiveTile; j2++)
                {
                    if (tiles[i][j2]->state == tiles[i][j]->state )
                    {
                        numberOfTilesMoved += moveTileTo(j, i, j2, i, performMove);
                        promoteTile(j2, i);

                        lastActiveTile = j2 - 1; // A promoted tile can not be promoted again
                        break;
                    }
                    else if (tiles[i][j2]->state != TILE_EMPTY && tiles[i][j2]->state != tiles[i][j]->state)
                    {
                        // Move to the tile before this
                        numberOfTilesMoved += moveTileTo(j, i, j2 - 1, i, performMove);

                        lastActiveTile = j2 - 1; 
                        break;
                    }
                    else if (tiles[i][j2]->state == TILE_EMPTY && j2 == lastActiveTile)
                    {
                        // Last column has an empty tile
                        numberOfTilesMoved += moveTileTo(j, i, j2, i, performMove);
                        lastActiveTile = j2;
                        break;
                    }
                }
            }
        }
    }

    return numberOfTilesMoved;
}

int GameBoard::moveTilesLeft(bool performMove)
{
    rotateBoard();
    rotateBoard();
    int numberOfTilesMoved = moveTilesRight(performMove);
    rotateBoard();
    rotateBoard();
    return numberOfTilesMoved;
}

int GameBoard::moveTilesUp(bool performMove)
{
    rotateBoard();
    int numberOfTilesMoved = moveTilesRight(performMove);
    rotateBoard();
    rotateBoard();
    rotateBoard();
    return numberOfTilesMoved;
}

int GameBoard::moveTilesDown(bool performMove)
{
    rotateBoard();
    rotateBoard();
    rotateBoard();
    int numberOfTilesMoved = moveTilesRight(performMove);
    rotateBoard();
    return numberOfTilesMoved;
}

bool GameBoard::hasGameBeenWon()
{
    for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
    {
        for (int j = 0; j < NUMBER_OF_HORIZONTAL_TILES; j++)
        {
            if (tiles[i][j]->state == TILE_2048)
            {
                return true;
            }
        }
    }
    return false;
}

bool GameBoard::hasGameBeenLost()
{
    bool result = true;
    takeStateBackup();
    
    result &= (moveTilesRight(false) == 0);
    restoreStateBackup();

    result &= (moveTilesLeft(false) == 0);
    restoreStateBackup();

    result &= (moveTilesUp(false) == 0);
    restoreStateBackup();

    result &= (moveTilesDown(false) == 0);
    restoreStateBackup();

    return result;
}

void GameBoard::takeStateBackup()
{
    scoreBackup = score;
    for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
    {
        for (int j = 0; j < NUMBER_OF_HORIZONTAL_TILES; j++)
        {
            tileStateBackup[i][j] = tiles[i][j]->state; 
            tilePromotionBackup[i][j] = tiles[i][j]->isPromoted;
        }
    }
}

void GameBoard::restoreStateBackup()
{
    score = scoreBackup;
    for (int i = 0; i < NUMBER_OF_VERTICAL_TILES; i++)
    {
        for (int j = 0; j < NUMBER_OF_HORIZONTAL_TILES; j++)
        {
            tiles[i][j]->state = tileStateBackup[i][j]; 
            tiles[i][j]->isPromoted = tilePromotionBackup[i][j];
        }
    }
}

void GameBoard::handleTickEvent()
{
    if (autoPlay && currentAnimationState == NO_ANIMATION && gameStatus == ACTIVE)
    {
        if (autoPlayPause-- == 0)
        {
            /*
            // Autoplay mode that picks the move with the most tiles that moves
            int move = 0;
            int maxNumberOfTilesMoved = 0;
            int numberOfTilesMoved = 0;
            takeStateBackup();
    
            numberOfTilesMoved = moveTilesRight(false);
            if (numberOfTilesMoved >= maxNumberOfTilesMoved) { move = 0; maxNumberOfTilesMoved = numberOfTilesMoved; }
            restoreStateBackup();
    
            numberOfTilesMoved = moveTilesLeft(false);
            if (numberOfTilesMoved >= maxNumberOfTilesMoved) { move = 1; maxNumberOfTilesMoved = numberOfTilesMoved; }
            restoreStateBackup();
    
            numberOfTilesMoved = moveTilesUp(false);
            if (numberOfTilesMoved >= maxNumberOfTilesMoved) { move = 2; maxNumberOfTilesMoved = numberOfTilesMoved; }
            restoreStateBackup();
    
            numberOfTilesMoved = moveTilesDown(false);
            if (numberOfTilesMoved >= maxNumberOfTilesMoved) { move = 3; maxNumberOfTilesMoved = numberOfTilesMoved; }
            restoreStateBackup();
            */
    
            // Autoplay mode that picks the move that scores the most
            int move = 0;
            int maxScore = 0;
            int numberOfTilesMoved = 0;
            takeStateBackup();
    
            numberOfTilesMoved = moveTilesRight(false);
            if (score >= maxScore && numberOfTilesMoved > 0) { move = 0; maxScore = score; }
            restoreStateBackup();
    
            numberOfTilesMoved = moveTilesLeft(false);
            if (score >= maxScore && numberOfTilesMoved > 0) { move = 1; maxScore = score; }
            restoreStateBackup();
    
            numberOfTilesMoved = moveTilesUp(false);
            if (score >= maxScore && numberOfTilesMoved > 0) { move = 2; maxScore = score; }
            restoreStateBackup();
    
            numberOfTilesMoved = moveTilesDown(false);
            if (score >= maxScore && numberOfTilesMoved > 0) { move = 3; maxScore = score; }
            restoreStateBackup();
    
    
            if (move == 0)
            {
                moveTilesRight(true);
            }
            else if (move == 1)
            {
                moveTilesLeft(true);
            }
            else if (move == 2)
            {
                moveTilesUp(true);
            }
            else if (move == 3)
            {
                moveTilesDown(true);
            }

            autoPlayPause = 14;
        }
    }
}

void GameBoard::setAutoPlay(bool newAutoPlay)
{
    autoPlay = newAutoPlay; 

    if (newAutoPlay)
    {
        autoPlayPause = 0;
        touchgfx::Application::getInstance()->registerTimerWidget(this);
    } 
    else
    {    
        touchgfx::Application::getInstance()->unregisterTimerWidget(this);
    }
}
