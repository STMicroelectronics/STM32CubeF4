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
  

#include <gui/product_presenter_screen/FeaturePoster.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/Color.hpp>

FeaturePoster::FeaturePoster() :
    startAnimationCallback(this, &FeaturePoster::startAnimationHandler),
    moveAnimationEnded(this, &FeaturePoster::moveAnimationEndedHandler)
{
    shadow[0].setBitmap(Bitmap(BITMAP_POSTER_SHADOW_TOP_ID));
    shadow[0].setXY(0, 0);
    add(shadow[0]);

    shadow[1].setBitmap(Bitmap(BITMAP_POSTER_SHADOW_LEFT_ID));
    shadow[1].setXY(0, shadow[0].getHeight());
    add(shadow[1]);

    shadow[2].setBitmap(Bitmap(BITMAP_POSTER_SHADOW_RIGHT_ID));
    shadow[2].setXY(shadow[0].getWidth() - shadow[2].getWidth(), shadow[0].getHeight());
    add(shadow[2]);

    shadow[3].setBitmap(Bitmap(BITMAP_POSTER_SHADOW_BOTTOM_ID));
    shadow[3].setXY(0, shadow[1].getRect().bottom());
    add(shadow[3]);

    posterImage.setBitmap(Bitmap(BITMAP_POSTER_IMAGE_00_ID));
    posterImage.setXY(0, 0);

    posterArea.setPosition(shadow[1].getWidth(), shadow[0].getHeight(), posterImage.getWidth(), 350);


    startAnimationButton.setBitmaps(Bitmap(BITMAP_POSTER_ANIMATION_BUTTON_ID), Bitmap(BITMAP_POSTER_ANIMATION_BUTTON_ID));
    startAnimationButton.setXY(192, 202);
    startAnimationButton.setAction(startAnimationCallback);

    startAnimationButtonArrow.setBitmaps(BITMAP_POSTER_ANIMATION_BUTTON_ARROW_00_ID, BITMAP_POSTER_ANIMATION_BUTTON_ARROW_06_ID);
    startAnimationButtonArrow.setXY(startAnimationButton.getX() + 39, startAnimationButton.getY() + 35);
    startAnimationButtonArrow.setUpdateTicksInterval(3);

    alphaBox.setColor(Color::getColorFrom24BitRGB(0x0B, 0x5E, 0x93));
    alphaBox.setPosition(0, 0, posterImage.getWidth(), 40);
    alphaBox.setAlpha(0);

    textAreaContainer.setPosition(posterImage.getX(), posterImage.getRect().bottom(), posterImage.getWidth(), 400);

    textBackground.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    textBackground.setPosition(0, 0, textAreaContainer.getWidth(), textAreaContainer.getHeight());

    textStartY = 20;
    text.setTypedText(TypedText(T_POSTER_00_TEXT));
    text.setColor(Color::getColorFrom24BitRGB(0x9C, 0x9C, 0x9C));
    text.setXY(16, textStartY);

    redBall.setBitmap(Bitmap(BITMAP_ALPHA_BALL_RED_ID));
    redBall.setXY(0, 196);
    redBall.setMoveAnimationEndedAction(moveAnimationEnded);
    redBall.setVisible(false);

    greenBall.setBitmap(Bitmap(BITMAP_ALPHA_BALL_GREEN_ID));
    greenBall.setXY(0, 196);
    greenBall.setMoveAnimationEndedAction(moveAnimationEnded);
    greenBall.setVisible(false);

    blueBall.setBitmap(Bitmap(BITMAP_ALPHA_BALL_BLUE_ID));
    blueBall.setXY(0, 196);
    blueBall.setMoveAnimationEndedAction(moveAnimationEnded);
    blueBall.setVisible(false);
    
    textAreaContainer.add(textBackground);
    textAreaContainer.add(text);    
    textAreaContainer.add(redBall);
    textAreaContainer.add(greenBall);
    textAreaContainer.add(blueBall);

    gradientOverlay.setBitmap(Bitmap(BITMAP_POSTER_GRADIENT_OVERLAY_ID));
    gradientOverlay.setXY(0, posterArea.getRect().bottom() - gradientOverlay.getHeight());

    posterArea.add(posterImage);
    posterArea.add(alphaBox);
    posterArea.add(headlineTop);
    posterArea.add(headlineBottom);
    posterArea.add(textAreaContainer);
    posterArea.add(startAnimationButton);
    posterArea.add(startAnimationButtonArrow);
    posterArea.add(gradientOverlay);
    add(posterArea);

    setWidth(shadow[1].getWidth() + posterImage.getWidth() + shadow[2].getWidth());
    setHeight(shadow[0].getHeight() + shadow[1].getHeight() + shadow[3].getHeight());

    currentState = SHOWING_IMAGE;
}

FeaturePoster::~FeaturePoster()
{

}

void FeaturePoster::setup(BitmapId newPosterImage, TEXTS posterHeadline, TEXTS posterText, bool showAlphaExample)
{
    this->posterImage.setBitmap(Bitmap(newPosterImage));

    headlineTop.setTypedText(TypedText(posterHeadline));
    headlineTop.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    headlineTopStartY = -30 - headlineTop.getTypedText().getFont()->getFontHeight();
    headlineTop.setXY(16, headlineTopStartY);
    headlineTop.resizeToCurrentText();

    headlineBottom.setTypedText(TypedText(posterHeadline));
    headlineBottom.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    headlineBottom.setXY(16, textAreaContainer.getY() - 10 - headlineBottom.getTypedText().getFont()->getFontHeight());
    headlineBottom.resizeToCurrentText();

    text.setTypedText(TypedText(posterText));

    if (showAlphaExample)
    {
        redBall.setVisible(true);
        greenBall.setVisible(true);
        blueBall.setVisible(true);

        moveAnimationEndedHandler(redBall);
        moveAnimationEndedHandler(greenBall);
        moveAnimationEndedHandler(blueBall);
    } 
}

void FeaturePoster::setHeadlineColor(uint16_t color)
{
    headlineBottom.setColor(color);
    headlineBottom.invalidate();
}

void FeaturePoster::animate()
{
    if (currentState == SHOWING_IMAGE)
    {
        posterImage.startMoveAnimation(0, -70, 25, EasingEquations::linearEaseNone, EasingEquations::quadEaseInOut);

        textAreaContainer.startMoveAnimation(0, alphaBox.getHeight(), 25, EasingEquations::linearEaseNone, EasingEquations::quadEaseInOut);    

        alphaBox.setFadeAnimationDelay(15);
        alphaBox.startFadeAnimation(255, 17);

        headlineTop.setMoveAnimationDelay(5);
        headlineTop.startMoveAnimation(headlineTop.getX(), 29 - headlineTop.getTypedText().getFont()->getFontHeight(), 25, EasingEquations::linearEaseNone, EasingEquations::quadEaseInOut);

        gradientOverlay.setFadeAnimationDelay(0);
        gradientOverlay.startFadeAnimation(0, 4);

        int startAnimationButtonEndY = alphaBox.getHeight() - startAnimationButton.getHeight() + 25;
        startAnimationButton.startMoveAnimation(startAnimationButton.getX(), startAnimationButtonEndY, 25, EasingEquations::linearEaseNone, EasingEquations::quadEaseInOut);
        startAnimationButtonArrow.startMoveAnimation(startAnimationButton.getX() + 39, startAnimationButtonEndY + 35, 25, EasingEquations::linearEaseNone, EasingEquations::quadEaseInOut);
        startAnimationButtonArrow.startAnimation(false, false, false);
        
        currentState = SHOWING_TEXT;
    } 
    else if (currentState == SHOWING_TEXT)
    {
        posterImage.startMoveAnimation(0, 0, 25, EasingEquations::linearEaseNone, EasingEquations::quadEaseInOut);
        
        textAreaContainer.startMoveAnimation(0, posterImage.getHeight(), 25, EasingEquations::linearEaseNone, EasingEquations::quadEaseInOut);

        alphaBox.setFadeAnimationDelay(8);
        alphaBox.startFadeAnimation(0, 17);

        headlineTop.setMoveAnimationDelay(0);
        headlineTop.startMoveAnimation(headlineTop.getX(), headlineTopStartY, 25, EasingEquations::linearEaseNone, EasingEquations::quadEaseInOut);

        gradientOverlay.setFadeAnimationDelay(20);
        gradientOverlay.startFadeAnimation(255, 8);

        startAnimationButton.startMoveAnimation(startAnimationButton.getX(), 202, 25, EasingEquations::linearEaseNone, EasingEquations::quadEaseInOut);
        startAnimationButtonArrow.startMoveAnimation(startAnimationButton.getX() + 39, 202 + 35, 25, EasingEquations::linearEaseNone, EasingEquations::quadEaseInOut);
        startAnimationButtonArrow.startAnimation(true, false, false);
        
        currentState = SHOWING_IMAGE;
    }
}

void FeaturePoster::startAnimationHandler(const AbstractButton& button)
{
    animate();
}

void FeaturePoster::invalidateText()
{
    headlineTop.invalidate();
    headlineTop.resizeToCurrentText();
    headlineTop.invalidate();
    headlineBottom.invalidate();
    headlineBottom.resizeToCurrentText();
    headlineBottom.invalidate();
    text.invalidate();
}

void FeaturePoster::moveAnimationEndedHandler(const MoveAnimator<Image>& image)
{
    MoveAnimator<Image>* active = &blueBall;
    int duration = 70;
    
    if(&image == &redBall)
    {
        active = &redBall;
        duration = 130;
    }
    else if(&image == &greenBall)
    {
        active = &greenBall;
        duration = 250;
    }
    
    int y = text.getTextHeight()+20;

    int x = active->getX() < textAreaContainer.getWidth()/2 ? textAreaContainer.getWidth()-active->getWidth()-30 : 30;
    active->startMoveAnimation(x,y,duration, EasingEquations::cubicEaseInOut);
}

void FeaturePoster::toggleViewMode()
{
    startAnimationHandler(startAnimationButton);
}
